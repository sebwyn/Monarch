cmake_minimum_required(VERSION 3.0.0)

add_subdirectory(ext/freetype-gl)
find_package(Freetype REQUIRED)

#graphics_api specific
find_library(glfw_path glfw)
find_library(glew_path glew)
find_package(OpenGL)

link_libraries(${glfw_path} ${glew_path} ${OPENGL_gl_LIBRARY} freetype-gl ${FREETYPE_LIBRARIES})

file(GLOB src_files *.cpp ${platform}/*.cpp)
add_library(Graphics ${src_files}) 
target_include_directories(Graphics PUBLIC include)  
