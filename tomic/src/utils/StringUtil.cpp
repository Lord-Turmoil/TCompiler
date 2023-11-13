/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/utils/StringUtil.h>
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

bool IsNullOrEmpty(const char* str)
{
    return !str || !*str;
}

bool BeginsWith(const char* str, const char* prefix)
{
    if (IsNullOrEmpty(prefix))
    {
        return true;
    }
    if (IsNullOrEmpty(str))
    {
        return false;
    }

    const char* original_str = str;
    while (*prefix && *str)
    {
        if (*prefix != *str)
        {
            return false;
        }
        prefix++;
        str++;
    }

    return !*prefix;
}

bool EndsWith(const char* str, const char* suffix)
{
    if (IsNullOrEmpty(suffix))
    {
        return true;
    }
    if (IsNullOrEmpty(str))
    {
        return false;
    }

    const char* str_end = str + strlen(str);
    const char* suffix_end = suffix + strlen(suffix);

    while (str_end >= str && suffix_end >= suffix)
    {
        if (*str_end != *suffix_end)
        {
            return false;
        }
        str_end--;
        suffix_end--;
    }

    return suffix_end < suffix;
}

bool ToInt(const char* str, int* value)
{
    if (str == nullptr)
    {
        return false;
    }

    if (sscanf(str, "%d", value) == 1)
    {
        return true;
    }

    return false;
}

bool ToBool(const char* str, bool* value)
{
    if (str == nullptr)
    {
        return false;
    }

    int intVal = 0;
    if (ToInt(str, &intVal))
    {
        *value = intVal != 0;
        return true;
    }

    static const char* TRUE_VALS[] = { "true", "True", "TRUE", nullptr };
    static const char* FALSE_VALS[] = { "false", "False", "FALSE", nullptr };

    for (int i = 0; TRUE_VALS[i]; ++i)
    {
        if (Equals(str, TRUE_VALS[i]))
        {
            *value = true;
            return true;
        }
    }
    for (int i = 0; FALSE_VALS[i]; ++i)
    {
        if (Equals(str, FALSE_VALS[i]))
        {
            *value = false;
            return true;
        }
    }

    return false;
}

const char* IntToString(int value)
{
    static char buf[64];
    sprintf(buf, "%d", value);
    return buf;
}

const char* BoolToString(bool value)
{
    return value ? "true" : "false";
}

}

TOMIC_END
