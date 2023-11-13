/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/inst/Instruction.h>
#include <tomic/llvm/ir/value/BasicBlock.h>
#include <tomic/llvm/ir/value/Function.h>

TOMIC_LLVM_BEGIN


Instruction::Instruction(ValueType valueType, TypePtr type, UseListPtr operands)
        : User(valueType, type, operands)
{
}

Instruction::Instruction(ValueType valueType, TypePtr type)
        : User(valueType, type)
{
}

FunctionPtr Instruction::ParentFunction() const
{
    return Parent()->Parent();
}

ModulePtr Instruction::ParentModule() const
{
    return ParentFunction()->Parent();
}

TOMIC_LLVM_END
