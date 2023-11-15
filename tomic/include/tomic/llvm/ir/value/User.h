/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_USER_H_
#define _TOMIC_LLVM_USER_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/value/Value.h>

TOMIC_LLVM_BEGIN

class User : public Value
{
public:
    ~User() override = default;

    virtual bool IsUser() const override { return true; }

    // Get the number of operands.
    int OperandsCount() const { return _useList.size(); }

protected:
    // Prohibit direct construction of User.
    User(ValueType valueType, TypePtr type, UseListPtr useList)
            : Value(valueType, type, useList) {}
    User(ValueType valueType, TypePtr type)
            : Value(valueType, type) {}

protected:
    class value_iterator_impl : use_base_iterator_base
    {
    public:
        value_iterator_impl(_use_iterator_raw iter) : use_base_iterator_base(iter) {}

        ValuePtr operator*() const
        {
            return (*_iter)->GetValue();
        }

        ValuePtr operator->() const
        {
            return operator*();
        }
    };

public:
    using value_iterator = value_iterator_impl;

    value_iterator OperandBegin() { return value_iterator(_useList.begin()); }
    value_iterator OperandEnd() { return value_iterator(_useList.end()); }

    ValuePtr OperandAt(int index) const { return _useList[index]->GetValue(); }
    void AddOperand(ValuePtr value);
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_USER_H_
