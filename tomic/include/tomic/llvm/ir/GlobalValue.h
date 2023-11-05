/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_GLOBAL_VALUE_H_
#define _TOMIC_LLVM_GLOBAL_VALUE_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/Constant.h>
#include "ValueType.h"

TOMIC_LLVM_BEGIN

class GlobalValue : public Constant
{
public:
    ~GlobalValue() override = default;

protected:
    GlobalValue(ValueType valueType, TypePtr type, const std::string& name, UseListPtr operands)
            : Constant(valueType, type, operands)
    {
        SetName(name);
    };
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_GLOBAL_VALUE_H_
