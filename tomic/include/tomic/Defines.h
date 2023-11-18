/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFINES_H_
#define _TOMIC_DEFINES_H_

#define TOMIC_VERSION_MAJOR "0"
#define TOMIC_VERSION_MINOR "4"
#define TOMIC_VERSION_PATCH "1"

#define TOMIC_VERSION (TOMIC_VERSION_MAJOR "." TOMIC_VERSION_MINOR "." TOMIC_VERSION_PATCH)


#define TOMIC_BEGIN namespace tomic {
#define TOMIC_END   }
#define TOMIC       ::tomic::


#ifdef TOMIC_ENABLE_ASSERT

#include <cassert>

#define TOMIC_ASSERT(expression) assert(expression)
#define TOMIC_PANIC(expression) assert(false && (expression))
#else
#define TOMIC_ASSERT(x)
#define TOMIC_PANIC(expression)
#endif

#ifdef _WIN32
#define TOMIC_SSCANF sscanf_s
#define TOMIC_SPRINTF sprintf_s
#define TOMIC_VSPRINTF vsprintf_s
#else
#define TOMIC_SSCANF sscanf
#define TOMIC_SPRINTF sprintf
#define TOMIC_VSPRINTF vsprintf
#endif

#endif
