/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_IR_INSTRUCTIONS_H_
#define _TOMIC_LLVM_IR_INSTRUCTIONS_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/value/inst/Instruction.h>
#include <tomic/llvm/ir/value/inst/InstructionTypes.h>

TOMIC_LLVM_BEGIN

/*
 * ============================== AllocaInst ==============================
 */

/*
 * AllocaInst will convert the type to its corresponding pointer. So
 * later when we use LoadInst, we can get the pointer value directly.
 * Since we didn't strictly follow LLVM to use Value as the number of
 * allocations, here we make AllocaInst derive directly from Instruction,
 * instead of UnaryInstruction.
 */

//  %1 = alloca i32, align 4

class AllocaInst final : public Instruction
{
public:
    ~AllocaInst() override = default;

    static AllocaInstPtr New(TypePtr type, int alignment = 0);

    void PrintAsm(IAsmWriterPtr writer) override;

private:
    AllocaInst(TypePtr type, int alignment);

    TypePtr _allocatedType;
    int _alignment;
};


/*
 * ============================== LoadInst ==============================
 */

//  %4 = load i32, i32* %2, align 4

class LoadInst final : public UnaryInstruction
{
public:
    ~LoadInst() override = default;

    static LoadInstPtr New(TypePtr type, ValuePtr address);

private:
    LoadInst(TypePtr type, ValuePtr address);
};


/*
 * ============================== StoreInst ==============================
 */

/*
 * The target of store instruction is a pointer, not a value.
 * It is actually a binary instruction, but LLVM doesn't treat it as
 * a binary instruction. So... Just instruction here. :)
 */

// store i32 0, i32* %1, align 4
// store i32 %4, i32* %3, align 4

class StoreInst final : public Instruction
{
public:
    ~StoreInst() override = default;

    static StoreInstPtr New(ValuePtr value, ValuePtr address);

    void PrintAsm(IAsmWriterPtr writer) override;

private:
    StoreInst(ValuePtr value, ValuePtr address);
};


/*
 * ============================== ReturnInst ==============================
 */

/*
 * ReturnInst is not strictly a unary instruction, since it may not require
 * a value to be returned. So we just make it a simple instruction.
 */

class ReturnInst final : public Instruction
{
public:
    ~ReturnInst() override = default;

    static ReturnInstPtr New(LlvmContextPtr context, ValuePtr value);
    static ReturnInstPtr New(LlvmContextPtr context);

    void PrintAsm(IAsmWriterPtr writer) override;

private:
    ReturnInst(TypePtr type, ValuePtr value);
    ReturnInst(TypePtr type);
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_IR_INSTRUCTIONS_H_
