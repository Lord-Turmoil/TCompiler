cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 11)

project(ToMiCompiler)

# source files
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS tomic/src/*.cpp)

# final target
add_executable(${CMAKE_PROJECT_NAME} ${SRC_LIST})

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE tomic/include)

# IoC dependency
add_subdirectory(mioc)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE mioc)
