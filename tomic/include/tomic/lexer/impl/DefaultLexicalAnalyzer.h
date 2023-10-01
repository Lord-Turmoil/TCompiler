/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_LEXICAL_ANALYZER_H_
#define _TOMIC_DEFAULT_LEXICAL_ANALYZER_H_

#include <vector>
#include "../../Common.h"
#include "../ILexicalAnalyzer.h"
#include <memory>

TOMIC_BEGIN

/*
 * LexicalTask is the base class of all tasks in DefaultLexicalAnalyser.
 * It is used to perform a specific task, such as analyse a number or an identifier.
 * The tasks are registered to the analyser, and the analyser will choose the right
 * task to perform.
 * The concrete tasks are defined below.
 */
class LexicalTask
{
public:
    virtual ~LexicalTask() = default;

    // Return whether this task begins with the given character.
    // Used to choose which task to perform.
    virtual bool BeginsWith(int begin) const = 0;

    // Return whether this task ends with the given character.
    // Use to check if the TokenPtr is properly ended or not.
    virtual bool EndsWith(int end) const = 0;

    // Accept the reader from LexicalAnalyser, and return the next TokenPtr.
    virtual TokenPtr Analyse(const twio::IAdvancedReaderPtr& reader) = 0;
};

using LexicalTaskPtr = std::shared_ptr<LexicalTask>;

/*
 * DefaultLexicalAnalyser is the default implementation of ILexicalAnalyser.
 * It is a state machine, and it will choose the right task to perform.
 * The tasks are defined in LexicalTask and will be registered to the analyser.
 */
class DefaultLexicalAnalyzer : public ILexicalAnalyzer
{
public:
    DefaultLexicalAnalyzer();
    ~DefaultLexicalAnalyzer() override = default;

    DefaultLexicalAnalyzer* SetReader(twio::IAdvancedReaderPtr reader) override;

    TokenPtr Next() override;

private:
    void _InitTasks();
    TokenPtr _Next();

    twio::IAdvancedReaderPtr _reader;
    std::vector<LexicalTaskPtr> _tasks;
};


/*
 * The following classes are the concrete tasks.
 */

// Recognize a number.
class NumberLexicalTask : public LexicalTask
{
public:
    bool BeginsWith(int begin) const override;
    bool EndsWith(int end) const override;
    TokenPtr Analyse(const twio::IAdvancedReaderPtr& reader) override;
};

// Recognize an identifier. (variable name, function name, etc.)
// Can be keywords.
class IdentifierLexicalTask : public LexicalTask
{
public:
    bool BeginsWith(int begin) const override;
    bool EndsWith(int end) const override;
    TokenPtr Analyse(const twio::IAdvancedReaderPtr& reader) override;
};

// Recognize a string literal.
class StringLexicalTask : public LexicalTask
{
public:
    bool BeginsWith(int begin) const override;
    bool EndsWith(int end) const override;
    TokenPtr Analyse(const twio::IAdvancedReaderPtr& reader) override;

private:
    bool _IsNormalChar(int ch) const;
    bool _IsNewLineChar(int ch, const twio::IAdvancedReaderPtr& reader) const;
    bool _IsFormatChar(int ch, const twio::IAdvancedReaderPtr& reader) const;
};

// Recognize a single-character operator. (+, -, *, /, brackets, etc.)
// But not >, & that will get confused with double-character operators.
class SingleOpLexicalTask : public LexicalTask
{
public:
    bool BeginsWith(int begin) const override;
    bool EndsWith(int end) const override;
    TokenPtr Analyse(const twio::IAdvancedReaderPtr& reader) override;
};

// Recognize a double-character operator. (&&, >=, etc.)
// And also those single-character operators.
class DoubleOpLexicalTask : public LexicalTask
{
public:
    bool BeginsWith(int begin) const override;
    bool EndsWith(int end) const override;
    TokenPtr Analyse(const twio::IAdvancedReaderPtr& reader) override;
};

// Recognize a delimiter. (comma, semicolon, brackets etc.)
class DelimiterLexicalTask : public LexicalTask
{
public:
    bool BeginsWith(int begin) const override;
    bool EndsWith(int end) const override;
    TokenPtr Analyse(const twio::IAdvancedReaderPtr& reader) override;
};

class UnknownLexicalTask : public LexicalTask
{
public:
    bool BeginsWith(int begin) const override;
    bool EndsWith(int end) const override;
    TokenPtr Analyse(const twio::IAdvancedReaderPtr& reader) override;
};

TOMIC_END

#endif // _DEFAULT_LEXICAL_ANALYZER_H_
