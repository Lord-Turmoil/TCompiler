cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

project(ToMiCompiler)

# source files
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS tomic/include/*.h tomic/src/*.cpp include/*.h src/*.cpp)

# final target
add_executable(${CMAKE_PROJECT_NAME} ${SRC_LIST})

# target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE tomic/include)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE TOMIC_ENABLE_ASSERT)

# IoC dependency
add_subdirectory(mioc)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE mioc)

# Twio Dependency
add_subdirectory(twio)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC twio)
