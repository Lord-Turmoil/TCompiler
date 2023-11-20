/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/asm/impl/VerboseAsmPrinter.h>
#include <tomic/llvm/asm/impl/VerboseAsmWriter.h>
#include <tomic/llvm/ir/Module.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/value/GlobalVariable.h>

TOMIC_LLVM_BEGIN

void VerboseAsmPrinter::Print(ModulePtr module, twio::IWriterPtr writer)
{
    auto asmWriter = VerboseAsmWriter::New(writer);

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
void VerboseAsmPrinter::_PrintHeader(IAsmWriterPtr writer)
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
void VerboseAsmPrinter::_PrintFooter(IAsmWriterPtr writer)
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
void VerboseAsmPrinter::_PrintModule(IAsmWriterPtr writer, ModulePtr module)
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
    for (auto it = module->GlobalBegin(); it != module->GlobalEnd(); ++it)
    {
        (*it)->PrintAsm(writer);
    }

    // Global string constants.
    if (module->GlobalStringCount() > 0)
    {
        writer->PushNewLine();
        for (auto it = module->GlobalStringBegin(); it != module->GlobalStringEnd(); ++it)
        {
            (*it)->PrintAsm(writer);
        }
        writer->PushNewLine();
    }

    // Functions.
    for (auto it = module->FunctionBegin(); it != module->FunctionEnd(); ++it)
    {
        (*it)->PrintAsm(writer);
    }

    // Main function.
    if (module->GetMainFunction())
    {
        module->GetMainFunction()->PrintAsm(writer);
    }
}


/*
 * This only prints the library function declaration... :(
 *
 * declare dso_local i32 @getint()
 * declare dso_local void @putstr(i8*)
 * declare dso_local void @putint(i32)
 */
void VerboseAsmPrinter::_PrintDeclaration(IAsmWriterPtr writer)
{
    writer->Push("declare i32 @getint()");
    writer->PushNewLine();
    writer->Push("declare void @putint(i32)");
    writer->PushNewLine();
    writer->Push("declare void @putstr(i8*)");
    writer->PushNewLine();
    writer->PushNewLine();
}


TOMIC_LLVM_END
