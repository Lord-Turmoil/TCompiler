/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../include/tomic/Defines.h"
#include "../../include/tomic/lexer/DefaultPreprocessor.h"

TOMIC_BEGIN

DefaultPreprocessor::DefaultPreprocessor() : _state({StateType::ANY, 0})
{
}

void DefaultPreprocessor::Process()
{
    TOMIC_ASSERT(_reader);
    TOMIC_ASSERT(_writer);

    int ch;
    do
    {
        ch = _reader->Read();
        if (ch == '\r')
        {
            // filter unnecessary '\r'
            continue;
        }
        _Process(ch);
    } while (ch != EOF);
}

DefaultPreprocessor* DefaultPreprocessor::SetReader(twio::IReaderPtr&& reader)
{
    _reader = std::move(reader);
    return this;
}

DefaultPreprocessor* DefaultPreprocessor::SetWriter(twio::IWriterPtr&& writer)
{
    _writer = std::move(writer);
    return this;
}


void DefaultPreprocessor::_Process(int ch)
{
    switch (_state.type)
    {
    case StateType::ANY:
        _ProcessAny(ch);
        break;
    case StateType::SLASH:
        _ProcessSlash(ch);
        break;
    case StateType::LINE_COMMENT:
        _ProcessLineComment(ch);
        break;
    case StateType::BLOCK_COMMENT_LEFT:
        _ProcessBlockCommentLeft(ch);
        break;
    case StateType::BLOCK_COMMENT_RIGHT:
        _ProcessBlockCommentRight(ch);
        break;
    case StateType::QUOTE:
        _ProcessQuote(ch);
        break;
    default:
        TOMIC_ASSERT(false && "Unknown state type");
    }
}

void DefaultPreprocessor::_ProcessAny(int ch)
{
    switch (ch)
    {
    case '/':
        _state.type = StateType::SLASH;
        break;
    case '\'':
    case '\"':
        _state.type = StateType::QUOTE;
        _state.value = ch;
        _writer->Write(ch);
        break;
    case EOF:
        break;
    default:
        _writer->Write(ch);
        break;
    }
}

void DefaultPreprocessor::_ProcessSlash(int ch)
{
    switch (ch)
    {
    case '/':
        _state.type = StateType::LINE_COMMENT;
        _writer->Write(FILLING);    // replace previous '/', too
        _writer->Write(FILLING);
        break;
    case '*':
        _state.type = StateType::BLOCK_COMMENT_LEFT;
        _writer->Write(FILLING);    // replace previous '/', too
        _writer->Write(FILLING);
        break;
    case '\'':
    case '\"':
        _state.type = StateType::QUOTE;
        _state.value = ch;
        _writer->Write('/');    // add the previous wrong '/'
        _writer->Write(ch);
        break;
    case EOF:
        // unfinished /
        _writer->Write('/');
        break;
    default:
        _state.type = StateType::ANY;
        _writer->Write('/');
        _writer->Write(ch);
        break;
    }
}

void DefaultPreprocessor::_ProcessLineComment(int ch)
{
    switch (ch)
    {
    case '\n':
        _state.type = StateType::ANY;
        _writer->Write('\n');
        break;
    case EOF:
        break;
    default:
        _writer->Write(FILLING);
        break;
    }
}

void DefaultPreprocessor::_ProcessBlockCommentLeft(int ch)
{
    switch (ch)
    {
    case '*':
        _state.type = StateType::BLOCK_COMMENT_RIGHT;
        break;
    case '\n':
        _writer->Write('\n');
        break;
    case EOF:
        break;
    default:
        _writer->Write(FILLING);
    }
}

void DefaultPreprocessor::_ProcessBlockCommentRight(int ch)
{
    switch (ch)
    {
    case '/':
        _state.type = StateType::ANY;
        _writer->Write(FILLING);    // *
        _writer->Write(FILLING);    // /
        break;
    case '\n':
        _state.type = StateType::BLOCK_COMMENT_LEFT;
        _writer->Write(FILLING);    // wrong *
        _writer->Write('\n');
        break;
    case EOF:
        _writer->Write(FILLING);    // wrong *
        break;
    default:
        _state.type = StateType::BLOCK_COMMENT_LEFT;
        _writer->Write(FILLING);    // wrong *
        _writer->Write(FILLING);
        break;
    }
}

void DefaultPreprocessor::_ProcessQuote(int ch)
{
    if (ch == EOF)
    {
        return;
    }

    switch (ch)
    {
    case '\'':
    case '\"':
        if (ch == _state.value)
        {
            // quote matches
            _state.type = StateType::ANY;
        }
        break;
    default:
        break;
    }

    _writer->Write(ch);
}

TOMIC_END
