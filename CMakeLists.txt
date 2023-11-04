cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)

project(ToMiCompiler)

# source files
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS src/*.cpp)

# final target
add_executable(${CMAKE_PROJECT_NAME} ${SRC_LIST})


# IoC dependency
add_subdirectory(3rd-party/mioc)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE mioc)

# Twio Dependency
add_subdirectory(3rd-party/twio)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE twio)

# ToMiCompiler Core
add_subdirectory(tomic)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE tomic)

set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/output")
install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION bin)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE INTERNAL)
