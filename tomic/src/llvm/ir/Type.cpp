/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/Type.h>

TOMIC_LLVM_BEGIN

TypePtr Type::GetVoidTy(LlvmContextPtr context)
{
    return context->GetVoidTy();
}

TypePtr Type::GetLabelTy(LlvmContextPtr context)
{
    return context->GetLabelTy();
}

TOMIC_LLVM_END
