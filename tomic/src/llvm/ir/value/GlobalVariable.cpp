/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/value/GlobalVariable.h>

#include <tomic/utils/StringUtil.h>

TOMIC_LLVM_BEGIN

/*
 * ==================== GlobalVariable ====================
 */
//
GlobalVariable::GlobalVariable(TypePtr type, bool isConstant, const std::string& name)
    : GlobalValue(ValueType::GlobalVariableTy, type, name), _isConstant(isConstant), _initializer(nullptr)
{
}


GlobalVariable::GlobalVariable(TypePtr type, bool isConstant, const std::string& name, ConstantDataPtr initializer)
    : GlobalValue(ValueType::GlobalVariableTy, type, name), _isConstant(isConstant), _initializer(initializer)
{
}


GlobalVariablePtr GlobalVariable::New(TypePtr valueType, bool isConstant, const std::string& name)
{
    auto type = PointerType::Get(valueType);
    auto value = std::shared_ptr<GlobalVariable>(new GlobalVariable(type, isConstant, name));

    type->Context()->StoreValue(value);

    return value.get();
}


GlobalVariablePtr GlobalVariable::New(TypePtr valueType,
                                      bool isConstant,
                                      const std::string& name,
                                      ConstantDataPtr initializer)
{
    auto type = PointerType::Get(valueType);
    auto value = std::shared_ptr<GlobalVariable>(new GlobalVariable(type, isConstant, name, initializer));

    type->Context()->StoreValue(value);

    return value.get();
}


/*
 * ==================== GlobalString ====================
 */

GlobalString::GlobalString(TypePtr type, std::string value, const std::string& name)
    : GlobalValue(ValueType::GlobalStringTy, type, name), _value(std::move(value))
{
}


GlobalStringPtr GlobalString::New(LlvmContextPtr context, const std::string& value)
{
    // Generate random name.
    static int idx = -1;
    std::string name = ".str";
    if (++idx > 0)
    {
        name.push_back('.');
        name.append(StringUtil::IntToString(idx));
    }

    // Get the type.
    int size = static_cast<int>(value.length()) + 1;
    auto type = PointerType::Get(ArrayType::Get(IntegerType::Get(context, 8), size));

    auto globalString = std::shared_ptr<GlobalString>(new GlobalString(type, value, name));
    context->StoreValue(globalString);

    return globalString.get();
}


TOMIC_LLVM_END
