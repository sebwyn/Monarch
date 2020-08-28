cmake_minimum_required(VERSION 3.0.0)

add_subdirectory(platforms)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#get and build sources
file(GLOB src_files *.cpp components/*.cpp components/2d/*.cpp components/3d/*.cpp components/gui/*.cpp)
message(${src_files})

link_libraries(Graphics)

add_library(MonarchRelease ${src_files})
target_include_directories(MonarchRelease PUBLIC include)
set_target_properties(MonarchRelease PROPERTIES OUTPUT_NAME "Monarch")

add_library(MonarchDebug ${src_files})
target_include_directories(MonarchDebug PUBLIC include)
set_target_properties(MonarchDebug PROPERTIES OUTPUT_NAME "Monarch_d" COMPILE_FLAGS " -g -D DEBUG_BUILD")
