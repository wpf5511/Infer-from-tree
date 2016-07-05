cmake_minimum_required(VERSION 3.3)
project(Infer_from_tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp ZparTree.cpp TemplateTree.cpp)
add_executable(Infer_from_tree ${SOURCE_FILES})