/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_SEMANTIC_ANALYZER_H_
#define _TOMIC_DEFAULT_SEMANTIC_ANALYZER_H_

#include <tomic/Common.h>
#include <tomic/parser/ISemanticAnalyzer.h>
#include <tomic/parser/ast/AstVisitor.h>
#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/ast/SyntaxType.h>
#include <tomic/logger/error/IErrorLogger.h>
#include <vector>

TOMIC_BEGIN

class DefaultSemanticAnalyzer : public ISemanticAnalyzer, private AstVisitor
{
    friend class DefaultSemanticAnalyzerActionMapper;

public:
    DefaultSemanticAnalyzer(IErrorLoggerPtr logger);
    ~DefaultSemanticAnalyzer() override = default;

    SymbolTablePtr Analyze(SyntaxTreePtr tree) override;

private:
    bool VisitEnter(SyntaxNodePtr node) override;
    bool VisitExit(SyntaxNodePtr node) override;
    bool Visit(SyntaxNodePtr node) override;

private:
    void _AnalyzePreamble();

private:
    IErrorLoggerPtr _logger;

    SymbolTablePtr _table;
    SymbolTableBlockPtr _currentBlock;

    // Record the syntax hierarchy.
    std::vector<SyntaxType> _typeStack;
    SyntaxType CurrentType() const { return _typeStack.back(); }

private:
    SymbolTableBlockPtr _GetOrCreateBlock(SyntaxNodePtr node);

private:
    bool _DefaultEnter(SyntaxNodePtr node) { return true; }
    bool _DefaultExit(SyntaxNodePtr node) { return true; }

    bool _EnterCompUnit(SyntaxNodePtr node);
    bool _ExitCompUnit(SyntaxNodePtr node);

    ////////// Decl
    bool _EnterDecl(SyntaxNodePtr node);
    bool _ExitDecl(SyntaxNodePtr node);
    bool _EnterBType(SyntaxNodePtr node);
    bool _ExitBType(SyntaxNodePtr node);

    // ConstDecl
    bool _EnterConstDecl(SyntaxNodePtr node);
    bool _ExitConstDecl(SyntaxNodePtr node);
    bool _EnterConstDef(SyntaxNodePtr node);
    bool _ExitConstDef(SyntaxNodePtr node);
    bool _EnterConstInitVal(SyntaxNodePtr node);
    bool _ExitConstInitVal(SyntaxNodePtr node);

    // VarDecl
    bool _EnterVarDecl(SyntaxNodePtr node);
    bool _ExitVarDecl(SyntaxNodePtr node);
    bool _EnterVarDef(SyntaxNodePtr node);
    bool _ExitVarDef(SyntaxNodePtr node);
    bool _EnterInitVal(SyntaxNodePtr node);
    bool _ExitInitVal(SyntaxNodePtr node);

    ////////// FuncDef
    bool _EnterFuncDef();
    bool _ExitFuncDef();
    bool _EnterFuncType(SyntaxNodePtr node);
    bool _ExitFuncType(SyntaxNodePtr node);
    bool _EnterFuncFParams(SyntaxNodePtr node);
    bool _ExitFuncFParams(SyntaxNodePtr node);
    bool _EnterFuncFParam(SyntaxNodePtr node);
    bool _ExitFuncFParam(SyntaxNodePtr node);
    bool _EnterFuncAParams(SyntaxNodePtr node);
    bool _ExitFuncAParams(SyntaxNodePtr node);

    bool _EnterBlock(SyntaxNodePtr node);
    bool _ExitBlock(SyntaxNodePtr node);
    bool _EnterBlockItem(SyntaxNodePtr node);
    bool _ExitBlockItem(SyntaxNodePtr node);

    ////////// MainFuncDef
    bool _EnterMainFuncDef(SyntaxNodePtr node);
    bool _ExitMainFuncDef(SyntaxNodePtr node);

