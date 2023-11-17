/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/BasicBlock.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/value/inst/Instructions.h>

TOMIC_LLVM_BEGIN

BasicBlockPtr BasicBlock::New(FunctionPtr parent)
{
    auto block = std::shared_ptr<BasicBlock>(new BasicBlock(parent));

    block->Context()->StoreValue(block);

    return block.get();
}

BasicBlock::BasicBlock(FunctionPtr parent)
        : Value(ValueType::BasicBlockTy, parent->Context()->GetVoidTy()),
          _parent(parent)
{
}

BasicBlockPtr BasicBlock::InsertInstruction(InstructionPtr inst)
{
    inst->SetParent(this);
    _instructions.push_back(inst);

    return this;
}

BasicBlockPtr BasicBlock::InsertInstruction(instruction_iterator iter, InstructionPtr inst)
{
    inst->SetParent(this);
    _instructions.insert(iter, inst);

    return this;
}

BasicBlockPtr BasicBlock::RemoveInstruction(InstructionPtr inst)
{
    inst->SetParent(nullptr);
    _instructions.remove(inst);

    return this;
}

TOMIC_LLVM_END
