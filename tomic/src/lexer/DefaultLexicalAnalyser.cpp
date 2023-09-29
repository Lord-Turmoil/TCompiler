/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../include/tomic/lexer/impl/DefaultLexicalAnalyser.h"
#include "../../include/tomic/lexer/token/ITokenMapper.h"
#include "../../include/tomic/utils/StringUtil.h"
#include "../../../mioc/include/mioc.h"

#include <cctype>

TOMIC_BEGIN

static const char _DIGITS[] = "0123456789";
static const char _WHITESPACES[] = " \t\r\n\v\f";
static const char _OPERATORS[] = "+-*/%&|!<>=";
static const char _DELIMITERS[] = ",;()[]{}";

DefaultLexicalAnalyser::DefaultLexicalAnalyser()
{
    _InitTasks();
}


DefaultLexicalAnalyser* DefaultLexicalAnalyser::SetReader(twio::IAdvancedReaderPtr reader)
{
    _reader = std::move(reader);
    return this;
}

TokenPtr DefaultLexicalAnalyser::Next()
{
    TokenPtr token;

    // Due to the recovery strategy we use, if nullptr is returned,
    // it means an invalid token is got. So we have to repetitively
    // read tokens, until a valid one is got. The EOF is also a valid
    // token, so we don't need to check it here. :)
    do
    {
        token = _Next();
    } while (!token);

    return token;
}

void DefaultLexicalAnalyser::_InitTasks()
{
    _tasks.emplace_back(std::make_shared<NumberLexicalTask>());
    _tasks.emplace_back(std::make_shared<IdentifierLexicalTask>());
    _tasks.emplace_back(std::make_shared<StringLexicalTask>());
    _tasks.emplace_back(std::make_shared<SingleOpLexicalTask>());
    _tasks.emplace_back(std::make_shared<DoubleOpLexicalTask>());
    _tasks.emplace_back(std::make_shared<DelimiterLexicalTask>());
    _tasks.emplace_back(std::make_shared<UnknownLexicalTask>());
}

TokenPtr DefaultLexicalAnalyser::_Next()
{
    // Read the first character to determine which task to perform.
    int lookahead;

    do
    {
        lookahead = _reader->Read();
    } while (StringUtil::Contains(_WHITESPACES, lookahead));

    // If end of file is reached, return a terminator token.
    if (lookahead == EOF)
    {
        return Token::New(TokenType::TK_TERMINATOR);
    }

    // Find a task to analyse the character.
    TokenPtr token;
    for (auto& task: _tasks)
    {
        if (task->BeginsWith(lookahead))
        {
            // Put lookahead back to the stream.
            _reader->Rewind();

            token = task->Analyse(_reader);
            break;
        }
    }

    if (!token)
    {
        TOMIC_ASSERT(false && "No task is registered to analyse the given character.");
    }

    if (token->type == TokenType::TK_UNKNOWN)
    {
        // TODO: Add error handling.
        TOMIC_ASSERT(false && "Unknown token.");
        return nullptr;
    }

    return token;
}


/*
 * The following classes are the concrete tasks.
 */

//////////////////// Number Lexical Task ////////////////////
bool NumberLexicalTask::BeginsWith(int begin) const
{
    return StringUtil::Contains(_DIGITS, begin);
}

bool NumberLexicalTask::EndsWith(int end) const
{
    // blank, or delimiter, or operator
    return (end == EOF) ||
           StringUtil::Contains(_WHITESPACES, end) ||
           StringUtil::Contains(_DELIMITERS, end) ||
           StringUtil::Contains(_OPERATORS, end);
}

TokenPtr NumberLexicalTask::Analyse(const twio::IAdvancedReaderPtr& reader)
{
    int ch = reader->Read();
    int lineNo = reader->Line();
    int charNo = reader->Char();
    std::string lexeme;

    while (ch != EOF && StringUtil::Contains(_DIGITS, ch))
    {
        lexeme += ch;
        ch = reader->Read();
    }

    if (!EndsWith(ch))
    {
        while (!EndsWith(ch))
        {
            lexeme += ch;
            ch = reader->Read();
        }
        if (ch != EOF)
        {
            reader->Rewind();
        }

        // TODO: Error handling.
        return Token::New(TokenType::TK_UNKNOWN, lexeme, lineNo, charNo);
    }

    if (ch != EOF)
    {
        reader->Rewind();
    }

    return Token::New(TokenType::TK_NUMBER, lexeme, lineNo, charNo);
}