    ////////// Stmt
    bool _EnterStmt(SyntaxNodePtr node);
    bool _ExitStmt(SyntaxNodePtr node);
    bool _EnterAssignmentStmt(SyntaxNodePtr node);
    bool _ExitAssignmentStmt(SyntaxNodePtr node);
    bool _EnterLVal(SyntaxNodePtr node);
    bool _ExitLVal(SyntaxNodePtr node);
    bool _EnterCond(SyntaxNodePtr node);
    bool _ExitCond(SyntaxNodePtr node);

    bool _EnterIfStmt(SyntaxNodePtr node);
    bool _ExitIfStmt(SyntaxNodePtr node);
    bool _EnterForStmt(SyntaxNodePtr node);
    bool _ExitForStmt(SyntaxNodePtr node);
    bool _EnterForInitStmt(SyntaxNodePtr node);
    bool _ExitForInitStmt(SyntaxNodePtr node);
    bool _EnterForStepStmt(SyntaxNodePtr node);
    bool _ExitForStepStmt(SyntaxNodePtr node);
    bool _EnterExpStmt(SyntaxNodePtr node);
    bool _ExitExpStmt(SyntaxNodePtr node);

    bool _EnterBreakStmt(SyntaxNodePtr node);
    bool _ExitBreakStmt(SyntaxNodePtr node);
    bool _EnterContinueStmt(SyntaxNodePtr node);
    bool _ExitContinueStmt(SyntaxNodePtr node);
    bool _AnalyzeReturnStmt(SyntaxNodePtr node);

    bool _EnterInStmt(SyntaxNodePtr node);
    bool _ExitInStmt(SyntaxNodePtr node);
    bool _EnterOutStmt(SyntaxNodePtr node);
    bool _ExitOutStmt(SyntaxNodePtr node);

    ////////// Exp
    bool _EnterExp(SyntaxNodePtr node);
    bool _ExitExp(SyntaxNodePtr node);
    bool _EnterConstExp(SyntaxNodePtr node);
    bool _ExitConstExp(SyntaxNodePtr node);
    bool _EnterAddExp(SyntaxNodePtr node);
    bool _ExitAddExp(SyntaxNodePtr node);
    bool _AnalyzeMulExp(SyntaxNodePtr node);
    bool _AnalyzeUnaryExp(SyntaxNodePtr node);
    bool _AnalyzeUnaryOp(SyntaxNodePtr node);
    bool _AnalyzePrimaryExp(SyntaxNodePtr node);
    bool _EnterFuncCall(SyntaxNodePtr node);
    bool _ExitFuncCall(SyntaxNodePtr node);
    bool _EnterNumber(SyntaxNodePtr node);
    bool _ExitNumber(SyntaxNodePtr node);

    bool _EnterOrExp(SyntaxNodePtr node);
    bool _ExitOrExp(SyntaxNodePtr node);
    bool _EnterAndExp(SyntaxNodePtr node);
    bool _ExitAndExp(SyntaxNodePtr node);
    bool _EnterEqExp(SyntaxNodePtr node);
    bool _ExitEqExp(SyntaxNodePtr node);
    bool _EnterRelExp(SyntaxNodePtr node);
    bool _ExitRelExp(SyntaxNodePtr node);
};


class DefaultSemanticAnalyzerActionMapper
{
public:
    // Action mapper
    using EnterAction = bool (DefaultSemanticAnalyzer::*)(SyntaxNodePtr);
    using ExitAction = bool (DefaultSemanticAnalyzer::*)(SyntaxNodePtr);

    DefaultSemanticAnalyzerActionMapper();
    ~DefaultSemanticAnalyzerActionMapper() = default;

    EnterAction GetEnterAction(SyntaxType type) const;
    ExitAction GetExitAction(SyntaxType type) const;

private:
    void _Init();

    std::map<SyntaxType, EnterAction> _enterActionMap;
    std::map<SyntaxType, ExitAction> _exitActionMap;
};

TOMIC_END

#endif // _TOMIC_DEFAULT_SEMANTIC_ANALYZER_H_
