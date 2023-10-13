/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/logger/error/impl/StandardErrorMapper.h>

TOMIC_BEGIN

StandardErrorMapper::StandardErrorMapper()
{
    _descriptions[static_cast<int>(ErrorType::ERR_UNKNOWN)] = "Unknown error";

    _descriptions[static_cast<int>(ErrorType::ERR_UNEXPECTED_TOKEN)] = "a";

    _descriptions[static_cast<int>(ErrorType::ERR_REDEFINED_SYMBOL)] = "b";
    _descriptions[static_cast<int>(ErrorType::ERR_UNDEFINED_SYMBOL)] = "c";

    _descriptions[static_cast<int>(ErrorType::ERR_ARGUMENT_COUNT_MISMATCH)] = "d";
    _descriptions[static_cast<int>(ErrorType::ERR_ARGUMENT_TYPE_MISMATCH)] = "e";

    _descriptions[static_cast<int>(ErrorType::ERR_RETURN_TYPE_MISMATCH)] = "f";
    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_RETURN_STATEMENT)] = "g";

    _descriptions[static_cast<int>(ErrorType::ERR_ASSIGN_TO_CONST)] = "h";

    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_SEMICOLON)] = "h";
    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_RIGHT_PARENTHESIS)] = "i";
    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_RIGHT_BRACKET)] = "j";
    _descriptions[static_cast<int>(ErrorType::ERR_MISSING_RIGHT_BRACE)] = "z";

    _descriptions[static_cast<int>(ErrorType::ERR_PRINTF_EXTRA_ARGUMENTS)] = "k";

    _descriptions[static_cast<int>(ErrorType::ERR_ILLEGAL_BREAK)] = "l";
    _descriptions[static_cast<int>(ErrorType::ERR_ILLEGAL_CONTINUE)] = "m";
}

const char* StandardErrorMapper::Description(ErrorType type)
{
    return _descriptions[static_cast<int>(type)];
}

TOMIC_END
