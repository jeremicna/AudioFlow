#include <CoreFoundation/CoreFoundation.h>
#include <CoreAudio/CoreAudio.h>
#include <iostream>
#include <map>
#include "processing/audioProcessor.h"
#include "processing/equalizer.h"
#include "fileutils/readIRFile.h"
#include "processing/convolutionReverb.h"
#define driver "HOLLY 2ch"


UInt32 driverID;
UInt32 defaultDeviceID;
std::vector<float> sharedBuffer;
std::mutex bufferMutex;
AudioProcessor* audioProcessor = nullptr;


std::map<UInt32 , std::string> getAudioDevices() {
    AudioObjectPropertyAddress propAddress;
    propAddress.mSelector = kAudioHardwarePropertyDevices;
    propAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propAddress.mElement = kAudioObjectPropertyElementMain;

    UInt32 propSize;
    OSStatus status = AudioObjectGetPropertyDataSize(
            kAudioObjectSystemObject, &propAddress, 0, nullptr, &propSize);

    if (status != noErr) {
        std::cerr << "Error getting device list size." << std::endl;
        return std::map<unsigned int, std::string>{};
    }

    AudioDeviceID* devices = new AudioDeviceID[propSize / sizeof(AudioDeviceID)];

    status = AudioObjectGetPropertyData(
            kAudioObjectSystemObject, &propAddress, 0, nullptr, &propSize, devices);

    if (status != noErr) {
        std::cerr << "Error getting device list." << std::endl;
        delete[] devices;
        return std::map<unsigned int, std::string>{};
    }

    std::map<AudioDeviceID, std::string> m;
    for (UInt32 i = 0; i < propSize / sizeof(AudioDeviceID); ++i) {
        CFStringRef cfName;
        propAddress.mSelector = kAudioDevicePropertyDeviceName;
        propAddress.mScope = kAudioObjectPropertyScopeGlobal;
        propAddress.mElement = kAudioObjectPropertyElementMain;

        UInt32 size = sizeof(CFStringRef);
        status = AudioObjectGetPropertyData(
                devices[i], &propAddress, 0, nullptr, &size, &cfName);

        if (status != noErr) {
            std::cerr << "Error getting device list." << std::endl;
            delete[] devices;
            return std::map<unsigned int, std::string>{};
        }

        AudioDeviceID deviceId = devices[i];
        char deviceName[256];

        propAddress.mSelector = kAudioObjectPropertyName;
        propAddress.mScope = kAudioObjectPropertyScopeGlobal;
        propAddress.mElement = kAudioObjectPropertyElementMain;

        UInt32 dataSize = sizeof(CFStringRef);

        status = AudioObjectGetPropertyData(
                deviceId,
                &propAddress,
                0,
                nullptr,
                &dataSize,
                &cfName
        );

        if (status != noErr) {
            std::cerr << "Error getting device name: " << status << std::endl;
            return std::map<unsigned int, std::string>{};
        }

        CFStringGetCString(cfName, deviceName, sizeof(deviceName), kCFStringEncodingUTF8);
        CFRelease(cfName);

        m[deviceId] = deviceName;
    }

    delete[] devices;
    return m;
}

UInt32 getDefaultOutputDevice() {
    OSStatus status;
    AudioObjectPropertyAddress propertyAddress;
    AudioObjectID deviceID;

    propertyAddress.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = kAudioObjectPropertyElementMain;

    UInt32 dataSize = sizeof(deviceID);
    status = AudioObjectGetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, nullptr, &dataSize, &deviceID);
    if (status != noErr) {
        std::cerr << "Error getting default output device ID." << std::endl;
        return 0; // Return 0 or another appropriate error value
    }

    return deviceID;
}

