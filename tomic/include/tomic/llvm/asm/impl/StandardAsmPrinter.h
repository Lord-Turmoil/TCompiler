/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_STANDARD_ASM_PRINTER_H_
#define _TOMIC_LLVM_STANDARD_ASM_PRINTER_H_

#include <tomic/llvm/asm/IAsmPrinter.h>
#include <tomic/llvm/asm/IAsmWriter.h>

TOMIC_LLVM_BEGIN

class StandardAsmPrinter : public IAsmPrinter
{
public:
    StandardAsmPrinter() = default;
    ~StandardAsmPrinter() override = default;

    void Print(ModulePtr module, twio::IWriterPtr writer) override;

private:
    void _PrintHeader(IAsmWriterPtr writer);
    void _PrintFooter(IAsmWriterPtr writer);

    void _PrintModule(IAsmWriterPtr writer, ModulePtr module);
    void _PrintGlobalVariable(IAsmWriterPtr writer, GlobalVariablePtr globalVariable);
    void _PrintFunction(IAsmWriterPtr writer, FunctionPtr function);

    void _PrintDeclaration(IAsmWriterPtr writer);
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_STANDARD_ASM_PRINTER_H_
