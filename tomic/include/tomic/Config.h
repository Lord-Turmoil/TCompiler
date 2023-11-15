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
    friend class ToMiCompiler;
public:
    enum class TargetType
    {
        Initial,
        Preprocess,
        Syntactic,
        Semantic,
        IR,
        ASM,
        COUNT
    };

    Config();
    ~Config() = default;

    static std::shared_ptr<Config> New() { return std::make_shared<Config>(); }

public:
    TargetType Target;

    // basic
    std::string Input;
    std::string Output;

    // AST
    bool EnableCompleteAst;
    bool EmitAst;
    std::string AstOutput;

    // LLVM IR
    bool EmitLlvm;
    std::string LlvmOutput;

    // logger
    bool EnableLog;
    std::string LogOutput;

    // error
    bool EnableError;
    bool EnableVerboseError;
    std::string ErrorOutput;
};

using ConfigPtr = std::shared_ptr<Config>;

TOMIC_END

#endif // _TOMIC_CONFIG_H_
