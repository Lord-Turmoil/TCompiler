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
    _PrintModule(asmWriter, module);
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
    // Library function declaration.
    _PrintDeclaration(writer);

    // Global variables.
    for (auto it = module->GlobalBegin(); it != module->GlobalEnd(); ++it)
    {
        (*it)->PrintAsm(writer);
    }

    // Global string constants.
    if (module->GlobalCount() > 0)
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
 * declare dso_local i32 @getint(...)
 * declare dso_local void @putstr(i8*)
 * declare dso_local void @putint(i32)
 */
void StandardAsmPrinter::_PrintDeclaration(IAsmWriterPtr writer)
{
    writer->Push("declare dso_local i32 @getint()");
    writer->PushNewLine();
    writer->Push("declare dso_local void @putstr(i8*)");
    writer->PushNewLine();
    writer->Push("declare dso_local void @putint(i32)");
    writer->PushNewLines(3);
}


TOMIC_LLVM_END
