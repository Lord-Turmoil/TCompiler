/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_LLVM_CONTEXT_H_
#define _TOMIC_LLVM_LLVM_CONTEXT_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/DerivedTypes.h>

#include <unordered_map>
#include <utility>

TOMIC_LLVM_BEGIN

class LlvmContext;
using LlvmContextPtr = LlvmContext*;

class LlvmContext
{
public:
    static LlvmContextPtr Instance();

    LlvmContext(const LlvmContext&) = delete;
    LlvmContext& operator=(const LlvmContext&) = delete;

    std::unordered_map<std::pair<TypePtr, int>, ArrayTypePtr> ArrayTypes;

    // Primitive types
    Type VoidTy, LabelTy;

    // Derived types
    IntegerType Int1Ty, Int8Ty, Int32Ty, Int64Ty;
    FunctionType FunctionTy;

private:
    // Prohibit direct instantiation.
    LlvmContext();

    static LlvmContextPtr _instance;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_LLVM_CONTEXT_H_
