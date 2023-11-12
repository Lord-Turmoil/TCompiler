/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/inst/Instructions.h>
#include <tomic/llvm/ir/LlvmContext.h>

TOMIC_LLVM_BEGIN

/*
 * ============================== AllocaInst ==============================
 */
AllocaInst::AllocaInst(TypePtr type, int alignment)
        : Instruction(ValueType::AllocaInstTy, type)
{
    _allocatedType = type;
    _alignment = alignment;
}

AllocaInstPtr AllocaInst::New(TypePtr type, int alignment)
{
    auto inst = std::shared_ptr<AllocaInst>(new AllocaInst(type, alignment));

    type->Context()->StoreValue(inst);

    return inst.get();
}

/*
 * ============================== LoadInst ==============================
 */

LoadInst::LoadInst(TypePtr type, ValuePtr address)
        : UnaryInstruction(ValueType::LoadInstTy, type, address)
{
}

LoadInstPtr LoadInst::New(TypePtr type, ValuePtr address)
{
    auto inst = std::shared_ptr<LoadInst>(new LoadInst(type, address));

    type->Context()->StoreValue(inst);

    return inst.get();
}

/*
 * ============================== StoreInst ==============================
 */

StoreInst::StoreInst(ValuePtr value, ValuePtr address)
        : Instruction(ValueType::StoreInstTy, value->Context()->GetVoidTy())
{
    AddOperand(value);
    AddOperand(address);
}

StoreInstPtr StoreInst::New(ValuePtr value, ValuePtr address)
{
    auto inst = std::shared_ptr<StoreInst>(new StoreInst(value, address));

    value->Context()->StoreValue(inst);

    return inst.get();
}

TOMIC_LLVM_END
