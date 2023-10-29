/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_CONFIG_H_
#define _TOMIC_CONFIG_H_

#include <tomic/Shared.h>
#include <memory>
#include <string>

TOMIC_BEGIN

class Config
{
public:
    Config() :
            _input("input.txt"),
            _astOutput("output.txt"),
            _irOutput("ir.txt"),
            _errorOutput("error.txt"),
            _logOutput("null"),
            _enableCompleteAst(false),
            _enableVerboseError(false) {}
    ~Config() = default;

    static std::shared_ptr<Config> New() { return std::make_shared<Config>(); }

    const char* Input() const { return _input.c_str(); }
    const char* AstOutput() const { return _astOutput.c_str(); }
    const char* IrOutput() const { return _irOutput.c_str(); }
    const char* ErrorOutput() const { return _errorOutput.c_str(); }
    const char* LogOutput() const { return _logOutput.c_str(); }
    bool EnableCompleteAst() const { return _enableCompleteAst; }
    bool EnableVerboseError() const { return _enableVerboseError; }

public:
    Config* SetInput(const char* input)
    {
        _input = input;
        return this;
    }
    Config* SetAstOutput(const char* astOutput)
    {
        _astOutput = astOutput;
        return this;
    }
    Config* SetIrOutput(const char* irOutput)
    {
        _irOutput = irOutput;
        return this;
    }
    Config* SetErrorOutput(const char* errorOutput)
    {
        _errorOutput = errorOutput;
        return this;
    }
    Config* SetLogOutput(const char* logOutput)
    {
        _logOutput = logOutput;
        return this;
    }
    Config* SetEnableCompleteAst(bool enable)
    {
        _enableCompleteAst = enable;
        return this;
    }
    Config* SetEnableVerboseError(bool enable)
    {
        _enableVerboseError = enable;
        return this;
    }

private:
    std::string _input;
    std::string _astOutput;
    std::string _irOutput;
    std::string _errorOutput;
    std::string _logOutput;
    bool _enableCompleteAst;
    bool _enableVerboseError;
};

using ConfigPtr = std::shared_ptr<Config>;

TOMIC_END

#endif // _TOMIC_CONFIG_H_
