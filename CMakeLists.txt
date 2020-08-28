cmake_minimum_required(VERSION 3.0.0)
project(Monarch VERSION 0.1.0)

set(api_to_use OpenGL)
set(platform Mac)

add_subdirectory(ext)
add_subdirectory(src)
