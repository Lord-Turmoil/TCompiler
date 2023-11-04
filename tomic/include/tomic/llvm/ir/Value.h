/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Reference:
 *   https://llvm.org/doxygen/classllvm_1_1Value.html
 *   https://llvm.org/doxygen/Value_8h_source.html
 */

#ifndef _TOMIC_LLVM_VALUE_H_
#define _TOMIC_LLVM_VALUE_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/IrForward.h>
#include <tomic/llvm/ir/ValueType.h>
#include <tomic/llvm/ir/Use.h>
#include <vector>
#include <memory>

TOMIC_LLVM_BEGIN

class Value
{
public:
    Value(TypePtr type, ValueType valueType) : _type(type), _valueType(valueType) {}
    virtual ~Value() = default;

    // Get the type of this value.
    TypePtr GetType() const { return _type; }
    ValueType GetValueType() const { return _valueType; }

    // Cast this type to a specific type.
    // You should use this function only when you are sure that this type is
    // actually the type you want to cast to.
    template<typename _Ty>
    _Ty* Cast() { return static_cast<_Ty*>(this); }

private:
    using _use_iterator_raw = std::vector<UsePtr>::iterator;

    class use_base_iterator_base
    {
    public:
        use_base_iterator_base(_use_iterator_raw iter) : _iter(iter) {}

        bool operator==(const use_base_iterator_base& rhs) const
        {
            return _iter == rhs._iter;
        }

        bool operator!=(const use_base_iterator_base& rhs) const
        {
            return _iter != rhs._iter;
        }

        // Prefix increment
        use_base_iterator_base& operator++()
        {
            ++_iter;
            return *this;
        }

        // Postfix increment
        use_base_iterator_base operator++(int)
        {
            use_base_iterator_base tmp = *this;
            ++_iter;
            return tmp;
        }

    protected:
        _use_iterator_raw _iter;
    };

    class use_iterator_impl : use_base_iterator_base
    {
    public:
        use_iterator_impl(_use_iterator_raw iter) : use_base_iterator_base(iter) {}

        UsePtr& operator*() const
        {
            return *_iter;
        }

        UsePtr* operator->() const
        {
            return &*_iter;
        }
    };

    class user_iterator_impl : use_base_iterator_base
    {
    public:
        user_iterator_impl(_use_iterator_raw iter) : use_base_iterator_base(iter) {}

        UserPtr operator*() const
        {
            return (*_iter)->GetUser();
        }

        UserPtr operator->() const
        {
            return operator*();
        }
    };

public:
    // Just... no const friend. :(
    using use_iterator = use_iterator_impl;
    using user_iterator = user_iterator_impl;

    use_iterator UseBegin() { return use_iterator(_useList.begin()); }
    use_iterator UseEnd() { return use_iterator(_useList.end()); }

    user_iterator UserBegin() { return user_iterator(_useList.begin()); }
    user_iterator UserEnd() { return user_iterator(_useList.end()); }

protected:
    TypePtr _type;
    std::vector<UsePtr> _useList;

private:
    ValueType _valueType;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_VALUE_H_
