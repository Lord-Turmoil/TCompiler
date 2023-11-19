/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_GLOBAL_VARIABLE_H_
#define _TOMIC_LLVM_GLOBAL_VARIABLE_H_

#include <string>
#include <tomic/llvm/ir/value/ConstantData.h>
#include <tomic/llvm/ir/value/GlobalValue.h>
#include <tomic/llvm/Llvm.h>

TOMIC_LLVM_BEGIN

class GlobalVariable : public GlobalValue
{
public:
    ~GlobalVariable() override = default;

    static GlobalVariablePtr New(TypePtr type, bool isConstant, const std::string& name);
    static GlobalVariablePtr New(TypePtr type, bool isConstant, const std::string& name,
                                 ConstantDataPtr initializer);

    static bool classof(const ValueType type) { return type == ValueType::GlobalVariableTy; }

    void PrintAsm(IAsmWriterPtr writer) override;

public:
    bool IsConstant() const { return _isConstant; }

private:
    GlobalVariable(TypePtr type, bool isConstant, const std::string& name);
    GlobalVariable(TypePtr type, bool isConstant, const std::string& name, ConstantDataPtr initializer);

    bool _isConstant;
    ConstantDataPtr _initializer;
};


/*
 * It is used for private string literals used in the program.
 */
class GlobalString : public GlobalValue
{
public:
    ~GlobalString() override = default;

    static GlobalStringPtr New(const std::string& value, const std::string& name);

    static bool classof(const ValueType type) { return type == ValueType::GlobalStringTy; }

    void PrintAsm(IAsmWriterPtr writer) override;
    void PrintUse(IAsmWriterPtr writer) override;

private:
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_GLOBAL_VARIABLE_H_
