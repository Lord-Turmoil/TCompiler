/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_TOKEN_MAPPER_H_
#define _TOMIC_DEFAULT_TOKEN_MAPPER_H_

#include <tomic/Shared.h>
#include <tomic/lexer/token/ITokenMapper.h>
#include <unordered_map>
#include <string>

TOMIC_BEGIN

class DefaultTokenMapper : public ITokenMapper
{
public:
    DefaultTokenMapper();
    ~DefaultTokenMapper() override = default;

    DefaultTokenMapper(const DefaultTokenMapper&) = delete;
    DefaultTokenMapper& operator=(const DefaultTokenMapper&) = delete;
    DefaultTokenMapper(DefaultTokenMapper&&) = delete;
    DefaultTokenMapper& operator=(DefaultTokenMapper&&) = delete;

public:
    TokenType Type(const std::string& lexeme) const override;
    const char* Lexeme(TokenType type) const override;

    const char* Description(TokenType type) const override;

    bool IsKeyword(const std::string& lexeme) const override;

private:
    void _Init();
    void _InitLexemeToType();
    void _InitTypeToLexeme();
    void _InitTypeToDescription();
    void _InitLexemeToIsKeyword();

    std::unordered_map<std::string, TokenType> _lexemeToType;
    std::unordered_map<TokenType, std::string> _typeToLexeme;
    std::unordered_map<TokenType, std::string> _typeToDescription;
    std::unordered_map<std::string, bool> _lexemeToIsKeyword;
};


TOMIC_END

#endif // _TOMIC_DEFAULT_TOKEN_MAPPER_H_
