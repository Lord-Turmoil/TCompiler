/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/Config.h>

TOMIC_BEGIN

Config::Config()
        : Target(TargetType::Initial),
          EnableCompleteAst(false),
          EmitAst(false),
          EnableLog(false),
          EnableError(false),
          EnableVerboseError(false)
{
}

TOMIC_END
