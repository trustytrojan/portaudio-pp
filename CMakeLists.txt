cmake_minimum_required(VERSION 3.15)
project(portaudio-pp LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(portaudio-pp INTERFACE)
add_library(portaudio-pp::portaudio-pp ALIAS portaudio-pp)

# find, require, and link to the portaudio C library
if(WIN32)
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "AMD64|x86_64")
        message("windows x64 detected, fetching portaudio dll...")
        file(DOWNLOAD
            https://github.com/spatialaudio/portaudio-binaries/raw/refs/heads/master/libportaudio64bit.dll
            ${CMAKE_CURRENT_BINARY_DIR}/portaudio_x64.dll)
        if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/portaudio.h)
            file(DOWNLOAD
                https://github.com/PortAudio/portaudio/raw/refs/tags/v19.7.0/include/portaudio.h
                ${CMAKE_CURRENT_BINARY_DIR}/portaudio.h)
        endif()
        target_include_directories(portaudio-pp INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
        target_link_directories(portaudio-pp INTERFACE ${CMAKE_CURRENT_BINARY_DIR})
        target_link_libraries(portaudio-pp INTERFACE portaudio_x64)
    else()
        message("other architecture detected, fetching portaudio source...")
        FetchContent_Declare(portaudio
            URL https://files.portaudio.com/archives/pa_stable_v190700_20210406.tgz)
        FetchContent_MakeAvailable(portaudio)
        # portaudio itself is a CMake project, so we can just link and be done
        target_link_libraries(portaudio-pp INTERFACE portaudio)
    endif()
else()
    # On non-Windows platforms, find the library and header path.
    find_path(PORTAUDIO_INCLUDE_DIR NAMES portaudio.h DOC "PortAudio include directory")
    find_library(PORTAUDIO_LIBRARY NAMES portaudio DOC "PortAudio library")

    if(PORTAUDIO_INCLUDE_DIR AND PORTAUDIO_LIBRARY)
        message(STATUS "Found PortAudio headers: ${PORTAUDIO_INCLUDE_DIR}")
        message(STATUS "Found PortAudio library: ${PORTAUDIO_LIBRARY}")
        target_include_directories(portaudio-pp INTERFACE ${PORTAUDIO_INCLUDE_DIR})
        target_link_libraries(portaudio-pp INTERFACE ${PORTAUDIO_LIBRARY})
    else()
        message(FATAL_ERROR "PortAudio not found. Please install PortAudio or set PORTAUDIO_INCLUDE_DIR and PORTAUDIO_LIBRARY manually.")
    endif()
endif()

# forward our include directory to dependent projects
target_include_directories(portaudio-pp INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

option(PORTAUDIO_PP_BUILD_EXAMPLES "Build the examples" OFF)

if(PORTAUDIO_PP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
