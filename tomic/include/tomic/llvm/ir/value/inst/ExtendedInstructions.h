/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Extended instruction are instructions that are not defined in LLVM IR.
 * They are used to represent some special instructions in the source code.
 * For example, input and output instructions.
 */

#ifndef _TOMIC_LLVM_EXTENDED_INSTRUCTIONS_H_
#define _TOMIC_LLVM_EXTENDED_INSTRUCTIONS_H_

#include <tomic/llvm/ir/IrForward.h>
#include <tomic/llvm/ir/value/inst/Instruction.h>
#include <tomic/llvm/ir/value/inst/InstructionTypes.h>

TOMIC_LLVM_BEGIN

/*
 * ============================== InputInst ==============================
 */
//
class InputInst final : public Instruction
{
public:
    ~InputInst() override = default;

    static InputInstPtr New(LlvmContextPtr context);

    static bool classof(const ValueType type) { return type == ValueType::InputInstTy; }

    void PrintAsm(IAsmWriterPtr writer) override;
    bool IsInput() const override { return true; }

private:
    InputInst(TypePtr type);
};


/*
 * ============================== OutputInst ==============================
 */

/*
 * Output instruction do need one and only one parameter, so it is a unary
 * instruction. :P
 */
class OutputInst final : public UnaryInstruction
{
public:
    ~OutputInst() override = default;

    static OutputInstPtr New(ValuePtr value);

    static bool classof(const ValueType type) { return type == ValueType::OutputInstTy; }

    void PrintAsm(IAsmWriterPtr writer) override;
    bool IsOutput() const override { return true; }

    ValuePtr GetValue() const { return _value; }

    // A utility function to test if it output integer or string..
    bool IsInteger() const;

private:
    OutputInst(ValuePtr value);

    /*
     * It can be an integer, or an anonymous string.
     */
    ValuePtr _value;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_EXTENDED_INSTRUCTIONS_H_
