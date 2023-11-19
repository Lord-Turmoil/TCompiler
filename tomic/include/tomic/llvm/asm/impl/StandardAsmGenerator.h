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

#include <unordered_map>

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
// ? If missing, class will be improperly indented.
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

    /**
     * Maintain a map from SymbolTableEntry to Value.
     * Do not use smart pointer here since it is not consistent!
     */
    std::unordered_map<SymbolTableEntryPtr, ValuePtr> _valueMap;

private:
    SymbolTableBlockPtr _GetSymbolTableBlock(SyntaxNodePtr node) const;

    void _AddValue(SymbolTableEntryPtr entry, ValuePtr value);
    ValuePtr _GetValue(SymbolTableEntryPtr entry);
    ValuePtr _GetLValValue(SyntaxNodePtr node);

    TypePtr _GetEntryType(SymbolTableEntryPtr entry);

    /*
     * Set the current function and basic block.
     * Will return the old value.
     */
    FunctionPtr _SetCurrentFunction(FunctionPtr function);
    BasicBlockPtr _SetCurrentBasicBlock(BasicBlockPtr block);
    InstructionPtr _InsertInstruction(InstructionPtr instruction);

private:
    /*
     * AstVisitor interface.
     * It is mainly used to iterate all BlockItems to generate instructions.
     */
    bool VisitEnter(SyntaxNodePtr node) override;
    // bool VisitExit(SyntaxNodePtr node) override;
    // bool Visit(SyntaxNodePtr node) override;

    bool _ParseCompilationUnit();

    FunctionPtr _ParseMainFunction(SyntaxNodePtr node);
    // FunctionPtr _GenerateFunction(SyntaxNodePtr node);

    /*
     * Parent entrance for generating instructions. Basic blocks will be
     * generated automatically depends on the instructions.
     */
    bool _ParseInstructions(SyntaxNodePtr node);

    void _ParseStatement(SyntaxNodePtr node);

    // Global variable and constant.
    void _ParseGlobalDecl(SyntaxNodePtr node);
    GlobalVariablePtr _ParseGlobalVarDef(SyntaxNodePtr node);
    GlobalVariablePtr _ParseGlobalConstantDef(SyntaxNodePtr node);
    ConstantDataPtr _ParseGlobalInitValue(SyntaxNodePtr node);

    // Local variable and constant.
    void _ParseVariableDecl(SyntaxNodePtr node);
    AllocaInstPtr _ParseVariableDef(SyntaxNodePtr node);
    AllocaInstPtr _ParseArrayDef(SyntaxNodePtr node);

    // Statement.
    ReturnInstPtr _ParseReturnStatement(SyntaxNodePtr node);
    void _ParseAssignStatement(SyntaxNodePtr node);

    // Expression.
    ValuePtr _ParseExpression(SyntaxNodePtr node);
    ValuePtr _ParseAddExp(SyntaxNodePtr node);
    ValuePtr _ParseMulExp(SyntaxNodePtr node);

    /*
     * For UnaryExp, there is a problem for nesting. e.g. +-+-a, or !!!!b.
     * It is solved by an AST transformer optimizer, so we won't need to
     * consider it here.
     */
    ValuePtr _ParseUnaryExp(SyntaxNodePtr node);

    ValuePtr _ParsePrimaryExp(SyntaxNodePtr node);

    ValuePtr _ParseLVal(SyntaxNodePtr node);
    ValuePtr _ParseNumber(SyntaxNodePtr node);
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_STANDARD_ASM_GENERATOR_H_
