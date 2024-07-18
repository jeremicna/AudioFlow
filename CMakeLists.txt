cmake_minimum_required(VERSION 3.25)
project(eq_cpp)

set(CMAKE_CXX_STANDARD 17)

# Find CoreAudio and CoreFoundation frameworks
find_library(CoreAudio CoreAudio)
find_library(CoreFoundation CoreFoundation)
find_library(AudioToolbox AudioToolbox)


# Link against CoreAudio and CoreFoundation
set(SOURCES
        main.cpp
        Amplifier.cpp
        AudioProcessor.cpp
        IIRFilter.cpp
        Equalizer.cpp
        )

add_executable(eq_cpp ${SOURCES})

target_link_libraries(eq_cpp ${CoreAudio} ${CoreFoundation} ${AudioToolbox})
