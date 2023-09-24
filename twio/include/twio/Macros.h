// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_MACROS_H_
#define _TWIO_MACROS_H_

#define TWIO_VERSION   "1.0.0"
#define TWIO_PROJ_NAME "Tiny Wrapped Input/Output"

#define TWIO_BEGIN namespace twio {
#define TWIO_END   }
#define TWIO       ::twio::

#ifdef _WIN32
#define _TWIO_FOR_WIN32 1
#else
#define _TWIO_FOR_WIN32 0
#endif

#ifdef ENABLE_ASSERT
#include <cassert>
#define TWIO_ASSERT(expression) assert(expression)
#define TWIO_PANIC(expression) assert(false && (expression))
#else
#define TWIO_ASSERT(...)
#endif

// Align size to 8 bytes.
#define TWIO_ALIGN_8(size) (((size) + 7) & ~7)
#define TWIO_ALIGN(size, align) (((size) + ((align) - 1)) & ~((align) - 1))

#endif // _TWIO_MACROS_H_
