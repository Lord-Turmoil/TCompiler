/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_IR_INSTRUCTIONS_H_
#define _TOMIC_LLVM_IR_INSTRUCTIONS_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/value/inst/Instruction.h>

TOMIC_LLVM_BEGIN

class AllocaInst final : public Instruction
{
public:
private:
    AllocaInst(TypePtr type, int alignment);

    int _alignment;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_IR_INSTRUCTIONS_H_
