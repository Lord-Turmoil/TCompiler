/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_ARGUMENT_H_
#define _TOMIC_LLVM_ARGUMENT_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/Value.h>
#include <tomic/llvm/ir/IrForward.h>

TOMIC_LLVM_BEGIN

class Argument final : public Value
{
public:
    Argument(TypePtr type, const std::string& name, FunctionPtr parent, int argNo);
    ~Argument() = default;

    static Argument New(TypePtr type, const std::string& name, FunctionPtr parent, int argNo);

    FunctionPtr Parent() const;
    int ArgNo() const;

private:
    FunctionPtr _parent;
    int _argNo;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_ARGUMENT_H_