/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/lexer/impl/token/DefaultTokenMapper.h>

TOMIC_BEGIN

DefaultTokenMapper::DefaultTokenMapper()
{
    _Init();
}

TokenType DefaultTokenMapper::Type(const std::string& lexeme) const
{
    auto iter = _lexemeToType.find(lexeme);
    if (iter == _lexemeToType.end())
    {
        return TokenType::TK_UNKNOWN;
    }
    return iter->second;
}

const char* DefaultTokenMapper::Lexeme(TokenType type) const
{
    auto iter = _typeToLexeme.find(type);
    if (iter == _typeToLexeme.end())
    {
        return nullptr;
    }
    return iter->second.c_str();
}

const char* DefaultTokenMapper::Description(TokenType type) const
{
    auto iter = _typeToDescription.find(type);
    if (iter == _typeToDescription.end())
    {
        return nullptr;
    }
    return iter->second.c_str();
}

bool DefaultTokenMapper::IsKeyword(const std::string& lexeme) const
{
    auto iter = _lexemeToIsKeyword.find(lexeme);
    if (iter == _lexemeToIsKeyword.end())
    {
        return false;
    }
    return iter->second;
}

void DefaultTokenMapper::_Init()
{
    _InitLexemeToType();
    _InitTypeToLexeme();
    _InitTypeToDescription();
    _InitLexemeToIsKeyword();
}

void DefaultTokenMapper::_InitLexemeToType()
{
    _lexemeToType["main"] = TokenType::TK_MAIN;
    _lexemeToType["return"] = TokenType::TK_RETURN;
    _lexemeToType["getint"] = TokenType::TK_GETINT;
    _lexemeToType["printf"] = TokenType::TK_PRINTF;

    _lexemeToType["if"] = TokenType::TK_IF;
    _lexemeToType["else"] = TokenType::TK_ELSE;

    _lexemeToType["for"] = TokenType::TK_FOR;
    _lexemeToType["break"] = TokenType::TK_BREAK;
    _lexemeToType["continue"] = TokenType::TK_CONTINUE;

    _lexemeToType["const"] = TokenType::TK_CONST;
    _lexemeToType["int"] = TokenType::TK_INT;
    _lexemeToType["void"] = TokenType::TK_VOID;

    _lexemeToType["!"] = TokenType::TK_NOT;
    _lexemeToType["&&"] = TokenType::TK_AND;
    _lexemeToType["||"] = TokenType::TK_OR;
    _lexemeToType["+"] = TokenType::TK_PLUS;
    _lexemeToType["-"] = TokenType::TK_MINUS;
    _lexemeToType["*"] = TokenType::TK_MULTIPLY;
    _lexemeToType["/"] = TokenType::TK_DIVIDE;
    _lexemeToType["%"] = TokenType::TK_MOD;
    _lexemeToType["<"] = TokenType::TK_LESS;
    _lexemeToType["<="] = TokenType::TK_LESS_EQUAL;
    _lexemeToType[">"] = TokenType::TK_GREATER;
    _lexemeToType[">="] = TokenType::TK_GREATER_EQUAL;
    _lexemeToType["=="] = TokenType::TK_EQUAL;
    _lexemeToType["!="] = TokenType::TK_NOT_EQUAL;
    _lexemeToType["="] = TokenType::TK_ASSIGN;

    _lexemeToType[";"] = TokenType::TK_SEMICOLON;
    _lexemeToType[","] = TokenType::TK_COMMA;

    _lexemeToType["("] = TokenType::TK_LEFT_PARENTHESIS;
    _lexemeToType[")"] = TokenType::TK_RIGHT_PARENTHESIS;
    _lexemeToType["{"] = TokenType::TK_LEFT_BRACE;
    _lexemeToType["}"] = TokenType::TK_RIGHT_BRACE;
    _lexemeToType["["] = TokenType::TK_LEFT_BRACKET;
    _lexemeToType["]"] = TokenType::TK_RIGHT_BRACKET;
}

void DefaultTokenMapper::_InitTypeToLexeme()
{
    for (auto& pair: _lexemeToType)
    {
        _typeToLexeme[pair.second] = pair.first;
    }
}

