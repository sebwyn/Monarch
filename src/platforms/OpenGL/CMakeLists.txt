cmake_minimum_required(VERSION 3.0.0)

add_subdirectory(ext/freetype-gl)
find_package(Freetype REQUIRED)

#graphics_api specific
find_library(glfw_path glfw)
find_library(glew_path glew)
find_package(OpenGL)

file(GLOB local_src *.cpp ${platform}/*.cpp PARENT_SCOPE)

set(graphics_libs ${glfw_path} ${glew_path} ${OPENGL_gl_LIBRARY} freetype-gl ${FREETYPE_LIBRARIES} PARENT_SCOPE)
set(graphics_includes ${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/${platform}/include /usr/local/include ${CMAKE_CURRENT_SOURCE_DIR}/ext/freetype-gl PARENT_SCOPE)
set(graphics_src ${local_src} PARENT_SCOPE)
