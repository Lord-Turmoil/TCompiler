/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFULAT_LEXICAL_PARSER_H_
#define _TOMIC_DEFULAT_LEXICAL_PARSER_H_

#include <tomic/Shared.h>
#include <tomic/lexer/ILexicalParser.h>
#include <tomic/lexer/ILexicalAnalyzer.h>
#include <tomic/lexer/token/ITokenMapper.h>
#include <tomic/logger/debug/ILogger.h>
#include <tomic/logger/error/IErrorLogger.h>
#include <vector>

TOMIC_BEGIN

class DefaultLexicalParser : public ILexicalParser
{
public:
    DefaultLexicalParser(ILexicalAnalyzerPtr analyzer, IErrorLoggerPtr errorLogger, ILoggerPtr logger);
    ~DefaultLexicalParser() override = default;

    DefaultLexicalParser* SetReader(twio::IAdvancedReaderPtr reader) override;

    TokenPtr Current() override;
    TokenPtr Next() override;
    TokenPtr Rewind() override;

    int SetCheckPoint() override;
    void Rollback(int checkpoint) override;

private:
    void _LogUnexpectedToken(TokenPtr token);
    void _RaiseUnexpectedTokenError(TokenPtr token);

private:
    ILexicalAnalyzerPtr _analyzer;
    IErrorLoggerPtr _errorLogger;
    ILoggerPtr _logger;

    std::vector<TokenPtr> _tokens;
    std::vector<TokenPtr>::iterator _current;
};


TOMIC_END

#endif // _TOMIC_DEFULAT_LEXICAL_PARSER_H_