bool setDefaultOutputDevice(UInt32 deviceID) {
    OSStatus status;
    AudioObjectPropertyAddress propertyAddress;

    propertyAddress.mSelector = kAudioHardwarePropertyDefaultOutputDevice;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = kAudioObjectPropertyElementMain;

    UInt32 dataSize = sizeof(deviceID);
    status = AudioObjectSetPropertyData(kAudioObjectSystemObject, &propertyAddress, 0, nullptr, dataSize, &deviceID);
    if (status != noErr) {
        std::cerr << "Error setting default output device ID." << std::endl;
        return false;
    }

    return true;
}

bool setAudioDeviceBufferSize(AudioDeviceID deviceID, UInt32 bufferSizeInFrames) {
    OSStatus status;
    AudioObjectPropertyAddress propertyAddress;
    propertyAddress.mSelector = kAudioDevicePropertyBufferFrameSize;
    propertyAddress.mScope = kAudioObjectPropertyScopeGlobal;
    propertyAddress.mElement = kAudioObjectPropertyElementMain;

    status = AudioObjectSetPropertyData(deviceID, &propertyAddress, 0, nullptr, sizeof(UInt32), &bufferSizeInFrames);
    if (status != noErr) {
        std::cerr << "Error setting buffer size: " << status << std::endl;
        return false;
    }

    return true;
}

float getAudioDeviceVolume(UInt32 deviceID) {
    OSStatus status;
    float volume;

    AudioObjectPropertyAddress volumeAddress;
    volumeAddress.mSelector = kAudioDevicePropertyVolumeScalar;
    volumeAddress.mScope = kAudioDevicePropertyScopeOutput;
    volumeAddress.mElement = kAudioObjectPropertyElementMain;

    UInt32 dataSize = sizeof(float);
    status = AudioObjectGetPropertyData(deviceID, &volumeAddress, 0, nullptr, &dataSize, &volume);
    if (status != noErr) {
        std::cerr << "Error getting volume." << std::endl;
        return -1.0; // Return a negative value to indicate error
    }

    return volume;
}

bool setAudioDeviceVolume(UInt32 deviceID, float volume) {
    OSStatus status;

    AudioObjectPropertyAddress volumeAddress;
    volumeAddress.mSelector = kAudioDevicePropertyVolumeScalar;
    volumeAddress.mScope = kAudioDevicePropertyScopeOutput;
    volumeAddress.mElement = kAudioObjectPropertyElementMain;

    status = AudioObjectSetPropertyData(deviceID, &volumeAddress, 0, nullptr, sizeof(float), &volume);
    if (status != noErr) {
        std::cerr << "Error setting property data." << std::endl;
        return false;
    }

    UInt32 mute = floor(1 - volume);
    AudioObjectPropertyAddress muteAddress;
    muteAddress.mSelector = kAudioDevicePropertyMute;
    muteAddress.mScope = kAudioDevicePropertyScopeOutput;
    muteAddress.mElement = kAudioObjectPropertyElementMain;

    status = AudioObjectSetPropertyData(deviceID, &muteAddress, 0, nullptr, sizeof(UInt32), &mute);
    if (status != noErr) {
        std::cerr << "Error unmuting device." << std::endl;
        return false;
    }

    return true;
}


void cleanup() {
    float driverVolume = getAudioDeviceVolume(driverID);
    setAudioDeviceVolume(defaultDeviceID, driverVolume);
    setDefaultOutputDevice(defaultDeviceID);
}

OSStatus driverIOProc(
        AudioObjectID inDevice,
        const AudioTimeStamp* inNow,
        const AudioBufferList* inInputData,
        const AudioTimeStamp* inInputTime,
        AudioBufferList* outOutputData,
        const AudioTimeStamp* inOutputTime,
        void* inClientData
) {
    bufferMutex.lock();
    for (int i = 0; i < inInputData->mNumberBuffers; ++i) {
        AudioBuffer buffer = inInputData->mBuffers[i];
        float* audioData = (float*)buffer.mData;
        UInt32 numSamples = buffer.mDataByteSize / sizeof(float);

        for (int j = 0; j < numSamples; ++j) {
            sharedBuffer.push_back(audioData[j]);
        }
    }
    bufferMutex.unlock();

    return noErr;
}

