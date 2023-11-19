/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_HEADER_PREPROCESSOR_H_
#define _TOMIC_HEADER_PREPROCESSOR_H_

#include <tomic/lexer/IPreprocessor.h>
#include <tomic/Shared.h>

TOMIC_BEGIN

/*
 * Replace not only comments, but also include directives.
 */
//
class HeaderPreprocessor : public IPreprocessor
{
public:
    HeaderPreprocessor();
    ~HeaderPreprocessor() override = default;


    void Process() override;

    HeaderPreprocessor* SetReader(twio::IReaderPtr reader) override;
    HeaderPreprocessor* SetWriter(twio::IWriterPtr writer) override;

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

#endif // _TOMIC_HEADER_PREPROCESSOR_H_
