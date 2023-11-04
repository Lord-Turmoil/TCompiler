# Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.
# CMakeLists for MinIoc.

# Add all source files
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS src/*.cpp)

# Add source to this project's executable.
add_library(tomic STATIC ${SRC_LIST})

target_include_directories(tomic PUBLIC include)
target_compile_definitions(tomic PUBLIC TOMIC_ENABLE_ASSERT)

# IoC dependency
add_subdirectory(3rd-party/mioc)
target_link_libraries(tomic PUBLIC mioc)

# Twio Dependency
add_subdirectory(3rd-party/twio)
target_link_libraries(tomic PUBLIC twio)