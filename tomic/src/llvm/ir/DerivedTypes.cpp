/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/DerivedTypes.h>
#include <tomic/llvm/ir/LlvmContext.h>

TOMIC_LLVM_BEGIN

/*
 * ==================== IntegerType ====================
 */

IntegerTypePtr IntegerType::Get(LlvmContextPtr context, unsigned bitWidth)
{
    TOMIC_ASSERT((bitWidth == 32) && "Unsupported bit width");
    return context->GetInt32Ty();
}


/*
 * ==================== FunctionType ====================
 */

FunctionType::FunctionType(TypePtr returnType, const std::vector<TypePtr>& paramTypes)
        : Type(returnType->Context(), FunctionTyID)
{
    _AddContainedType(returnType);
    for (auto paramType : paramTypes)
    {
        _AddContainedType(paramType);
    }
}

FunctionType::FunctionType(TypePtr returnType)
        : Type(returnType->Context(), FunctionTyID)
{
    _AddContainedType(returnType);
}

FunctionTypePtr FunctionType::Get(TypePtr returnType, const std::vector<Type*>& paramTypes)
{
    return returnType->Context()->GetFunctionType(returnType, paramTypes);
}

FunctionTypePtr FunctionType::Get(TypePtr returnType)
{
    return returnType->Context()->GetFunctionType(returnType);
}

bool FunctionType::Equals(TypePtr returnType, const std::vector<TypePtr>& paramTypes) const
{
    if ((returnType != ReturnType()) || (paramTypes.size() != ParamCount()))
    {
        return false;
    }

    auto selfIter = ParamBegin();
    auto otherIter = paramTypes.begin();
    while (selfIter != ParamEnd())
    {
        if (*selfIter != *otherIter)
        {
            return false;
        }
        ++selfIter;
        ++otherIter;
    }

    return true;
}

bool FunctionType::Equals(TypePtr returnType) const
{
    return (returnType == ReturnType()) && (ParamCount() == 0);
}


/*
 * ==================== ArrayType ====================
 */

ArrayType::ArrayType(TypePtr elementType, int elementCount)
        : Type(elementType->Context(), ArrayTyID), _elementType(elementType), _elementCount(elementCount)
{
}

ArrayTypePtr ArrayType::Get(TypePtr elementType, int elementCount)
{
    return elementType->Context()->GetArrayType(elementType, elementCount);
}


/*
 * ==================== PointerType ====================
 */

PointerTypePtr PointerType::Get(TypePtr elementType)
{
    return elementType->Context()->GetPointerType(elementType);
}

PointerType::PointerType(TypePtr elementType)
        : Type(elementType->Context(), PointerTyID), _elementType(elementType)
{
}

TOMIC_LLVM_END
