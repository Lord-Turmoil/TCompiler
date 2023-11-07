/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_LLVM_CONTEXT_H_
#define _TOMIC_LLVM_LLVM_CONTEXT_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/IrForward.h>
#include <tomic/llvm/ir/DerivedTypes.h>
#include <tomic/llvm/ir/value/Value.h>
#include <unordered_map>
#include <utility>

TOMIC_LLVM_BEGIN

// Hope this hash function works well...

struct ArrayTypePair
{
    ArrayTypePair() : first(nullptr), second(0) {}
    ArrayTypePair(TypePtr type, int count) : first(type), second(count) {}

    bool operator==(const ArrayTypePair& other) const noexcept
    {
        return (first == other.first) && (second == other.second);
    }

    TypePtr first;
    int second;
};

struct ArrayTypePairHash
{
    size_t operator()(const ArrayTypePair& p) const noexcept
    {
        return ((size_t)(p.first) << 32) | (size_t)(p.second);
    }
};

class LlvmContext
{
    // Only Module can create LlvmContext.
    friend class Module;

public:
    ~LlvmContext() = default;

    LlvmContext(const LlvmContext&) = delete;
    LlvmContext& operator=(const LlvmContext&) = delete;

    TypePtr GetVoidTy() { return &voidTy; }
    TypePtr GetLabelTy() { return &labelTy; }
    IntegerTypePtr GetInt32Ty() { return &int32Ty; }

    ArrayTypePtr GetArrayType(TypePtr elementType, int elementCount);
    FunctionTypePtr GetFunctionType(TypePtr returnType, const std::vector<TypePtr>& paramTypes);
    FunctionTypePtr GetFunctionType(TypePtr returnType);

    // We want to use raw pointers, but do not want to delete them manually.
    // So we use shared_ptr to manage them internally. :P
    ValuePtr StoreValue(ValueSmartPtr value);
    void RemoveValue(ValuePtr value);

    UsePtr StoreUse(UseSmartPtr use);
    void RemoveUse(UsePtr use);

private:
    // Prohibit direct instantiation.
    LlvmContext();

    // Primitive types
    Type voidTy, labelTy;

    // Derived types
    IntegerType int32Ty;

    /*
     * Since std container will auto-grow, direct memory storage may cause address
     * inconsistency, so we use shared_ptr to manage them.
     */
    std::unordered_map<ArrayTypePair, ArrayTypeSmartPtr, ArrayTypePairHash> _arrayTypes;

    /*
     * Since it is not that simple to compare two function types, we use a vector
     * to store all function types, and compare them one by one... :(
     */
    std::vector<FunctionTypeSmartPtr> _functionTypes;

    /*
     * Since we want to use raw pointers, we need to store them somewhere.
     * We use a map to store them, and use shared_ptr to manage them.
     * But in fact, we use raw pointers everywhere. :P
     */
    std::unordered_map<ValuePtr, ValueSmartPtr> _valueMap;
    std::unordered_map<UsePtr, UseSmartPtr> _useMap;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_LLVM_CONTEXT_H_
