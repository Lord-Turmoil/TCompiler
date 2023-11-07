/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/DerivedTypes.h>

TOMIC_LLVM_BEGIN

LlvmContext::LlvmContext() :
        voidTy(this, Type::VoidTyID),
        labelTy(this, Type::LabelTyID),
        int32Ty(this, 32)
{
}

ArrayTypePtr LlvmContext::GetArrayType(TypePtr elementType, int elementCount)
{
    ArrayTypePair pair(elementType, elementCount);
    auto iter = _arrayTypes.find(pair);

    if (iter != _arrayTypes.end())
    {
        return iter->second.get();
    }
    else
    {
        auto it = _arrayTypes.emplace(pair, new ArrayType(elementType, elementCount));
        return it.first->second.get();
    }
}

FunctionTypePtr LlvmContext::GetFunctionType(TypePtr returnType, const std::vector<TypePtr>& paramTypes)
{
    for (auto& type : _functionTypes)
    {
        if (type->Equals(returnType, paramTypes))
        {
            return type.get();
        }
    }

    auto type = _functionTypes.emplace_back(new FunctionType(returnType, paramTypes));
    return type.get();
}

FunctionTypePtr LlvmContext::GetFunctionType(TypePtr returnType)
{
    for (auto& type : _functionTypes)
    {
        if (type->Equals(returnType))
        {
            return type.get();
        }
    }

    auto type = _functionTypes.emplace_back(new FunctionType(returnType));
    return type.get();
}

ValuePtr LlvmContext::StoreValue(ValueSmartPtr value)
{
    _valueMap.emplace(value.get(), value);
    return value.get();
}

void LlvmContext::RemoveValue(ValuePtr value)
{
    _valueMap.erase(value);
}

UsePtr LlvmContext::StoreUse(UseSmartPtr use)
{
    _useMap.emplace(use.get(), use);
    return use.get();
}

void LlvmContext::RemoveUse(UsePtr use)
{
    _useMap.erase(use);
}


TOMIC_LLVM_END
