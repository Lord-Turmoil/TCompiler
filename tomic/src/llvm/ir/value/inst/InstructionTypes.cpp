/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/value/inst/InstructionTypes.h>

TOMIC_LLVM_BEGIN

/*
 * ============================== Unary Instruction ==============================
 */
//

UnaryInstruction::UnaryInstruction(ValueType valueType, TypePtr type, ValuePtr operand)
    : Instruction(valueType, type), _operand(operand)
{
    AddOperand(operand);
}


/*
 * ============================== Unary Operator ==============================
 */

UnaryOperator::UnaryOperator(TypePtr type, ValuePtr operand, UnaryOpType opType)
    : UnaryInstruction(ValueType::UnaryOperatorTy, type, operand),
      _opType(opType)
{
}


UnaryOperatorPtr UnaryOperator::New(UnaryOpType opType, ValuePtr operand)
{
    auto type = operand->GetType();
    auto inst = std::shared_ptr<UnaryOperator>(new UnaryOperator(type, operand, opType));

    operand->Context()->StoreValue(inst);

    return inst.get();
}


/*
 * ============================== Binary Operator ==============================
 */

BinaryOperator::BinaryOperator(TypePtr type, ValuePtr lhs, ValuePtr rhs, BinaryOpType opType)
    : Instruction(ValueType::BinaryOperatorTy, type),
      _opType(opType),
      _leftOperand(lhs),
      _rightOperand(rhs)
{
    AddOperand(lhs);
    AddOperand(rhs);
}


BinaryOperatorPtr BinaryOperator::New(BinaryOpType opType, ValuePtr lhs, ValuePtr rhs)
{
    auto type = lhs->GetType();
    auto inst = std::shared_ptr<BinaryOperator>(new BinaryOperator(type, lhs, rhs, opType));

    lhs->Context()->StoreValue(inst);

    return inst.get();
}


/*
 * ============================== Compare Instruction ==============================
 */

CompareInstruction::CompareInstruction(TypePtr type, ValuePtr lhs, ValuePtr rhs, PredicateType predicateType)
    : Instruction(ValueType::CompareInstTy, type), _predicateType(predicateType)
{
    AddOperand(lhs);
    AddOperand(rhs);
}


CompareInstructionPtr CompareInstruction::New(PredicateType predicateType, ValuePtr lhs, ValuePtr rhs)
{
    auto type = lhs->GetType();
    auto inst = std::shared_ptr<CompareInstruction>(new CompareInstruction(type, lhs, rhs, predicateType));

    lhs->Context()->StoreValue(inst);

    return inst.get();
}


TOMIC_LLVM_END
