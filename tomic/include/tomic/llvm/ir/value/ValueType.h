/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_VALUE_TYPE_H_
#define _TOMIC_LLVM_VALUE_TYPE_H_

#include <tomic/llvm/Llvm.h>

TOMIC_LLVM_BEGIN

/*
 * ValueType is an enum class that represents the type of Value.
 * ... Too many types to list here. :(
 */
enum class ValueType
{
    // === Value ===
    ArgumentTy,
    BasicBlockTy,

    // === Value.User.Instruction ===
    BinaryOperatorTy,
    BranchInstTy,
    IndirectBrInstTy,
    GetElementPtrInstTy,
    ReturnInstTy,
    StoreInstTy,

    // === Value.User.Instruction.UnaryInstruction ===
    AllocaInstTy,
    LoadInstTy,
    UnaryOperatorTy,

    // === Value.User.Constant.GlobalValue.GlobalObject ===
    ConstantTy,
    GlobalValueTy,
    FunctionTy,
    GlobalVariableTy,
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_VALUE_TYPE_H_
