/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC__TOKEN_MAPPER_H_
#define _TOMIC__TOKEN_MAPPER_H_

#include "../../Common.h"
#include "Token.h"

TOMIC_BEGIN

// A utility class to map token kind to string and vice versa.
// Only support tokens with fixed lexeme. (That is, except for identifiers and
// format strings.
class ITokenMapper
{
public:
    virtual ~ITokenMapper() = default;

    // Only support token with fixed lexeme, or externally added lexeme.
    virtual TokenType Type(const std::string& lexeme) const = 0;
    // Only support type with fixed lexeme! Externally added lexeme.
    virtual const char* Lexeme(TokenType type) const = 0;

    virtual const char* Description(TokenType type) const = 0;

    virtual bool IsKeyword(const std::string& lexeme) const = 0;
};

TOMIC_END

#endif
