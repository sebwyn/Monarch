cmake_minimum_required(VERSION 3.0.0)

if(${api_to_use} MATCHES OpenGL)
    add_subdirectory(freetype-gl)
endif()
