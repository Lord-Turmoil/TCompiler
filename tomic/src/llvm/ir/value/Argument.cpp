/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/value/Argument.h>
#include <tomic/llvm/ir/value/Function.h>

TOMIC_LLVM_BEGIN

ArgumentPtr Argument::New(TypePtr type, const std::string& name, int argNo)
{
    auto arg = std::shared_ptr<Argument>(new Argument(type, name, argNo));

    type->Context()->StoreValue(arg);

    return arg.get();
}


Argument::Argument(TypePtr type, const std::string& name, int argNo)
    : Value(ValueType::ArgumentTy, type), _parent(nullptr), _argNo(argNo)
{
    SetName(name);
}


TOMIC_LLVM_END