void DefaultTokenMapper::_InitTypeToDescription()
{
    _typeToDescription[TokenType::TK_UNKNOWN] = "UNKNOWN";
    _typeToDescription[TokenType::TK_IDENTIFIER] = "IDENFR";
    _typeToDescription[TokenType::TK_INTEGER] = "INTCON";
    _typeToDescription[TokenType::TK_FORMAT] = "STRCON";
    _typeToDescription[TokenType::TK_MAIN] = "MAINTK";
    _typeToDescription[TokenType::TK_RETURN] = "RETURNTK";
    _typeToDescription[TokenType::TK_GETINT] = "GETINTTK";
    _typeToDescription[TokenType::TK_PRINTF] = "PRINTFTK";

    _typeToDescription[TokenType::TK_IF] = "IFTK";
    _typeToDescription[TokenType::TK_ELSE] = "ELSETK";

    _typeToDescription[TokenType::TK_FOR] = "FORTK";
    _typeToDescription[TokenType::TK_BREAK] = "BREAKTK";
    _typeToDescription[TokenType::TK_CONTINUE] = "CONTINUETK";

    _typeToDescription[TokenType::TK_CONST] = "CONSTTK";
    _typeToDescription[TokenType::TK_INT] = "INTTK";
    _typeToDescription[TokenType::TK_VOID] = "VOIDTK";

    _typeToDescription[TokenType::TK_NOT] = "NOT";
    _typeToDescription[TokenType::TK_AND] = "AND";
    _typeToDescription[TokenType::TK_OR] = "OR";
    _typeToDescription[TokenType::TK_PLUS] = "PLUS";
    _typeToDescription[TokenType::TK_MINUS] = "MINU";
    _typeToDescription[TokenType::TK_MULTIPLY] = "MULT";
    _typeToDescription[TokenType::TK_DIVIDE] = "DIV";
    _typeToDescription[TokenType::TK_MOD] = "MOD";
    _typeToDescription[TokenType::TK_LESS] = "LSS";
    _typeToDescription[TokenType::TK_LESS_EQUAL] = "LEQ";
    _typeToDescription[TokenType::TK_GREATER] = "GRE";
    _typeToDescription[TokenType::TK_GREATER_EQUAL] = "GEQ";
    _typeToDescription[TokenType::TK_EQUAL] = "EQL";
    _typeToDescription[TokenType::TK_NOT_EQUAL] = "NEQ";
    _typeToDescription[TokenType::TK_ASSIGN] = "ASSIGN";

    _typeToDescription[TokenType::TK_SEMICOLON] = "SEMICN";
    _typeToDescription[TokenType::TK_COMMA] = "COMMA";

    _typeToDescription[TokenType::TK_LEFT_PARENTHESIS] = "LPARENT";
    _typeToDescription[TokenType::TK_RIGHT_PARENTHESIS] = "RPARENT";
    _typeToDescription[TokenType::TK_LEFT_BRACE] = "LBRACE";
    _typeToDescription[TokenType::TK_RIGHT_BRACE] = "RBRACE";
    _typeToDescription[TokenType::TK_LEFT_BRACKET] = "LBRACK";
    _typeToDescription[TokenType::TK_RIGHT_BRACKET] = "RBRACK";
};

void DefaultTokenMapper::_InitLexemeToIsKeyword()
{
    _lexemeToIsKeyword["main"] = true;
    _lexemeToIsKeyword["return"] = true;
    _lexemeToIsKeyword["getint"] = true;
    _lexemeToIsKeyword["printf"] = true;

    _lexemeToIsKeyword["if"] = true;
    _lexemeToIsKeyword["else"] = true;

    _lexemeToIsKeyword["for"] = true;
    _lexemeToIsKeyword["break"] = true;
    _lexemeToIsKeyword["continue"] = true;

    _lexemeToIsKeyword["const"] = true;
    _lexemeToIsKeyword["int"] = true;
    _lexemeToIsKeyword["void"] = true;
}

TOMIC_END
