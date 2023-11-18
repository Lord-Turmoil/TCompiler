/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_ASM_PRINTER_H_
#define _TOMIC_LLVM_ASM_PRINTER_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/IrForward.h>
#include <memory>

TOMIC_LLVM_BEGIN

/*
 * Values and Types can generate their LLVM IR ASM by themselves, so
 * this ASM printer simply joins them together, and add some trivial
 * header and footer to the .ll file.
 */
class IAsmPrinter
{
public:
    virtual ~IAsmPrinter() = default;

    // In print, twio::IWriterPtr will be converted to IAsmWriterPtr.
    virtual void Print(ModulePtr module, twio::IWriterPtr writer) = 0;
};


using IAsmPrinterPtr = std::shared_ptr<IAsmPrinter>;

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_ASM_PRINTER_H_
