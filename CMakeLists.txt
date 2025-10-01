cmake_minimum_required(VERSION 3.15)
project(portaudio-pp LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(PORTAUDIO REQUIRED portaudio-2.0)

add_library(portaudio-pp INTERFACE)
add_library(portaudio-pp::portaudio-pp ALIAS portaudio-pp)

target_include_directories(portaudio-pp INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${PORTAUDIO_INCLUDE_DIRS}
)

target_link_libraries(portaudio-pp INTERFACE ${PORTAUDIO_LIBRARIES})

option(PORTAUDIO_PP_BUILD_EXAMPLES "Build the examples" OFF)

if(PORTAUDIO_PP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
