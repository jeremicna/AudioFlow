//
// Created by Jeremi Campagna on 2024-07-18.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "readIRFile.h"
#include "../fileutils/globals.h"

struct WAVHeader {
    char chunkID[4];
    uint32_t chunkSize;
    char format[4];
};

struct Chunk {
    char chunkID[4];
    uint32_t chunkSize;
    std::vector<char> data;
};

std::vector<float> readIRFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file) {
        std::cerr << "Cannot open file: " << path << std::endl;
    }

    WAVHeader header;
    file.read(reinterpret_cast<char*>(&header), sizeof(header));
    if (std::strncmp(header.chunkID, "RIFF", 4) != 0 || std::strncmp(header.format, "WAVE", 4) != 0) {
        std::cerr << "Not a valid WAV file." << std::endl;
    }

    std::vector<Chunk> chunks;

    while (file.tellg() < header.chunkSize + 8) {
        Chunk chunk;
        file.read(reinterpret_cast<char*>(&chunk.chunkID), sizeof(chunk.chunkID));
        file.read(reinterpret_cast<char*>(&chunk.chunkSize), sizeof(chunk.chunkSize));

        chunk.data.resize(chunk.chunkSize);
        file.read(chunk.data.data(), chunk.chunkSize);

        chunks.push_back(chunk);
    }

    uint16_t numChannels;
    uint32_t sampleRate;
    uint16_t bitsPerSample;
    std::vector<float> audioData;

    for (const auto& chunk : chunks) {
        if (std::string(chunk.chunkID, 4) == "fmt ") {
            if (chunk.chunkSize >= 16) {
                numChannels = *reinterpret_cast<const uint16_t*>(chunk.data.data() + 2);
                sampleRate = *reinterpret_cast<const uint32_t*>(chunk.data.data() + 4);
                bitsPerSample = *reinterpret_cast<const uint16_t*>(chunk.data.data() + 14);

                if (numChannels != 2 || sampleRate != 48000 || bitsPerSample != 16) {
                    std::cerr << "Only 32-bit 48kHz stereo wav files are supported for impulse responses." << std::endl;
                }
            } else {
                std::cerr << "Unexpected fmt chunk size: " << chunk.chunkSize << std::endl;
            }
        }

        if (std::string(chunk.chunkID, 4) == "data") {
            if (bitsPerSample == 16) {
                for (size_t i = 0; i < chunk.data.size(); i += 4) {
                    int16_t sample = *reinterpret_cast<const int16_t*>(&chunk.data[i]);
                    float normalizedSample = sample / 32768.0f / 8;
                    audioData.push_back(normalizedSample);
                }
                if (audioData.size() < convolutionChunkSize) {
                    audioData.resize(convolutionChunkSize, 0.0f);
                }
            } else {
                std::cerr << "Unsupported bits per sample: " << bitsPerSample << std::endl;
            }
        }
    }

    file.close();

    return audioData;
}