/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_FUNCTION_H_
#define _TOMIC_LLVM_FUNCTION_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/value/GlobalValue.h>
#include <tomic/llvm/ir/value/Argument.h>
#include <tomic/llvm/ir/SlotTracker.h>
#include <vector>
#include <list>

TOMIC_LLVM_BEGIN

/*
 * The structure of LLVM IR is like this:
 *
 * Module
 *  |- Function
 *  \- Function
 *      |- Parameter
 *      |- Parameter
 *      |- BasicBlock
 *      |   |- Instruction
 *      |   |- Instruction
 *      |   \- Instruction
 *      \- BasicBlock
 *          \- Instruction
 */
//
class Function final : public GlobalValue
{
public:
    using block_iterator = std::list<BasicBlockPtr>::iterator;
    using block_reverse_iterator = std::list<BasicBlockPtr>::reverse_iterator;
    using argument_iterator = std::vector<ArgumentPtr>::iterator;

    ~Function() override = default;

    static FunctionPtr New(TypePtr returnType, const std::string& name);
    static FunctionPtr New(TypePtr returnType, const std::string& name, std::vector<ArgumentPtr> args);

    static bool classof(const ValueType type)
    {
        return type == ValueType::FunctionTy;
    }

    BasicBlockPtr NewBasicBlock();

    void PrintAsm(IAsmWriterPtr writer) override;

    bool IsFunction() const override { return true; }

    TypePtr ReturnType() const;

public:
    int ArgCount() const { return _args.size(); }
    ArgumentPtr GetArg(int argNo) const { return _args[argNo]; }
    argument_iterator ArgBegin() { return _args.begin(); }
    argument_iterator ArgEnd() { return _args.end(); }

    int BasicBlockCount() const { return _basicBlocks.size(); }
    block_iterator BasicBlockBegin() { return _basicBlocks.begin(); }
    block_iterator BasicBlockEnd() { return _basicBlocks.end(); }
    block_reverse_iterator BasicBlockRBegin() { return _basicBlocks.rbegin(); }
    block_reverse_iterator BasicBlockREnd() { return _basicBlocks.rend(); }
    BasicBlockPtr FirstBasicBlock() { return _basicBlocks.front(); }
    BasicBlockPtr LastBasicBlock() { return _basicBlocks.back(); }

    // Insert a basic block at the end of the function.
    FunctionPtr InsertBasicBlock(BasicBlockPtr block);
    // Insert a basic block before the specified iterator.
    FunctionPtr InsertBasicBlock(block_iterator iter, BasicBlockPtr block);
    // Remove a basic block from the function.
    FunctionPtr RemoveBasicBlock(BasicBlockPtr block);

    // Get the slot tracker of this function.
    SlotTrackerPtr GetSlotTracker() { return &_slotTracker; }

private:
    Function(TypePtr type, const std::string& name);
    Function(TypePtr type, const std::string& name, std::vector<ArgumentPtr> args);

private:
    // We can generate arguments via its type.
    std::vector<ArgumentPtr> _args;
    std::list<BasicBlockPtr> _basicBlocks;

    // Slot tracker is used to track all IDs of LLVM value.
    SlotTracker _slotTracker;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_FUNCTION_H_
