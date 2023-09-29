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
    return (ch != EOF) && (strchr(str, ch) != nullptr);
}

}

TOMIC_END