//////////////////// Identifier Lexical Task ////////////////////
bool IdentifierLexicalTask::BeginsWith(int begin) const
{
    return std::isalpha(begin) || begin == '_';
}

bool IdentifierLexicalTask::EndsWith(int end) const
{
    // blank, or delimiter, or operator
    return (end == EOF) ||
           StringUtil::Contains(_WHITESPACES, end) ||
           StringUtil::Contains(_DELIMITERS, end) ||
           StringUtil::Contains(_OPERATORS, end);
}

TokenPtr IdentifierLexicalTask::Analyse(const twio::IAdvancedReaderPtr& reader)
{
    int ch = reader->Read();
    int lineNo = reader->Line();
    int charNo = reader->Char();
    std::string lexeme;

    // The first one must be a letter or underscore.
    // So we don't need to check the end condition.
    while (ch != EOF && (std::isalnum(ch) || ch == '_'))
    {
        lexeme += ch;
        ch = reader->Read();
    }

    if (!EndsWith(ch))
    {
        while (!EndsWith(ch))
        {
            lexeme += ch;
            ch = reader->Read();
        }
        if (ch != EOF)
        {
            reader->Rewind();
        }

        return Token::New(TokenType::TK_UNKNOWN, lexeme, lineNo, charNo);
    }

    if (ch != EOF)
    {
        reader->Rewind();
    }

    auto mapper = mioc::SingletonContainer::GetContainer()->Resolve<ITokenMapper>();
    auto token = Token::New(mapper->Type(lexeme), lexeme, lineNo, charNo);
    if (token->type == TokenType::TK_UNKNOWN)
    {
        token->type = TokenType::TK_IDENTIFIER;
    }
    return token;
}


//////////////////// String Lexical Task ////////////////////
bool StringLexicalTask::BeginsWith(int begin) const
{
    return begin == '"';
}

bool StringLexicalTask::EndsWith(int end) const
{
    return end == '"';
}

TokenPtr StringLexicalTask::Analyse(const twio::IAdvancedReaderPtr& reader)
{
    int ch = reader->Read();
    int lineNo = reader->Line();
    int charNo = reader->Char();
    std::string lexeme;
    bool error = false;

    // The first one must be a double quote.
    lexeme += ch;
    ch = reader->Read();
    while (ch != EOF && ch != '"')
    {
        if (_IsNormalChar(ch))
        {
            lexeme += ch;
        }
        else if (_IsNewLineChar(ch, reader))
        {
            lexeme += ch;
            lexeme += reader->Read();
        }
        else if (_IsFormatChar(ch, reader))
        {
            lexeme += ch;
            lexeme += reader->Read();
        }
        else
        {
            // TODO: Mark Error
            error = true;
        }

        ch = reader->Read();
    }

    if (ch != EOF)
    {
        lexeme += ch;
    }
    else
    {
        // TODO: Unfinished string.
        error = true;
    }

    if (error)
    {
        // TODO: Report error
        return Token::New(TokenType::TK_UNKNOWN, lexeme, lineNo, charNo);
    }

    return Token::New(TokenType::TK_FORMAT, lexeme, lineNo, charNo);
}

bool StringLexicalTask::_IsNormalChar(int ch) const
{
    return (ch == 32) || (ch == 33) || (ch >= 40 && ch <= 126);
}

bool StringLexicalTask::_IsNewLineChar(int ch, const twio::IAdvancedReaderPtr& reader) const
{
    bool ret = false;

    if (ch == '\\')
    {
        ch = reader->Read();
        if (ch == 'n')
        {
            ret = true;
        }

        if (ch != EOF)
        {
            reader->Rewind();
        }
    }

    return ret;
}

