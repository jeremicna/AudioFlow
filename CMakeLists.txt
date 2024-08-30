cmake_minimum_required(VERSION 3.25)
project(eq_cpp)

set(CMAKE_CXX_STANDARD 17)

find_library(CoreAudio CoreAudio)
find_library(CoreFoundation CoreFoundation)
find_library(AudioToolbox AudioToolbox)
find_library(Accelerate Accelerate)

set(SOURCES
        src/main.cpp
        src/processing.cpp
        processing/amplifier.cpp
        processing/iirFilter.cpp
        processing/equalizer.cpp
        processing/convolutionReverb.cpp
        processing/smoother.cpp
        processing/audioProcessor.cpp
        fileutils/readIRFile.cpp
        fileutils/config.cpp
        fileutils/globals.cpp
        )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../build)

add_executable(eq_cpp ${SOURCES})

target_link_libraries(eq_cpp ${CoreAudio} ${CoreFoundation} ${AudioToolbox} ${Accelerate})
