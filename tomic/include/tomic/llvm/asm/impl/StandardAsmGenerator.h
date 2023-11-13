/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_STANDARD_ASM_GENERATOR_H_
#define _TOMIC_LLVM_STANDARD_ASM_GENERATOR_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/asm/IAsmGenerator.h>
#include <tomic/parser/ast/AstVisitor.h>

TOMIC_LLVM_BEGIN

/*
 * StandardAsmGenerator is a standard implementation of IAsmGenerator.
 * It will generate LLVM IR code from a SyntaxTree and a SymbolTable.
 * It will not check the validity of the SyntaxTree and SymbolTable!
 *
 * It again uses the visitor pattern to traverse the SyntaxTree, but
 * this time basic iteration will also be used, since we have already
 * known the structure of the SyntaxTree.
 */
class StandardAsmGenerator : public IAsmGenerator, private AstVisitor
{
public:
    StandardAsmGenerator();
    ~StandardAsmGenerator() override = default;

    ModuleSmartPtr Generate(
            SyntaxTreePtr syntaxTree,
            SymbolTablePtr symbolTable,
            const char* name) override;

private:
    SyntaxTreePtr _syntaxTree;
    SymbolTablePtr _symbolTable;
    ModuleSmartPtr _module;

    /*
     * Current function and block. Maintain a global state.
     * Since they will not be nested, so it's OK not to use stack.
     */
    FunctionPtr _currentFunction;
    BasicBlockPtr _currentBlock;

private:
    SymbolTableBlockPtr _GetSymbolTableBlock(SyntaxNodePtr node);

    /*
     * Set the current function and basic block.
     * Will return the old value.
     */
    FunctionPtr _SetCurrentFunction(FunctionPtr function);
    BasicBlockPtr _SetCurrentBasicBlock(BasicBlockPtr block);

private:
    /*
     * AstVisitor interface.
     * It is mainly used to iterate all BlockItems to generate instructions.
     */
    bool VisitEnter(SyntaxNodePtr node) override;
    // bool VisitExit(SyntaxNodePtr node) override;
    // bool Visit(SyntaxNodePtr node) override;

    bool _ParseCompilationUnit();

    FunctionPtr _GenerateMainFunction(SyntaxNodePtr node);
    // FunctionPtr _GenerateFunction(SyntaxNodePtr node);

    /*
     * Parent entrance for generating instructions. Basic blocks will be
     * generated automatically depends on the instructions.
     */
    bool _GenerateInstructions(SyntaxNodePtr node);

    void _GenerateStatement(SyntaxNodePtr node);

    ReturnInstPtr _GenerateReturnStatement(SyntaxNodePtr node);
    ValuePtr _GenerateExpression(SyntaxNodePtr node);
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_STANDARD_ASM_GENERATOR_H_
