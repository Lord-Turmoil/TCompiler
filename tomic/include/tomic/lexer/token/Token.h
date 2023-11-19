/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_TOKEN_H_
#define _TOMIC_TOKEN_H_

#include <memory>
#include <string>
#include <tomic/Shared.h>

TOMIC_BEGIN

enum class TokenType
{
    TK_UNKNOWN,        // unknown token
    TK_TERMINATOR,     // terminator

    TK_IDENTIFIER,     // identifier
    TK_INTEGER,         // integer constant
    TK_FORMAT,         // format string, e.g. "a = %d"
    TK_MAIN,           // main
    TK_RETURN,         // return
    TK_GETINT,         // getint
    TK_PRINTF,         // printf

    TK_IF,             // if
    TK_ELSE,           // else

    TK_FOR,            // for
    TK_BREAK,          // break
    TK_CONTINUE,       // continue

    TK_CONST,          // const
    TK_INT,            // int
    TK_VOID,           // void

    TK_NOT,            // !
    TK_AND,            // &&
    TK_OR,             // ||
    TK_PLUS,           // +
    TK_MINUS,          // -
    TK_MULTIPLY,       // *
    TK_DIVIDE,         // /
    TK_MOD,            // %
    TK_LESS,           // <
    TK_LESS_EQUAL,     // <=
    TK_GREATER,        // >
    TK_GREATER_EQUAL,  // >=
    TK_EQUAL,          // ==
    TK_NOT_EQUAL,      // !=
    TK_ASSIGN,         // =

    TK_SEMICOLON,      // ;
    TK_COMMA,          // ,

    TK_LEFT_PARENTHESIS,    // (
    TK_RIGHT_PARENTHESIS,   // )
    TK_LEFT_BRACE,          // {
    TK_RIGHT_BRACE,         // }
    TK_LEFT_BRACKET,        // [
    TK_RIGHT_BRACKET,       // ]
};


// For now, memory layout is not considered.
struct Token
{
    TokenType type;     // The type of the token.

    std::string lexeme; // The actual string that is read from the source code.

    int lineNo; // The line number of the token.
    int charNo; // The character number of the token.

    Token(TokenType type, std::string lexeme, int lineNo = 0, int charNo = 0)
        : type(type), lexeme(lexeme), lineNo(lineNo), charNo(charNo)
    {
    }


    static std::shared_ptr<Token> New(TokenType type, std::string lexeme, int lineNo = 0, int charNo = 0)
    {
        return std::make_shared<Token>(type, std::move(lexeme), lineNo, charNo);
    }


    static std::shared_ptr<Token> New(TokenType type)
    {
        return std::make_shared<Token>(type, "", 0, 0);
    }


    static TokenType Type(const std::shared_ptr<Token>& token)
    {
        return token ? token->type : TokenType::TK_UNKNOWN;
    }
};


using TokenPtr = std::shared_ptr<Token>;

// For example, the Token for 'int' can be:
// { TK_INT, "int", 1, 1 }
// The Token for 'number' can be:
// { TK_IDENT, "number", 3, 5 }


TOMIC_END

#endif // _TOMIC_TOKEN_H_
