/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_IR_INSTRUCTION_TYPES_H_
#define _TOMIC_LLVM_IR_INSTRUCTION_TYPES_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/IrForward.h>
#include <tomic/llvm/ir/value/inst/Instruction.h>

TOMIC_LLVM_BEGIN

class UnaryInstruction : public Instruction
{
public:
    ~UnaryInstruction() override = default;

    ValuePtr Operand() const { return OperandAt(0); }

protected:
    UnaryInstruction(ValueType valueType, TypePtr type, ValuePtr operand);
};


enum class UnaryOpType
{
    Not,
    Neg
};

class UnaryOperator final : public UnaryInstruction
{
public:
    static UnaryOperatorPtr New(UnaryOpType opType, ValuePtr operand);

private:
    UnaryOperator(TypePtr type, ValuePtr operand, UnaryOpType opType);

    UnaryOpType _opType;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_IR_INSTRUCTION_TYPES_H_
