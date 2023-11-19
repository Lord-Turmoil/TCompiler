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
    : Instruction(ValueType::InputInstTy, type), _name("getint")
{
}

InputInstPtr InputInst::New(LlvmContextPtr context)
{
    auto inst = std::shared_ptr<InputInst>(new InputInst(context->GetInt32Ty()));

    context->StoreValue(inst);

    return inst.get();
}

TOMIC_LLVM_END
