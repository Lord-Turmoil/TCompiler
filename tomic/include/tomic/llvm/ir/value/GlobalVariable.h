/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_GLOBAL_VARIABLE_H_
#define _TOMIC_LLVM_GLOBAL_VARIABLE_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/value/GlobalValue.h>

TOMIC_LLVM_BEGIN

class GlobalVariable : public GlobalValue
{
public:
    ~GlobalVariable() override = default;

    static GlobalVariablePtr New(TypePtr type, bool isConstant, const std::string& name);
    static GlobalVariablePtr New(TypePtr type, bool isConstant, const std::string& name, ConstantPtr initializer);

public:
    bool IsConstant() const { return _isConstant; }

private:
    GlobalVariable(TypePtr type, bool isConstant, const std::string& name);
    GlobalVariable(TypePtr type, bool isConstant, const std::string& name, ConstantPtr initializer);

    bool _isConstant;
    ConstantPtr _initializer;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_GLOBAL_VARIABLE_H_
