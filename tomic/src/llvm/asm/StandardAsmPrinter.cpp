/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/asm/impl/StandardAsmPrinter.h>
#include <tomic/llvm/asm/impl/StandardAsmWriter.h>
#include <tomic/llvm/ir/Module.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/value/GlobalVariable.h>

TOMIC_LLVM_BEGIN

void StandardAsmPrinter::Print(ModulePtr module, twio::IWriterPtr writer)
{
    auto asmWriter = StandardAsmWriter::New(writer);

    _PrintHeader(asmWriter);
    _PrintModule(asmWriter, module);
    _PrintFooter(asmWriter);
}


/*
 * ; Tony's Mini Compiler (ToMiC) [Version {version}]
 * ; (C) Copyright Tony's Studio 2018 - {2023. All rights reserved.
 * ;
 * ; LLVM IR Version: {llvm-version}
 * (blank line)
 */
void StandardAsmPrinter::_PrintHeader(IAsmWriterPtr writer)
{
    writer->CommentBegin();
    writer->Push("Tony's Mini Compiler (ToMiC) [Version ");
    writer->Push(TOMIC_VERSION);
    writer->Push("]");
    writer->CommentEnd();

    writer->CommentBegin();
    writer->Push("(C) Tony's Studio 2018 - 2023. All rights reserved.");
    writer->CommentEnd();

    writer->PushComment("");

    writer->CommentBegin();
    writer->Push("LLVM IR Version: ");
    writer->Push(LLVM_VERSION);
    writer->CommentEnd();

    writer->PushNewLine();
}


/*
 * ; End of LLVM IR
 */
void StandardAsmPrinter::_PrintFooter(IAsmWriterPtr writer)
{
    writer->PushNewLine();
    writer->PushComment("End of LLVM IR");
}


/*
 * ; LLVM IR Module: {module-name}
 * (blank line)
 * {global variables}
 * (blank line)
 * {functions}
 */
void StandardAsmPrinter::_PrintModule(IAsmWriterPtr writer, ModulePtr module)
{
    // Module name.
    writer->CommentBegin();
    writer->Push("Module ID = ");
    writer->Push('\'');
    writer->Push(module->Name());
    writer->Push('\'');
    writer->CommentEnd();

    writer->Push("source_filename = ");
    writer->Push('\"');
    writer->Push(module->Name());
    writer->Push('\"');
    writer->PushNewLine();
    writer->PushNewLine();

    // Library function declaration.
    _PrintDeclaration(writer);

    // Global variables.
    for (auto globalIter = module->GlobalBegin(); globalIter != module->GlobalEnd(); ++globalIter)
    {
        _PrintGlobalVariable(writer, *globalIter);
    }

    // Functions.
    for (auto funcIter = module->FunctionBegin(); funcIter != module->FunctionEnd(); ++funcIter)
    {
        _PrintFunction(writer, *funcIter);
    }

    // Main function.
    if (module->GetMainFunction())
    {
        _PrintFunction(writer, module->GetMainFunction());
    }

    writer->PushNewLine();
}


void StandardAsmPrinter::_PrintGlobalVariable(IAsmWriterPtr writer, GlobalVariablePtr globalVariable)
{
    globalVariable->PrintAsm(writer);
}


void StandardAsmPrinter::_PrintFunction(IAsmWriterPtr writer, FunctionPtr function)
{
    function->PrintAsm(writer);
}


/*
 * This only prints the library function declaration... :(
 *
 * declare dso_local i32 @getint(...)
 * declare dso_local void @putstr(i8*)
 * declare dso_local void @putint(i32)
 */
void StandardAsmPrinter::_PrintDeclaration(IAsmWriterPtr writer)
{
    writer->Push("declare dso_local i32 @getint(...)");
    writer->PushNewLine();
    writer->Push("declare dso_local void @putstr(i8*)");
    writer->PushNewLine();
    writer->Push("declare dso_local void @putint(i32)");
    writer->PushNewLine();
    writer->PushNewLine();
}


TOMIC_LLVM_END
