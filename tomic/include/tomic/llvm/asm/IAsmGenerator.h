/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Overview:
 *
 * STOP USING BARNACLE DEPENDENCY INJECTION!!! >:(
 * HERE WE JUST GENERATE THE ONE AND ONLY LLVM IR MODULE.
 * (But... I'll use it secretly. :P)
 *
 * This class is to generate LLVM assembly code from our syntax tree
 * and symbol table.
 *
 * This is the bridge between our syntax tree and LLVM IR.
 * The ASM generator will get us a LLVM IR module, which contains all
 * Values of LLVM IR.
 *
 * After this, we can use AsmWriter to write our LLVM IR module to a file,
 * or anywhere you like.
 */

#ifndef _TOMIC_LLVM_ASM_GENERATOR_H_
#define _TOMIC_LLVM_ASM_GENERATOR_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/Module.h>

#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/table/SymbolTable.h>

TOMIC_LLVM_BEGIN

class IAsmGenerator
{
public:
    virtual ~IAsmGenerator() = default;

    virtual ModuleSmartPtr Generate(
        SyntaxTreePtr syntaxTree,
        SymbolTablePtr symbolTable,
        const char* name) = 0;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_ASM_GENERATOR_H_