OSStatus defaultDeviceIOProc(
        AudioObjectID inDevice,
        const AudioTimeStamp* inNow,
        const AudioBufferList* inInputData,
        const AudioTimeStamp* inInputTime,
        AudioBufferList* outOutputData,
        const AudioTimeStamp* inOutputTime,
        void* inClientData
) {
    bufferMutex.lock();
    for (UInt32 i = 0; i < outOutputData->mNumberBuffers; ++i) {
        AudioBuffer outBuffer = outOutputData->mBuffers[i];
        float* outputData = (float*)outBuffer.mData;
        UInt32 numSamples = outBuffer.mDataByteSize / sizeof(float);

        audioProcessor->process(sharedBuffer);

        for (int j = 0; j < numSamples && !sharedBuffer.empty(); ++j) {
            outputData[j] = sharedBuffer.front();
            sharedBuffer.erase(sharedBuffer.begin());
        }
    }
    bufferMutex.unlock();

    return noErr;
}

// Separate main for win and mac
int main() {
    std::vector<float> f = {32, 64, 125, 250, 500, 1000, 2000, 4000, 8000, 16000};
    std::vector<float> q = {1.41, 1.41, 1.41, 1.41, 1.41, 1.41, 1.41, 1.41, 1.41, 1.41};
    std::vector<float> g = {15, 12.3, 9.7, 7.4, 3.1, 0, 0, 0, 0, 0};

    std::vector<float> ir = readIRFile("/Users/jeremicampagna/Desktop/internship grind/projects/eq-cpp/assets/ir.wav");

    audioProcessor = new AudioProcessor(Amplifier(-15), Equalizer(f, q, g, 48000), ConvolutionReverb(ir));

    // Get device IDs
    std::map<UInt32, std::string> ad = getAudioDevices();
    for (auto const& [key, val] : ad) {
        if (val == driver) {
            driverID = key;
        }
    }
    defaultDeviceID = getDefaultOutputDevice();

    // Volume and device swaps
    float defaultDeviceVolume = getAudioDeviceVolume(defaultDeviceID);
    setAudioDeviceVolume(driverID, defaultDeviceVolume);
    setDefaultOutputDevice(driverID);
    setAudioDeviceVolume(defaultDeviceID, 1);

    // Set buffer size
    UInt32 bufferSizeInFrames = 4096; // Choose your desired buffer size
    if (!setAudioDeviceBufferSize(driverID, bufferSizeInFrames)) {
        std::cerr << "Failed to set buffer size for driver device." << std::endl;
    }
    if (!setAudioDeviceBufferSize(defaultDeviceID, bufferSizeInFrames)) {
        std::cerr << "Failed to set buffer size for default output device." << std::endl;
    }

    // Create audio device processes
    AudioDeviceIOProcID inputIOProcId;
    AudioDeviceIOProcID outputIOProcID;

    AudioDeviceCreateIOProcID(driverID, driverIOProc, nullptr, &inputIOProcId);
    AudioDeviceCreateIOProcID(defaultDeviceID, defaultDeviceIOProc, nullptr, &outputIOProcID);

    // Open the audio device for input or output
    AudioDeviceStart(driverID, inputIOProcId);
    AudioDeviceStart(defaultDeviceID, outputIOProcID);

    sleep(300);

    // Cleanup: Stop and release resources
    AudioDeviceStop(driverID, inputIOProcId);
    AudioDeviceStop(defaultDeviceID, outputIOProcID);

    AudioDeviceDestroyIOProcID(driverID, inputIOProcId);
    AudioDeviceDestroyIOProcID(defaultDeviceID, outputIOProcID);

    // Return to default output device and original volume
    ::atexit(cleanup);
}

