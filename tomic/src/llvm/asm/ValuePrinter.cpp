/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/DerivedTypes.h>
#include <tomic/llvm/ir/value/inst/Instructions.h>
#include <tomic/llvm/ir/value/inst/InstructionTypes.h>
#include <tomic/llvm/ir/value/ConstantData.h>
#include <tomic/llvm/ir/value/GlobalVariable.h>
#include <tomic/llvm/ir/value/Value.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/value/Argument.h>
#include <tomic/llvm/ir/value/BasicBlock.h>
#include <tomic/utils/StringUtil.h>

TOMIC_LLVM_BEGIN

/*
 * ============================== Value ==============================
 */
//
void Value::PrintAsm(IAsmWriterPtr writer)
{
    TOMIC_PANIC("Not implemented!");
}


void Value::PrintUse(IAsmWriterPtr writer)
{
    TOMIC_PANIC("Not implemented!");
}


void Value::PrintName(IAsmWriterPtr writer)
{
    TOMIC_PANIC("Not implemented!");
}


/*
 * ============================ Constant =============================
 */

/*
 * ConstantData is a compile time constant, so its name and asm are
 * the same. Only one exception, is 'zero initializer', which is used
 * only in PrintAsm.
 */

// {type} {value}
void ConstantData::PrintAsm(IAsmWriterPtr writer)
{
    GetType()->PrintAsm(writer);
    if (IsArray())
    {
        if (IsAllZero())
        {
            writer->PushNext("zeroinitializer");
        }
        else
        {
            writer->PushNext('[');
            for (auto valueIter = _values.begin(); valueIter != _values.end(); ++valueIter)
            {
                if (valueIter != _values.begin())
                {
                    writer->Push(", ");
                }
                (*valueIter)->PrintAsm(writer);
            }
            writer->Push(']');
        }
    }
    else
    {
        writer->PushNext(StringUtil::IntToString(_value));
    }
}

void ConstantData::PrintName(IAsmWriterPtr writer)
{
    if (IsArray())
    {
        writer->Push('[');
        for (auto valueIter = _values.begin(); valueIter != _values.end(); ++valueIter)
        {
            if (valueIter != _values.begin())
            {
                writer->Push(", ");
            }
            (*valueIter)->PrintAsm(writer);
        }
        writer->Push(']');
    }
    else
    {
        writer->Push(StringUtil::IntToString(_value));
    }
}

void ConstantData::PrintUse(IAsmWriterPtr writer)
{
    GetType()->PrintAsm(writer);
    writer->PushSpace();
    PrintName(writer);
}


/*
 * =========================== GlobalValue ===========================
 */

void GlobalValue::PrintName(IAsmWriterPtr writer)
{
    writer->Push('@');
    writer->Push(GetName());
}


void GlobalValue::PrintUse(IAsmWriterPtr writer)
{
    GetType()->PrintAsm(writer);
    writer->PushSpace();
    PrintName(writer);
}


void GlobalVariable::PrintAsm(IAsmWriterPtr writer)
{
    // Name
    PrintName(writer);

    writer->PushNext('=');

    // Attribute
    writer->PushNext("dso_local");
    writer->PushNext(IsConstant() ? "constant" : "global");
    writer->PushSpace();

    // Initializer
    if (_initializer)
    {
        _initializer->PrintAsm(writer);
    }
    else
    {
        GetType()->PrintAsm(writer);
        if (GetType()->IsArrayTy())
        {
            writer->PushNext("zeroinitializer");
        }
        else
        {
            writer->PushNext('0');
        }
    }

    writer->PushNewLine();
}


/*
 * ============================ Function =============================
 */

/*
 * ; {function type}
 * define dso_local {type} @{name}({type} %{0}, {type} %{1}, ...) {
 *     // basic blocks
 *     // instructions
 * }
 */
void Function::PrintAsm(IAsmWriterPtr writer)
{
    auto rawType = GetType();
    TOMIC_ASSERT(rawType->IsFunctionTy());
    auto type = rawType->Cast<FunctionType>();

    // First, we trace all the slot.
    GetSlotTracker()->Trace(this);

    // Blank line.
    writer->PushNewLine();

    // For debug purpose, we print some comments.
    writer->CommentBegin();
    writer->Push("Function type: ");
    type->PrintAsm(writer);
    writer->CommentEnd();

    // Function header.
    writer->Push("define dso_local ");
    type->ReturnType()->PrintAsm(writer);

    // Function name.
    writer->PushNext('@');
    writer->Push(GetName());

    // Function parameters.
    writer->Push('(');
    for (auto argIter = ArgBegin(); argIter != ArgEnd(); ++argIter)
    {
        auto arg = *argIter;
        if (argIter != ArgBegin())
        {
            writer->Push(", ");
        }
        arg->GetType()->PrintAsm(writer);

        writer->PushNext('%');
        writer->Push(GetSlotTracker()->Slot(arg));
    }
    writer->Push(')');

    // Function body.
    writer->PushNext('{');
    writer->PushNewLine();

    // Basic blocks.
    for (auto blockIter = BasicBlockBegin(); blockIter != BasicBlockEnd(); ++blockIter)
    {
        (*blockIter)->PrintAsm(writer);
    }

    // End of function.
    writer->Push('}');
}


/*
 * ============================ Argument =============================
 */

