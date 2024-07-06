cmake_minimum_required(VERSION 3.25)
project(eq_cpp)

set(CMAKE_CXX_STANDARD 17)

# Find CoreAudio and CoreFoundation frameworks
find_library(CoreAudio CoreAudio)
find_library(CoreFoundation CoreFoundation)
find_library(AudioToolbox AudioToolbox)

# Link against CoreAudio and CoreFoundation

add_executable(eq_cpp main.cpp)

target_link_libraries(eq_cpp ${CoreAudio} ${CoreFoundation} ${AudioToolbox})
