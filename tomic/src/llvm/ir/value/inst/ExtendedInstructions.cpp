/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/value/inst/ExtendedInstructions.h>

TOMIC_LLVM_BEGIN

/*
 * ============================== InputInst ==============================
 */
//

InputInst::InputInst(TypePtr type)
    : Instruction(ValueType::InputInstTy, type)
{
    SetName("getint");
}


InputInstPtr InputInst::New(LlvmContextPtr context)
{
    auto inst = std::shared_ptr<InputInst>(new InputInst(context->GetInt32Ty()));

    context->StoreValue(inst);

    return inst.get();
}


/*
 * ============================== OutputInst ==============================
 */

OutputInst::OutputInst(ValuePtr value)
    : UnaryInstruction(ValueType::OutputInstTy, value->Context()->GetVoidTy(), value)
{
    _value = value;
    if (IsInteger())
    {
        SetName("putint");
    }
    else
    {
        SetName("putstr");
    }
}


OutputInstPtr OutputInst::New(ValuePtr value)
{
    auto inst = std::shared_ptr<OutputInst>(new OutputInst(value));

    value->Context()->StoreValue(inst);

    return inst.get();
}


bool OutputInst::IsInteger() const
{
    return GetValue()->GetType()->IsIntegerTy();
}


TOMIC_LLVM_END
