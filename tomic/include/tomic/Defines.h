/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFINES_H_
#define _TOMIC_DEFINES_H_

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


#endif
