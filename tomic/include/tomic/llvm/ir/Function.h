/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_FUNCTION_H_
#define _TOMIC_LLVM_FUNCTION_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/GlobalValue.h>
#include <tomic/llvm/ir/Argument.h>
#include <vector>
#include <list>

TOMIC_LLVM_BEGIN

class Function final : public GlobalValue
{
public:
    using block_iterator = std::list<BasicBlockPtr>::iterator;

    ~Function() override = default;

    static FunctionPtr New(TypePtr type, const std::string& name, UseListPtr operands);

    int ArgCount() const { return _args.size(); }
    ArgumentPtr GetArg(int argNo) const { return _args[argNo]; }

    int BasicBlockCount() const { return _basicBlocks.size(); }
    block_iterator BasicBlockBegin() { return _basicBlocks.begin(); }
    block_iterator BasicBlockEnd() { return _basicBlocks.end(); }

    // Insert a basic block at the end of the function.
    FunctionPtr InsertBasicBlock(BasicBlockPtr block);
    // Insert a basic block before the specified iterator.
    FunctionPtr InsertBasicBlock(block_iterator iter, BasicBlockPtr block);
    // Remove a basic block from the function.
    FunctionPtr RemoveBasicBlock(BasicBlockPtr block);

private:
    Function(TypePtr type, const std::string& name, UseListPtr operands);

private:
    // We can generate arguments via its type.
    std::vector<ArgumentPtr> _args;
    std::list<BasicBlockPtr> _basicBlocks;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_FUNCTION_H_
