/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../include/tomic/utils/StringUtil.h"
#include <cstring>
#include <cstdio>

TOMIC_BEGIN

namespace StringUtil
{

bool Contains(const char* str, int ch)
{
    if (str == nullptr)
    {
        return false;
    }

    return (ch != EOF) && (strchr(str, ch) != nullptr);
}

bool Equals(const char* str1, const char* str2)
{
    // If both are nullptr, they are equal.
    if (str1 == str2)
    {
        return true;
    }

    // If one of them is nullptr, they are not equal.
    if (str1 == nullptr || str2 == nullptr)
    {
        return false;
    }

    return strcmp(str1, str2) == 0;
}

}

TOMIC_END
