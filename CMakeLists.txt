cmake_minimum_required(VERSION 3.13)
project(2048)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "")

include_directories(${PROJECT_SOURCE_DIR}/SDL-2.0.7/include)
link_directories(${PROJECT_SOURCE_DIR}/SDL-2.0.7/lib/x86)

set(SOURCE_FILES main.cpp main.cpp core/core.cpp core/core.h core/display.cpp core/display.h)
add_executable(2048 ${SOURCE_FILES})

target_link_libraries(2048 SDL2main SDL2)