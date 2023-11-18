/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_CONSTANT_H_
#define _TOMIC_LLVM_CONSTANT_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/value/User.h>

TOMIC_LLVM_BEGIN

/*
 * I don't know how to implement this. The original LLVM implementation
 * is too complicated. :(
 */
class Constant : public User
{
public:
    ~Constant() override = default;

protected:
    Constant(ValueType valueType, TypePtr type, UseListPtr operands)
        : User(valueType, type, operands)
    {
    }
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_CONSTANT_H_
