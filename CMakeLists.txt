cmake_minimum_required(VERSION 3.0)
project(subgraph_matching)

set(PROD ON)

if(PROD)
add_compile_options(-O3)
add_definitions(-DNDEBUG)
else()
add_compile_options(-Og)
add_compile_options(-g)
add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
endif()

add_compile_options(-Wall)
add_compile_options(-std=c++11)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB SOURCES src/*)

add_subdirectory(main)