bool StringLexicalTask::_IsFormatChar(int ch, const twio::IAdvancedReaderPtr& reader) const
{
    bool ret = false;

    if (ch == '%')
    {
        ch = reader->Read();
        if (ch == 'd')
        {
            ret = true;
        }

        if (ch != EOF)
        {
            reader->Rewind();
        }
    }

    return ret;
}


//////////////////// Single Op Lexical Task ////////////////////
bool SingleOpLexicalTask::BeginsWith(int begin) const
{
    return StringUtil::Contains("+-*/%", begin);
}

bool SingleOpLexicalTask::EndsWith(int end) const
{
    return true;
}

TokenPtr SingleOpLexicalTask::Analyse(const twio::IAdvancedReaderPtr& reader)
{
    int ch = reader->Read();
    int lineNo = reader->Line();
    int charNo = reader->Char();
    std::string lexeme;

    // The first one must be a single-character operator.
    lexeme += ch;

    auto mapper = mioc::SingletonContainer::GetContainer()->Resolve<ITokenMapper>();
    return Token::New(mapper->Type(lexeme), lexeme, lineNo, charNo);
}


//////////////////// Double Op Lexical Task ////////////////////
bool DoubleOpLexicalTask::BeginsWith(int begin) const
{
    return StringUtil::Contains("&|=<>!", begin);
}

bool DoubleOpLexicalTask::EndsWith(int end) const
{
    return true;
}

TokenPtr DoubleOpLexicalTask::Analyse(const twio::IAdvancedReaderPtr& reader)
{
    int ch = reader->Read();
    int next;
    int lineNo = reader->Line();
    int charNo = reader->Char();
    std::string lexeme;

    // The first one must be a double-character operator.
    lexeme += ch;
    switch (ch)
    {
    case '&':
    case '|':
    case '=':
        next = reader->Read();
        if (next == ch)
        {
            lexeme += next;
        }
        else if (next != EOF)
        {
            reader->Rewind();
        }
        break;
    case '<':
    case '>':
        next = reader->Read();
        if (next == '=')
        {
            lexeme += next;
        }
        else if (next != EOF)
        {
            reader->Rewind();
        }
        break;
    case '!':
        next = reader->Read();
        if (next == '=')
        {
            lexeme += next;
        }
        else if (next != EOF)
        {
            reader->Rewind();
        }
        break;
    default:
        TOMIC_ASSERT(false && "Unknown double-character operator.");
    }

    auto mapper = mioc::SingletonContainer::GetContainer()->Resolve<ITokenMapper>();
    return Token::New(mapper->Type(lexeme), lexeme, lineNo, charNo);
}


//////////////////// Delimiter Lexical Task ////////////////////
bool DelimiterLexicalTask::BeginsWith(int begin) const
{
    return StringUtil::Contains(_DELIMITERS, begin);
}

bool DelimiterLexicalTask::EndsWith(int end) const
{
    return true;
}

TokenPtr DelimiterLexicalTask::Analyse(const twio::IAdvancedReaderPtr& reader)
{
    int ch = reader->Read();
    int lineNo = reader->Line();
    int charNo = reader->Char();
    std::string lexeme;

    // The first one must be a delimiter.
    lexeme += ch;

    auto mapper = mioc::SingletonContainer::GetContainer()->Resolve<ITokenMapper>();
    return Token::New(mapper->Type(lexeme), lexeme, lineNo, charNo);
}


//////////////////// Unknown Lexical Task ////////////////////
bool UnknownLexicalTask::BeginsWith(int begin) const
{
    return true;
}

bool UnknownLexicalTask::EndsWith(int end) const
{
    return true;
}

TokenPtr UnknownLexicalTask::Analyse(const twio::IAdvancedReaderPtr& reader)
{
    int ch = reader->Read();
    int lineNo = reader->Line();
    int charNo = reader->Char();
    std::string lexeme;

    lexeme += ch;

    return Token::New(TokenType::TK_UNKNOWN, lexeme, lineNo, charNo);
}

TOMIC_END
