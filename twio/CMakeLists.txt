# Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.
# CMakeLists for Tiny Wrapped Input/Output.

# Add all source files
file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS src/*.cpp)

# Add source to this project's executable.
add_library(twio STATIC ${SRC_LIST})

target_include_directories(twio PUBLIC include)
target_compile_definitions(twio PUBLIC TWIO_ENABLE_ASSERT)