// type name
void Argument::PrintAsm(IAsmWriterPtr writer)
{
    FunctionPtr parent = Parent();

    GetType()->PrintAsm(writer);
    writer->PushNext('%');
    writer->Push(StringUtil::IntToString(parent->GetSlotTracker()->Slot(this)));
}


void Argument::PrintUse(IAsmWriterPtr writer)
{
    PrintAsm(writer);
}


/*
 * =========================== BasicBlock ============================
 */

// label:
// Or we shouldn't print the first label?
void BasicBlock::PrintAsm(IAsmWriterPtr writer)
{
    auto func = Parent();

    if (this != *func->BasicBlockBegin())
    {
        writer->Push(StringUtil::IntToString(func->GetSlotTracker()->Slot(this)));
        writer->Push(':');
        writer->PushNewLine();
    }

    for (auto instIter = InstructionBegin(); instIter != InstructionEnd(); ++instIter)
    {
        writer->PushSpaces(4);
        (*instIter)->PrintAsm(writer);
    }

}


void BasicBlock::PrintUse(IAsmWriterPtr writer)
{
    GetType()->PrintAsm(writer);
    writer->PushSpace();
    PrintName(writer);
}


void BasicBlock::PrintName(IAsmWriterPtr writer)
{
    writer->Push('%');
    writer->Push(StringUtil::IntToString(Parent()->GetSlotTracker()->Slot(this)));
}


/*
 * =========================== Instruction ===========================
 */

void Instruction::PrintName(IAsmWriterPtr writer)
{
    TOMIC_ASSERT(!(GetType()->IsVoidTy()));

    writer->Push('%');
    writer->Push(StringUtil::IntToString(ParentFunction()->GetSlotTracker()->Slot(this)));
}


void Instruction::PrintUse(IAsmWriterPtr writer)
{
    TOMIC_ASSERT(!(GetType()->IsVoidTy()));

    GetType()->PrintAsm(writer);
    writer->PushNext('%');
    writer->Push(StringUtil::IntToString(ParentFunction()->GetSlotTracker()->Slot(this)));
}


// %1 = alloca i32[, align 4]
void AllocaInst::PrintAsm(IAsmWriterPtr writer)
{
    PrintName(writer);

    writer->PushNext('=');
    writer->PushNext("alloca");
    writer->PushSpace();
    AllocatedType()->PrintAsm(writer);
    writer->PushNewLine();
}


// store i32 1, i32* %3[, align 4]
void StoreInst::PrintAsm(IAsmWriterPtr writer)
{
    writer->Push("store");
    writer->PushSpace();

    OperandAt(0)->PrintUse(writer);
    writer->Push(", ");
    OperandAt(1)->PrintUse(writer);

    writer->PushNewLine();
}


// %3 = load i32, i32* %1, align 4
void LoadInst::PrintAsm(IAsmWriterPtr writer)
{
    PrintName(writer);

    writer->PushNext("= load ");
    GetType()->PrintAsm(writer);
    writer->Push(", ");

    Address()->PrintUse(writer);

    writer->PushNewLine();
}


/*
 * ========================== ReturnInst =============================
 */

void ReturnInst::PrintAsm(IAsmWriterPtr writer)
{
    writer->Push("ret");
    if (OperandsCount() > 0)
    {
        writer->PushSpace();
        OperandAt(0)->PrintUse(writer);
    }
    writer->PushNewLine();
}


/*
 * ========================= OperatorInst ============================
 */

/*
 * %11 = add nsw i32 %9, %10
 * %18 = sub nsw i32 0, %7
 * %13 = mul nsw i32 %12, 2
 * %15 = sdiv i32 %14, 2
 * %17 = srem i32 %16, 2
 */
void BinaryOperator::PrintAsm(IAsmWriterPtr writer)
{
    const char* op;
    switch (OpType())
    {
    case BinaryOpType::Add:
        op = "add nsw";
        break;
    case BinaryOpType::Sub:
        op = "sub nsw";
        break;
    case BinaryOpType::Mul:
        op = "mul nsw";
        break;
    case BinaryOpType::Div:
        op = "sdiv";
        break;
    case BinaryOpType::Mod:
        op = "srem";
        break;
    }

    PrintName(writer);
    writer->PushNext("=");
    writer->PushNext(op);
    writer->PushSpace();

    // Only one type.
    GetType()->PrintAsm(writer);
    writer->PushSpace();

    LeftOperand()->PrintName(writer);
    writer->Push(", ");
    RightOperand()->PrintName(writer);

    writer->PushNewLine();
}

/*
 * %2 = add nsw i32 0, %1
 * %2 = sub nsw i32 0, %1
 */
void UnaryOperator::PrintAsm(IAsmWriterPtr writer)
{
    const char* op;

    switch (OpType())
    {
    case UnaryOpType::Pos:
        op = "add nsw";
        break;
    case UnaryOpType::Neg:
        op = "sub nsw";
        break;
    default:
        TOMIC_PANIC("Not supported");
        return;
    }

    PrintName(writer);
    writer->PushNext("=");
    writer->PushNext(op);
    writer->PushSpace();

    GetType()->PrintAsm(writer);
    writer->PushNext('0');
    writer->Push(", ");
    Operand()->PrintName(writer);

    writer->PushNewLine();
}


TOMIC_LLVM_END
