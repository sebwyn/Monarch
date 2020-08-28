cmake_minimum_required(VERSION 3.0.0)
project(Monarch VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(glfw_path glfw)
find_library(glew_path glew)
find_package(OpenGL)

add_subdirectory(external/freetype-gl)

find_package(Freetype REQUIRED)

set(libs ${glfw_path} ${glew_path} ${OPENGL_gl_LIBRARY} freetype-gl ${FREETYPE_LIBRARIES})

#get and build sources
file(GLOB src_files src/*.cpp src/Components/*.cpp src/platforms/${api_to_use}/*.cpp src/platforms/${api_to_use}/${platform}/*.cpp src/components/gui/*.cpp)
message(${src_files})

add_library(MonarchRelease ${src_files})
target_link_libraries(MonarchRelease ${libs})
target_include_directories(MonarchRelease PRIVATE ${includes})
set_target_properties(MonarchRelease PROPERTIES OUTPUT_NAME "Monarch")

add_library(MonarchDebug ${src_files})
target_link_libraries(MonarchDebug ${libs})
target_include_directories(MonarchDebug PRIVATE ${includes})
set_target_properties(MonarchDebug PROPERTIES OUTPUT_NAME "Monarch_d" COMPILE_FLAGS " -g -D DEBUG_BUILD")