/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/lexer/impl/DefaultLexicalParser.h>

TOMIC_BEGIN

DefaultLexicalParser::DefaultLexicalParser(ILexicalAnalyzerPtr analyzer, IErrorLoggerPtr errorLogger, ILoggerPtr logger)
    : _analyzer(analyzer), _errorLogger(errorLogger), _logger(logger)
{
    TOMIC_ASSERT(_analyzer);
    TOMIC_ASSERT(_errorLogger);
    TOMIC_ASSERT(_logger);

    _current = _tokens.end();
}


DefaultLexicalParser* DefaultLexicalParser::SetReader(twio::IAdvancedReaderPtr reader)
{
    _analyzer->SetReader(reader);
    return this;
}


TokenPtr DefaultLexicalParser::Current()
{
    if (_current == _tokens.begin())
    {
        return nullptr;
    }
    return *(_current - 1);
}


TokenPtr DefaultLexicalParser::Next()
{
    if (_current == _tokens.end())
    {
        TokenPtr token = _analyzer->Next();
        while (token->type == TokenType::TK_UNKNOWN)
        {
            _RaiseUnexpectedTokenError(token);
            _LogUnexpectedToken(token);
            token = _analyzer->Next();
        }
        // EOF reached.
        if (token->type == TokenType::TK_TERMINATOR)
        {
            // _current is still _tokens.end()
            return token;
        }

        _tokens.push_back(token);
        _current = _tokens.end() - 1;
    }

    return *(_current++);
}


TokenPtr DefaultLexicalParser::Rewind()
{
    if (_current != _tokens.begin())
    {
        return *(--_current);
    }

    return nullptr;
}


int DefaultLexicalParser::SetCheckPoint()
{
    return _current - _tokens.begin();
}


void DefaultLexicalParser::Rollback(int checkpoint)
{
    _current = _tokens.begin() + checkpoint;
}


void DefaultLexicalParser::_LogUnexpectedToken(TokenPtr token)
{
    TOMIC_ASSERT(token);
    _logger->LogFormat(LogLevel::ERROR, "(%d:%d) Unexpected token %s",
                       token->lineNo,
                       token->charNo,
                       token->lexeme.c_str());
}


void DefaultLexicalParser::_RaiseUnexpectedTokenError(TokenPtr token)
{
    TOMIC_ASSERT(token);
    _errorLogger->LogFormat(
        token->lineNo,
        token->charNo,
        ErrorType::ERR_UNEXPECTED_TOKEN,
        "Unexpected token %s",
        token->lexeme.c_str());
}


TOMIC_END
