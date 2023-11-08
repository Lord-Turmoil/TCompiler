/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/inst/InstructionTypes.h>
#include <tomic/llvm/ir/LlvmContext.h>

TOMIC_LLVM_BEGIN

/*
 * ============================== Unary Instruction ==============================
 */

UnaryInstruction::UnaryInstruction(ValueType valueType, TypePtr type, ValuePtr operand)
        : Instruction(valueType, type)
{
    AddOperand(operand);
}


/*
 * ============================== Unary Operator ==============================
 */

UnaryOperator::UnaryOperator(TypePtr type, ValuePtr operand, UnaryOpType opType)
        : UnaryInstruction(ValueType::UnaryOperatorTy, type, operand), _opType(opType)
{
}

UnaryOperatorPtr UnaryOperator::New(UnaryOpType opType, ValuePtr operand)
{
    auto type = operand->GetType();
    auto inst = std::shared_ptr<UnaryOperator>(new UnaryOperator(type, operand, opType));

    operand->Context()->StoreValue(inst);

    return inst.get();
}

TOMIC_LLVM_END
