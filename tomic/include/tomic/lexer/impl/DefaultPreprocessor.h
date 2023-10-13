/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_PREPROCESSOR_H_
#define _TOMIC_DEFAULT_PREPROCESSOR_H_

#include <tomic/Shared.h>
#include <tomic/lexer/IPreprocessor.h>

TOMIC_BEGIN

// Read the source code, and replace comments with white space.
class DefaultPreprocessor : public IPreprocessor
{
public:
    DefaultPreprocessor();

    ~DefaultPreprocessor() override = default;

    void Process() override;

    DefaultPreprocessor* SetReader(twio::IReaderPtr reader) override;
    DefaultPreprocessor* SetWriter(twio::IWriterPtr writer) override;

    twio::IReaderPtr GetReader() const override { return _reader; }

    twio::IWriterPtr GetWriter() const override { return _writer; }

private:
    // Return the character to be put into output stream. 0 if nothing, -1 if EOF.
    void _Process(int ch);
    void _ProcessAny(int ch);
    void _ProcessSlash(int ch);
    void _ProcessLineComment(int ch);
    void _ProcessBlockCommentLeft(int ch);
    void _ProcessBlockCommentRight(int ch);
    void _ProcessQuote(int ch);

    twio::IReaderPtr _reader;
    twio::IWriterPtr _writer;

    static const char FILLING = ' ';

    enum class StateType
    {
        ANY,                    // any state
        SLASH,                  // '/'
        LINE_COMMENT,           // '//'
        BLOCK_COMMENT_LEFT,     // '/*'
        BLOCK_COMMENT_RIGHT,    // '*' (waiting for '/')
        QUOTE                   // ' or ", with value set
    };

    struct State
    {
        StateType type;
        int value;
    } _state;
};


TOMIC_END

#endif
