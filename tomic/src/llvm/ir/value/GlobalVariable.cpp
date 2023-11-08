/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/GlobalVariable.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/LlvmContext.h>

TOMIC_LLVM_BEGIN

GlobalVariable::GlobalVariable(TypePtr type, bool isConstant, const std::string& name)
        : GlobalValue(ValueType::GlobalVariableTy, type, name), _isConstant(isConstant), _initializer(nullptr)
{
}

GlobalVariable::GlobalVariable(TypePtr type, bool isConstant, const std::string& name, InitializerPtr initializer)
        : GlobalValue(ValueType::GlobalVariableTy, type, name), _isConstant(isConstant), _initializer(initializer)
{
}

GlobalVariablePtr GlobalVariable::New(TypePtr type, bool isConstant, const std::string& name)
{
    auto globalVariable = std::shared_ptr<GlobalVariable>(new GlobalVariable(type, isConstant, name));

    type->Context()->StoreValue(globalVariable);

    return globalVariable.get();
}

GlobalVariablePtr GlobalVariable::New(TypePtr type,
                                      bool isConstant,
                                      const std::string& name,
                                      InitializerPtr initializer)
{
    auto globalVariable = std::shared_ptr<GlobalVariable>(new GlobalVariable(type, isConstant, name, initializer));

    type->Context()->StoreValue(globalVariable);

    return globalVariable.get();
}

TOMIC_LLVM_END
