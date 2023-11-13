/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/User.h>

TOMIC_LLVM_BEGIN

void User::AddOperand(ValuePtr value)
{
    auto use = Use::New(this, value);

    AddUse(use);
    value->AddUse(use);
}

TOMIC_LLVM_END
