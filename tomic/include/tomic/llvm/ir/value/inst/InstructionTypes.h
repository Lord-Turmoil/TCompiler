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

/*
 * ============================== Unary Instruction =============================
 */

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
    Neg,
    Pos
};


/*
 * ============================== Unary Operator ===============================
 */

class UnaryOperator final : public UnaryInstruction
{
public:
    ~UnaryOperator() override = default;

    static UnaryOperatorPtr New(UnaryOpType opType, ValuePtr operand);

    void PrintAsm(IAsmWriterPtr writer) override;

    UnaryOpType OpType() const { return _opType; }

private:
    UnaryOperator(TypePtr type, ValuePtr operand, UnaryOpType opType);

    UnaryOpType _opType;
};


/*
 * ============================== Binary Instruction ============================
 */

enum class BinaryOpType
{
    Add,
    Sub,
    Mul,
    Div,
    Mod
};


class BinaryOperator final : public Instruction
{
public:
    ~BinaryOperator() override = default;

    static BinaryOperatorPtr New(BinaryOpType opType, ValuePtr lhs, ValuePtr rhs);

    void PrintAsm(IAsmWriterPtr writer) override;

    BinaryOpType OpType() const { return _opType; }

    ValuePtr LeftOperand() const { return OperandAt(0); }
    ValuePtr RightOperand() const { return OperandAt(1); }

private:
    BinaryOperator(TypePtr type, ValuePtr lhs, ValuePtr rhs, BinaryOpType opType);

    BinaryOpType _opType;
};


/*
 * ============================== Compare Instruction ==============================
 */

enum class PredicateType
{
    Equal,
    NotEqual,
    GreaterThan,
    GreaterThanOrEqual,
    LessThan,
    LessThanOrEqual
};


class CompareInstruction final : public Instruction
{
public:
    ~CompareInstruction() override = default;

    static CompareInstructionPtr New(PredicateType predicateType, ValuePtr lhs, ValuePtr rhs);

    PredicateType GetPredicateType() const { return _predicateType; }

    ValuePtr LeftOperand() const { return OperandAt(0); }
    ValuePtr RightOperand() const { return OperandAt(1); }

protected:
    CompareInstruction(TypePtr type, ValuePtr lhs, ValuePtr rhs, PredicateType predicateType);

    PredicateType _predicateType;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_IR_INSTRUCTION_TYPES_H_
