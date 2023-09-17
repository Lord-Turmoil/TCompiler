# Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.
# CMakeLists for MinIoc.

file(GLOB_RECURSE SRC_LIST CONFIGURE_DEPENDS include/mioc/*.h)
add_library(mioc INTERFACE ${SRC_LIST})
target_include_directories(mioc INTERFACE include)
