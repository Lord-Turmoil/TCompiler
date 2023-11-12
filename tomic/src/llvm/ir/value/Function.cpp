/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/value/Argument.h>
#include <tomic/llvm/ir/value/BasicBlock.h>
#include <tomic/llvm/ir/LlvmContext.h>

TOMIC_LLVM_BEGIN

FunctionPtr Function::New(TypePtr type, const std::string& name)
{
    auto func = std::shared_ptr<Function>(new Function(type, name));

    type->Context()->StoreValue(func);

    return func.get();
}

Function::Function(TypePtr type, const std::string& name)
        : GlobalValue(ValueType::FunctionTy, type, name)
{
}

// Insert a basic block at the end of the function.
FunctionPtr Function::InsertBasicBlock(BasicBlockPtr block)
{
    _basicBlocks.push_back(block);
    return this;
}

FunctionPtr Function::InsertBasicBlock(block_iterator iter, BasicBlockPtr block)
{
    _basicBlocks.insert(iter, block);
    return this;
}

FunctionPtr Function::RemoveBasicBlock(BasicBlockPtr block)
{
    _basicBlocks.remove(block);
    return this;
}

TOMIC_LLVM_END
