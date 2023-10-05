/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_STRING_UTIL_H_
#define _TOMIC_STRING_UTIL_H_

#include "../Common.h"

TOMIC_BEGIN

namespace StringUtil
{
bool Contains(const char* str, int ch);
bool Equals(const char* str1, const char* str2);
}

TOMIC_END

#endif