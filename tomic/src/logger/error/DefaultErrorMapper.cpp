/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/logger/error/impl/DefaultErrorMapper.h"

TOMIC_BEGIN

DefaultErrorMapper::DefaultErrorMapper()
{
    _descriptions[static_cast<int>(ErrorType::ERR_UNKNOWN)] = "Unknown error";
    _descriptions[static_cast<int>(ErrorType::ERR_UNEXPECTED_TOKEN)] = "Unexpected token";

    _descriptions[static_cast<int>(ErrorType::ERR_REDEFINED_SYMBOL)] = "Redefined symbol";
    _descriptions[static_cast<int>(ErrorType::ERR_UNDEFINED_SYMBOL)] = "Undefined symbol";

    _descriptions[static_cast<int>(ErrorType::ERR_ARGUMENT_COUNT_MISMATCH)] = "Argument count mismatch";
    _descriptions[static_cast<int>(ErrorType::ERR_ARGUMENT_TYPE_MISMATCH)] = "Argument type mismatch";

    _descriptions[static_cast<int>(ErrorType::ERR_RETURN_TYPE_MISMATCH)] = "Return type mismatch";
    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_RETURN_STATEMENT)] = "Missing return statement";

    _descriptions[static_cast<int>(ErrorType::ERR_ASSIGN_TO_CONST)] = "Assign to const";

    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_SEMICOLON)] = "Missing ;";
    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_RIGHT_PARENTHESIS)] = "Missing )";
    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_RIGHT_BRACKET)] = "Missing ]";
    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_RIGHT_BRACE)] = "Missing }";

    _descriptions[static_cast<int>(ErrorType::ERR_PRINTF_EXTRA_ARGUMENTS)] = "Extra arguments for printf";

    _descriptions[static_cast<int>(ErrorType::ERR_ILLEGAL_BREAK)] = "Illegal break";
    _descriptions[static_cast<int>(ErrorType::ERR_ILLEGAL_CONTINUE)] = "Illegal continue";
}

const char* DefaultErrorMapper::Description(ErrorType type)
{
    return _descriptions[static_cast<int>(type)];
}

TOMIC_END
