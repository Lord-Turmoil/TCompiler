/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * For all PrintAsm function of types.
 * Note that it will not care for spaces around it.
 * Whether a constant is included in GlobalVariable, not in type.
 */

#include <tomic/llvm/ir/DerivedTypes.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/utils/StringUtil.h>

#include <string>

TOMIC_LLVM_BEGIN

/*
 * No need to care about IntegerType or else, since they will
 * override this function.
 */
// void
void Type::PrintAsm(IAsmWriterPtr writer)
{
    switch (_typeId)
    {
    case VoidTyID:
        writer->Push("void");
        break;
    case LabelTyID:
        writer->Push("label");
        break;
    default:
        TOMIC_PANIC("Should not reach here!");
    }
}


// i32
void IntegerType::PrintAsm(IAsmWriterPtr writer)
{
    writer->Push('i');
    writer->Push(StringUtil::IntToString(BitWidth()));
}


// return type ( arg1, arg2, ... )
void FunctionType::PrintAsm(IAsmWriterPtr writer)
{
    ReturnType()->PrintAsm(writer);
    writer->PushNext("(");
    for (auto argIter = ParamBegin(); argIter != ParamEnd(); ++argIter)
    {
        if (argIter != ParamBegin())
        {
            writer->Push(", ");
        }
        (*argIter)->PrintAsm(writer);
    }
    writer->Push(')');
}


// [ n x type ]
void ArrayType::PrintAsm(IAsmWriterPtr writer)
{
    writer->Push('[');
    writer->Push(StringUtil::IntToString(ElementCount()));
    writer->PushNext('x');
    writer->PushSpace();
    ElementType()->PrintAsm(writer);
    writer->Push("]");
}


// type*
void PointerType::PrintAsm(IAsmWriterPtr writer)
{
    ElementType()->PrintAsm(writer);
    writer->Push('*');
}


TOMIC_LLVM_END
