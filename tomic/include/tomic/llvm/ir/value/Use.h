/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_USE_H_
#define _TOMIC_LLVM_USE_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/IrForward.h>
#include <memory>

TOMIC_LLVM_BEGIN


class Use
{
public:
    Use(const Use&) = delete;
    Use& operator=(const Use&) = delete;

    static UsePtr New(UserPtr user, ValuePtr value);

    // Avoid using 'Value' again as function name.
    ValuePtr GetValue() const { return _value; }
    // Avoid using 'User' again as function name.
    UserPtr GetUser() const { return _user; }

private:
    Use(UserPtr user, ValuePtr value) : _user(user), _value(value) {}

    UserPtr _user;
    ValuePtr _value;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_USE_H_
