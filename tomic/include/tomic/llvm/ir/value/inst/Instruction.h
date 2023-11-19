/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_INSTRUCTION_H_
#define _TOMIC_LLVM_INSTRUCTION_H_

#include <tomic/llvm/ir/value/User.h>

TOMIC_LLVM_BEGIN

class Instruction : public User
{
public:
    ~Instruction() override = default;

    void PrintName(IAsmWriterPtr writer) override;
    void PrintUse(IAsmWriterPtr writer) override;

    bool IsInstruction() const override { return true; }

    BasicBlockPtr Parent() const { return _parent; }
    void SetParent(BasicBlockPtr parent) { _parent = parent; }

    FunctionPtr ParentFunction() const;
    ModulePtr ParentModule() const;

    virtual bool IsReturn() const { return false; }
    virtual bool IsAlloca() const { return false; }
    virtual bool IsLoad() const { return false; }
    virtual bool IsStore() const { return false; }
    virtual bool IsBinaryOperator() const { return false; }
    virtual bool IsUnaryOperator() const { return false; }
    virtual bool IsCompare() const { return false; }

protected:
    Instruction(ValueType valueType, TypePtr type, UseListPtr operands);
    Instruction(ValueType valueType, TypePtr type);

private:
    BasicBlockPtr _parent;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_INSTRUCTION_H_
