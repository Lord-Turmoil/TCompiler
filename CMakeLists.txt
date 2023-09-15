cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 11)

project(ToMiCompiler)

aux_source_directory(tomic/src SRC_LIST)

add_executable(${CMAKE_PROJECT_NAME} ${SRC_LIST})
