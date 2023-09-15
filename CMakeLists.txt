cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 11)

project(ToMiCompiler)

# pre-task: Syntax test
add_subdirectory(pre)

# main project
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS tomic/src/*.cpp)

# final target
add_executable(${CMAKE_PROJECT_NAME} ${SRC_LIST})

# include directories
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC mioc/include)  # IoC Lib
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC tomic/include)
