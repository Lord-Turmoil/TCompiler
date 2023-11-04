/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_GENERATOR_H_
#define _TOMIC_LLVM_GENERATOR_H_

#include <tomic/Shared.h>
#include <tomic/parser/table/SymbolTableForward.h>
#include <tomic/parser/ast/SyntaxTree.h>
#include <memory>

TOMIC_BEGIN

class ILlvmGenerator
{
public:
    virtual ~ILlvmGenerator() = default;

    virtual ILlvmGenerator* SetWriter(twio::IWriterPtr writer) = 0;
    virtual void Generate(SyntaxTreePtr tree, SymbolTablePtr table) = 0;
};

using ILlvmGeneratorPtr = std::shared_ptr<ILlvmGenerator>;

TOMIC_END

#endif // _TOMIC_LLVM_GENERATOR_H_
