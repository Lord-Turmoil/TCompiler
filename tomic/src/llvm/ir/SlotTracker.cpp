/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/SlotTracker.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/value/BasicBlock.h>
#include <tomic/llvm/ir/value/inst/Instruction.h>

TOMIC_LLVM_BEGIN

void SlotTracker::Trace(FunctionPtr function)
{
    // Clear history.
    _slot.clear();

    int slot = 0;

    // First, add all parameters.
    for (auto arg = function->ArgBegin(); arg != function->ArgEnd(); ++arg)
    {
        _slot.emplace(*arg, slot++);
    }

    // Then add all basic blocks, and all instructions in each basic block.
    for (auto blockIter = function->BasicBlockBegin(); blockIter != function->BasicBlockEnd(); ++blockIter)
    {
        auto block = *blockIter;
        _slot.emplace(block, slot++);

        for (auto instIter = block->InstructionBegin(); instIter != block->InstructionEnd(); ++instIter)
        {
            auto inst = *instIter;

            // We only track non-void instructions.
            if (!inst->GetType()->IsVoidTy())
            {
                _slot.emplace(inst, slot++);
            }
        }
    }
}

int SlotTracker::Slot(ValuePtr value)
{
    auto iter = _slot.find(value);
    if (iter != _slot.end())
    {
        return iter->second;
    }

    TOMIC_PANIC("Value not found in slot tracker.");

    return 0;
}

TOMIC_LLVM_END
