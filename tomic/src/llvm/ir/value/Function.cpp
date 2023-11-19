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

FunctionPtr Function::New(TypePtr returnType, const std::string& name)
{
    auto func = std::shared_ptr<Function>(new Function(FunctionType::Get(returnType), name));

    returnType->Context()->StoreValue(func);

    return func.get();
}

FunctionPtr Function::New(TypePtr returnType, const std::string& name, std::vector<ArgumentPtr> args)
{
    std::vector<TypePtr> argTypes;
    for (auto arg : args)
    {
        argTypes.push_back(arg->GetType());
    }

    auto func = std::shared_ptr<Function>(new Function(FunctionType::Get(returnType, argTypes), name, std::move(args)));

    returnType->Context()->StoreValue(func);

    return func.get();
}

BasicBlockPtr Function::NewBasicBlock()
{
    return BasicBlock::New(this);
}

Function::Function(TypePtr type, const std::string& name)
    : GlobalValue(ValueType::FunctionTy, type, name)
{
}

Function::Function(TypePtr type, const std::string& name, std::vector<ArgumentPtr> args)
    : GlobalValue(ValueType::FunctionTy, type, name), _args(std::move(args))
{
    for (auto arg : _args)
    {
        arg->SetParent(this);
    }
}


// Insert a basic block at the end of the function.
FunctionPtr Function::InsertBasicBlock(BasicBlockPtr block)
{
    block->SetParent(this);
    _basicBlocks.push_back(block);
    return this;
}


FunctionPtr Function::InsertBasicBlock(block_iterator iter, BasicBlockPtr block)
{
    block->SetParent(this);
    _basicBlocks.insert(iter, block);
    return this;
}


FunctionPtr Function::RemoveBasicBlock(BasicBlockPtr block)
{
    block->SetParent(nullptr);
    _basicBlocks.remove(block);
    return this;
}


TOMIC_LLVM_END
