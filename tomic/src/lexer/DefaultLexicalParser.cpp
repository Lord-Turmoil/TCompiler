/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../include/tomic/lexer/impl/DefaultLexicalParser.h"

TOMIC_BEGIN

DefaultLexicalParser::DefaultLexicalParser(ILexicalAnalyserPtr analyser)
        : _analyser(analyser)
{
    _current = _tokens.end();
}

DefaultLexicalParser* DefaultLexicalParser::SetReader(twio::IAdvancedReaderPtr reader)
{
    _analyser->SetReader(reader);
    return this;
}

TokenPtr DefaultLexicalParser::Next()
{
    if (_current == _tokens.end())
    {
        TokenPtr token = _analyser->Next();
        // Filter out unknown tokens.
        while (token->type != TokenType::TK_TERMINATOR)
        {
            if (token->type != TokenType::TK_UNKNOWN)
            {
                break;
            }

            // TODO: Error report
            token = _analyser->Next();
        }

        // EOF reached.
        if (token->type == TokenType::TK_TERMINATOR)
        {
            // _current is still _tokens.end()
            return nullptr;
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

TOMIC_END
