cmake_minimum_required(VERSION 3.0.0)
project(Monarch VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(api_to_use OpenGL)
set(platform Mac)

add_subdirectory(ext)
add_subdirectory(src)
