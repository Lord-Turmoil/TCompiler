/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/Use.h>
#include <tomic/llvm/ir/value/User.h>
#include <tomic/llvm/ir/value/Value.h>
#include <tomic/llvm/ir/LlvmContext.h>

TOMIC_LLVM_BEGIN

UsePtr Use::New(UserPtr user, ValuePtr value)
{
    auto use = std::shared_ptr<Use>(new Use(user, value));
    return user->Context()->StoreUse(use);
}


TOMIC_LLVM_END
