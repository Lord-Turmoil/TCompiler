/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_ERROR_TYPE_H_
#define _TOMIC_ERROR_TYPE_H_

#include <tomic/Shared.h>

TOMIC_BEGIN

enum class ErrorType
{
    ERR_UNKNOWN,

    ERR_UNEXPECTED_TOKEN,

    ERR_REDEFINED_SYMBOL,
    ERR_UNDEFINED_SYMBOL,

    ERR_ARGUMENT_COUNT_MISMATCH,
    ERR_ARGUMENT_TYPE_MISMATCH,

    ERR_RETURN_TYPE_MISMATCH,
    ERR_MISSING_RETURN_STATEMENT,

    ERR_ASSIGN_TO_CONST,

    ERR_MISSING_SEMICOLON,
    ERR_MISSING_RIGHT_PARENTHESIS,
    ERR_MISSING_RIGHT_BRACKET,
    ERR_MISSING_RIGHT_BRACE,

    ERR_PRINTF_EXTRA_ARGUMENTS,

    ERR_ILLEGAL_BREAK,
    ERR_ILLEGAL_CONTINUE,

    ERR_COUNT
};


TOMIC_END

#endif // _TOMIC_ERROR_TYPE_H_
