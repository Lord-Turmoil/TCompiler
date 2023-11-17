/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/asm/impl/StandardAsmPrinter.h>
#include <tomic/llvm/asm/impl/StandardAsmWriter.h>
#include <tomic/llvm/ir/Module.h>
#include <tomic/llvm/ir/value/Function.h>

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
    writer->Push("LLVM IR Module: ");
    writer->Push(module->Name());
    writer->CommentEnd();
    writer->PushNewLine();

    // Global variables.
    for (auto globalIter = module->GlobalBegin(); globalIter != module->GlobalEnd(); ++globalIter)
    {
        _PrintGlobalVariable(writer, *globalIter);
    }
    if (module->GlobalCount() > 0)
    {
        writer->PushNewLine();
    }

    // Functions.
    for (auto funcIter = module->FunctionBegin(); funcIter != module->FunctionEnd(); ++funcIter)
    {
        _PrintFunction(writer, *funcIter);
    }
    if (module->FunctionCount() > 0)
    {
        writer->PushNewLine();
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
}

void StandardAsmPrinter::_PrintFunction(IAsmWriterPtr writer, FunctionPtr function)
{
    function->PrintAsm(writer);
}

TOMIC_LLVM_END
