/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/Argument.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/LlvmContext.h>

TOMIC_LLVM_BEGIN

ArgumentPtr Argument::New(TypePtr type, FunctionPtr parent, int argNo)
{
    auto arg = std::shared_ptr<Argument>(new Argument(type, parent, argNo));

    type->Context()->StoreValue(arg);

    return arg.get();
}

Argument::Argument(tomic::llvm::TypePtr type, tomic::llvm::FunctionPtr parent, int argNo)
        : Value(ValueType::ArgumentTy, type), _parent(parent), _argNo(argNo)
{
}

TOMIC_LLVM_END

