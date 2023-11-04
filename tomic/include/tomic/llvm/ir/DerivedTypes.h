/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Reference:
 *   https://llvm.org/doxygen/classllvm_1_1Type.html
 *   https://llvm.org/doxygen/DerivedTypes_8h_source.html
 */

#ifndef _TOMIC_LLVM_DERIVED_TYPE_H_
#define _TOMIC_LLVM_DERIVED_TYPE_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/Type.h>
#include <memory>

TOMIC_LLVM_BEGIN

/*
 * In LLVM implementation, types are shared
 */

class IntegerType : public Type
{
    friend class LlvmContext;
public:
    IntegerType(const IntegerType&) = delete;
    IntegerType& operator=(const IntegerType&) = delete;

    static IntegerType* Get(unsigned bitWidth);

    unsigned BitWidth() const { return _bitWidth; }

protected:
    IntegerType(unsigned bitWidth) : Type(IntegerTyID), _bitWidth(bitWidth) {}

private:
    unsigned _bitWidth;
};

using IntegerTypePtr = IntegerType*;

/*
 * Forgive me, for using such an implementation with low efficiency. :(
 */
class FunctionType;
using FunctionTypePtr = FunctionType*;
using FunctionTypeSmartPtr = std::shared_ptr<FunctionType>;

class FunctionType : public Type
{
    friend class LlvmContext;
public:
    static FunctionTypePtr Get(TypePtr returnType, const std::vector<Type*>& paramTypes);
    static FunctionTypePtr Get(TypePtr returnType);

    Type* ReturnType() const { return _returnType; }

    using param_iterator = std::vector<Type*>::iterator;
    using const_param_iterator = std::vector<Type*>::const_iterator;

    param_iterator ParamBegin() { return _containedTypes.begin() + 1; }
    param_iterator ParamEnd() { return _containedTypes.end(); }
    const_param_iterator ParamBegin() const { return _containedTypes.begin() + 1; }
    const_param_iterator ParamEnd() const { return _containedTypes.end(); }

    int ParamCount() const { return _containedTypes.size() - 1; }

    bool Equals(TypePtr returnType, const std::vector<Type*>& paramTypes) const;
    bool Equals(TypePtr returnType) const;

private:
    FunctionType(TypePtr returnType, const std::vector<Type*>& paramTypes);
    FunctionType(TypePtr returnType);

private:
    Type* _returnType;
};

class ArrayType;
using ArrayTypePtr = ArrayType*;
using ArrayTypeSmartPtr = std::shared_ptr<ArrayType>;

class ArrayType : public Type
{
    friend class LlvmContext;
public:
    ArrayType() : _elementType(nullptr), _elementCount(0) {}

    static ArrayTypePtr Get(TypePtr elementType, int elementCount);

    Type* ElementType() const { return _elementType; }
    int ElementCount() const { return _elementCount; }

protected:
    ArrayType(TypePtr elementType, int elementCount);

private:
    Type* _elementType;
    int _elementCount;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_TYPE_H_
