cmake_minimum_required(VERSION 3.17)
project(fsgcore)

# C++ 17
set(CMAKE_CXX_STANDARD 17)

# FSGCore Include Directory
include_directories(Include)

# Generic
file(GLOB_RECURSE FSGCORE ${CMAKE_SOURCE_DIR} "Source/**/Generic/*.cpp")

# Windows
file(GLOB_RECURSE FSGCORE_WIN ${CMAKE_SOURCE_DIR} "Source/**/Win/*.cpp")
# Linux
file(GLOB_RECURSE FSGCORE_LINUX ${CMAKE_SOURCE_DIR} "Source/**/Linux/*.cpp")


# Building
add_library(fsgcore SHARED ${FSGCORE} ${FSGCORE_WIN} ${FSGCORE_LINUX})