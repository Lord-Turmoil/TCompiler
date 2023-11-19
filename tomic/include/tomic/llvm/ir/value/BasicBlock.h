/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_BASIC_BLOCK_H_
#define _TOMIC_LLVM_BASIC_BLOCK_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/value/Value.h>
#include <tomic/llvm/ir/IrForward.h>
#include <list>

TOMIC_LLVM_BEGIN

class BasicBlock final : public Value
{
public:
    ~BasicBlock() override = default;

    static BasicBlockPtr New(FunctionPtr parent = nullptr);

    static bool classof(const ValueType type) { return type == ValueType::BasicBlockTy; }

    void PrintAsm(IAsmWriterPtr writer) override;
    void PrintUse(IAsmWriterPtr writer) override;
    void PrintName(IAsmWriterPtr writer) override;

    FunctionPtr Parent() const { return _parent; }
    void SetParent(FunctionPtr parent) { _parent = parent; }

public:
    using instruction_iterator = std::list<InstructionPtr>::iterator;
    using instruction_reverse_iterator = std::list<InstructionPtr>::reverse_iterator;

    int InstructionCount() const { return _instructions.size(); }
    instruction_iterator InstructionBegin() { return _instructions.begin(); }
    instruction_iterator InstructionEnd() { return _instructions.end(); }
    instruction_reverse_iterator InstructionRBegin() { return _instructions.rbegin(); }
    instruction_reverse_iterator InstructionREnd() { return _instructions.rend(); }
    InstructionPtr FirstInstruction() { return _instructions.front(); }
    InstructionPtr LastInstruction() { return _instructions.back(); }

    // Insert an instruction at the end of the basic block.
    BasicBlockPtr InsertInstruction(InstructionPtr inst);
    // Insert an instruction before the specified iterator.
    BasicBlockPtr InsertInstruction(instruction_iterator iter, InstructionPtr inst);
    // Remove an instruction from the basic block.
    BasicBlockPtr RemoveInstruction(InstructionPtr inst);

private:
    BasicBlock(FunctionPtr parent);

    FunctionPtr _parent;

    std::list<InstructionPtr> _instructions;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_BASIC_BLOCK_H_
