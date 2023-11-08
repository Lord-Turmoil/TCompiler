/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_INITIALIZER_H_
#define _TOMIC_LLVM_INITIALIZER_H_

#include "tomic/llvm/Llvm.h"
#include "IrForward.h"
#include <memory>
#include <vector>

TOMIC_LLVM_BEGIN

/*
 * In LLVM, it seems that initializer is a special kind of constant, which can be
 * "mysteriously" cast into constant data...
 * Here, I use my own implementation of an initializer list.
 * An initializer can be an initializer list or a simple integer.
 */

class Initializer;
using InitializerPtr = std::shared_ptr<Initializer>;

class Initializer
{
public:
    static InitializerPtr New(TypePtr type, int value);
    static InitializerPtr New(std::vector<InitializerPtr> values);

    TypePtr GetType() const { return _type; }
    bool IsAllZero() const { return _isAllZero; }

private:
    // We will generate types automatically based on which constructor is called.
    Initializer(TypePtr type, int value);
    Initializer(std::vector<InitializerPtr> values);

    TypePtr _type;
    bool _isAllZero;
    int _value;
    std::vector<InitializerPtr> _values;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_INITIALIZER_H_
