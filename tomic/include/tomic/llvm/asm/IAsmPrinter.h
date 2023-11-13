/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_ASM_PRINTER_H_
#define _TOMIC_LLVM_ASM_PRINTER_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/IrForward.h>

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

    // In print, twio::IWriter will be converted to IAsmWriter.
    virtual void Print(ModulePtr module, twio::IWriter writer) = 0;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_ASM_PRINTER_H_
