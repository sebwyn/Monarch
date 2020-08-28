cmake_minimum_required(VERSION 3.1.0)

add_subdirectory(platforms/${api_to_use})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#get and build sources
file(GLOB src_files *.cpp components/*.cpp components/2d/*.cpp components/3d/*.cpp components/gui/*.cpp)
set(src_files ${src_files} ${graphics_src})
message(${src_files})

link_libraries(${graphics_libs})
message(${graphics_includes})

add_library(MonarchRelease ${src_files})
target_include_directories(MonarchRelease PUBLIC include ${graphics_includes})
set_target_properties(MonarchRelease PROPERTIES OUTPUT_NAME "Monarch")

add_library(MonarchDebug ${src_files})
target_include_directories(MonarchDebug PUBLIC include ${graphics_includes})
set_target_properties(MonarchDebug PROPERTIES OUTPUT_NAME "Monarch_d" COMPILE_FLAGS " -g -D DEBUG_BUILD")
