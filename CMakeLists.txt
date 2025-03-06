cmake_minimum_required(VERSION 3.30)
project(Raytracing_demo C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/Users/latep/OneDrive/Työpöytä/SDL2-devel-2.32.2-mingw.tar/SDL2-devel-2.32.2-mingw/SDL2-2.32.2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIR})
add_executable(Raytracing_demo main.c)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})