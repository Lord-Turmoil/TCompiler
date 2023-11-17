/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_IR_CONSTANT_DATA_H_
#define _TOMIC_LLVM_IR_CONSTANT_DATA_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/value/Constant.h>

TOMIC_LLVM_BEGIN

class ConstantData : public Constant
{
public:
    ~ConstantData() override = default;

    static ConstantDataPtr New(TypePtr type, int value);
    static ConstantDataPtr New(std::vector<ConstantDataPtr> values);

    // ConstantData has no name.
    void PrintAsm(IAsmWriterPtr writer) override;
    void PrintUse(IAsmWriterPtr writer) override;

    bool IsAllZero() const { return _isAllZero; }
    bool IsArray() const { return !_values.empty(); }
    int IntValue() const { return _value; }

private:
    ConstantData(TypePtr type, int value);
    ConstantData(TypePtr type, std::vector<ConstantDataPtr> values);

    bool _isAllZero;
    int _value;
    std::vector<ConstantDataPtr> _values;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_IR_CONSTANT_DATA_H_
