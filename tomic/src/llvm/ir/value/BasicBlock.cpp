/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/BasicBlock.h>
#include <tomic/llvm/ir/value/Function.h>
#include "tomic/llvm/ir/LlvmContext.h"

TOMIC_LLVM_BEGIN

BasicBlockPtr BasicBlock::New(FunctionPtr parent)
{
    auto block = std::shared_ptr<BasicBlock>(new BasicBlock(parent));

    LlvmContext::Instance()->StoreValue(block);

    return block.get();
}

BasicBlock::BasicBlock(FunctionPtr parent)
        : Value(ValueType::BasicBlockTy, LlvmContext::Instance()->GetVoidTy()),
          _parent(parent)
{
}

TOMIC_LLVM_END
