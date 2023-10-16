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

class IConfig
{
public:
    virtual ~IConfig() = default;

    virtual const char* Input() const = 0;
    virtual const char* Output() const = 0;
    virtual const char* Error() const = 0;
    virtual const char* Ext() const = 0;
    virtual bool Silent() const = 0;
    virtual bool EnableCompleteAst() const = 0;
    virtual bool EnableLog() const = 0;
    virtual bool EnableVerboseError() const = 0;
};

using IConfigPtr = std::shared_ptr<IConfig>;

class Config : public IConfig
{
public:
    Config() :
            _input("input.txt"),
            _output("output.txt"),
            _error("error.txt"),
            _outputExt(".txt"),
            _silent(true),
            _enableCompleteAst(false),
            _enableLog(false),
            _enableVerboseError(false) {}
    ~Config() override = default;

    static std::shared_ptr<Config> New() { return std::make_shared<Config>(); }

    const char* Input() const override { return _input.c_str(); }
    const char* Output() const override { return _output.c_str(); }
    const char* Error() const override { return _error.c_str(); }
    const char* Ext() const override { return _outputExt.c_str(); }
    bool Silent() const override { return _silent; }
    bool EnableCompleteAst() const override { return _enableCompleteAst; }
    bool EnableLog() const override { return _enableLog; }
    bool EnableVerboseError() const override { return _enableVerboseError; }

public:
    Config* SetInput(const char* input)
    {
        _input = input;
        return this;
    }
    Config* SetOutput(const char* output)
    {
        _output = output;
        return this;
    }
    Config* SetError(const char* error)
    {
        _error = error;
        return this;
    }
    Config* SetExt(const char* ext)
    {
        _outputExt = ext;
        return this;
    }
    Config* SetSilent(bool silent)
    {
        _silent = silent;
        return this;
    }
    Config* SetEnableCompleteAst(bool enable)
    {
        _enableCompleteAst = enable;
        return this;
    }
    Config* SetEnableLog(bool enable)
    {
        _enableLog = enable;
        return this;
    }
    Config* SetEnableVerboseError(bool enable)
    {
        _enableVerboseError = enable;
        return this;
    }

private:
    std::string _input;
    std::string _output;
    std::string _error;
    std::string _outputExt;
    bool _silent;
    bool _enableCompleteAst;
    bool _enableLog;
    bool _enableVerboseError;
};

using ConfigPtr = std::shared_ptr<Config>;

TOMIC_END

#endif // _TOMIC_CONFIG_H_
