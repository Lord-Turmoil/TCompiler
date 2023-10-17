/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_SEMANTIC_ANALYZER_H_
#define _TOMIC_DEFAULT_SEMANTIC_ANALYZER_H_

#include <tomic/Shared.h>
#include <tomic/parser/ISemanticAnalyzer.h>
#include <tomic/parser/ast/AstVisitor.h>
#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/ast/SyntaxType.h>
#include <tomic/logger/error/IErrorLogger.h>
#include <tomic/logger/error/ErrorType.h>
#include <tomic/logger/debug/ILogger.h>
#include <tomic/parser/table/SymbolTable.h>
#include <tomic/parser/table/SymbolTableBlock.h>
#include <vector>
#include <stack>

TOMIC_BEGIN

class DefaultSemanticAnalyzer : public ISemanticAnalyzer, private AstVisitor
{
    friend class DefaultSemanticAnalyzerActionMapper;

public:
    DefaultSemanticAnalyzer(IErrorLoggerPtr errorLogger, ILoggerPtr logger);
    ~DefaultSemanticAnalyzer() override = default;

    SymbolTablePtr Analyze(SyntaxTreePtr tree) override;

private:
    bool VisitEnter(SyntaxNodePtr node) override;
    bool VisitExit(SyntaxNodePtr node) override;
    bool Visit(SyntaxNodePtr node) override;

private:
    bool _AnalyzePreamble(SyntaxNodePtr node);

private:
    IErrorLoggerPtr _errorLogger;
    ILoggerPtr _logger;

    SymbolTablePtr _table;
    SymbolTableBlockPtr _currentBlock;

    // This is used to get the line info.
    std::stack<SyntaxNodePtr> _nodeStack;
    SyntaxNodePtr _errorCandidate;


private:
    SymbolTableBlockPtr _GetOrCreateBlock(SyntaxNodePtr node);
    void _AddToSymbolTable(SymbolTableEntryPtr entry);

    int _ValidateConstSubscription(SyntaxNodePtr constExp);
    void _ValidateSubscription(SyntaxNodePtr exp);

private:
    void _Log(LogLevel level, const char* format, ...);
    void _LogError(ErrorType type, const char* format, ...);

private:
    /*
     * I don't want to write this much functions, I just leave them
     * be, in case I need to do something special in the future.
     */

    bool _DefaultEnter(SyntaxNodePtr node) { return true; }
    bool _DefaultExit(SyntaxNodePtr node) { return true; }

    bool _EnterCompUnit(SyntaxNodePtr node);
    bool _ExitCompUnit(SyntaxNodePtr node);

    ////////// Decl
    bool _EnterDecl(SyntaxNodePtr node);
    bool _ExitDecl(SyntaxNodePtr node) { return true; }
    bool _EnterBType(SyntaxNodePtr node) { return true; }
    bool _ExitBType(SyntaxNodePtr node);

    // ConstDecl
    bool _EnterConstDecl(SyntaxNodePtr node);
    bool _ExitConstDecl(SyntaxNodePtr node) { return true; }
    bool _EnterConstDef(SyntaxNodePtr node) { return true; }
    bool _ExitConstDef(SyntaxNodePtr node);

    bool _EnterConstInitVal(SyntaxNodePtr node) { return true; }
    bool _ExitConstInitVal(SyntaxNodePtr node);

    // VarDecl
    bool _EnterVarDecl(SyntaxNodePtr node) { return true; }
    bool _ExitVarDecl(SyntaxNodePtr node) { return true; }
    bool _EnterVarDef(SyntaxNodePtr node) { return true; }
    bool _ExitVarDef(SyntaxNodePtr node);

    bool _EnterInitVal(SyntaxNodePtr node) { return true; }
    bool _ExitInitVal(SyntaxNodePtr node);

    ////////// FuncDef
    bool _EnterFuncDef(SyntaxNodePtr node) { return true; }
    bool _ExitFuncDef(SyntaxNodePtr node);
    bool _EnterFuncDecl(SyntaxNodePtr node) { return true; }
    bool _ExitFuncDecl(SyntaxNodePtr node);
    bool _EnterFuncType(SyntaxNodePtr node) { return true; }
    bool _ExitFuncType(SyntaxNodePtr node);
    bool _EnterFuncFParams(SyntaxNodePtr node) { return true; }
    bool _ExitFuncFParams(SyntaxNodePtr node);
    bool _EnterFuncFParam(SyntaxNodePtr node) { return true; }
    bool _ExitFuncFParam(SyntaxNodePtr node);
    bool _EnterFuncAParams(SyntaxNodePtr node) { return true; }
    bool _ExitFuncAParams(SyntaxNodePtr node);

    bool _EnterFuncAParam(SyntaxNodePtr node) { return true; }
    bool _ExitFuncAParam(SyntaxNodePtr node);

    bool _EnterBlock(SyntaxNodePtr node);
    bool _ExitBlock(SyntaxNodePtr node);
    bool _EnterBlockItem(SyntaxNodePtr node) { return true; }
    bool _ExitBlockItem(SyntaxNodePtr node) { return true; }

    ////////// MainFuncDef
    bool _EnterMainFuncDef(SyntaxNodePtr node);
    bool _ExitMainFuncDef(SyntaxNodePtr node);

    ////////// Stmt
    bool _EnterStmt(SyntaxNodePtr node) { return true; }
    bool _ExitStmt(SyntaxNodePtr node) { return true; }
    bool _EnterAssignmentStmt(SyntaxNodePtr node) { return true; }
    bool _ExitAssignmentStmt(SyntaxNodePtr node);
    bool _EnterLVal(SyntaxNodePtr node) { return true; }
    bool _ExitLVal(SyntaxNodePtr node);
    bool _EnterCond(SyntaxNodePtr node) { return true; }
    bool _ExitCond(SyntaxNodePtr node);

    bool _EnterIfStmt(SyntaxNodePtr node) { return true; }
    bool _ExitIfStmt(SyntaxNodePtr node) { return true; }
    bool _EnterForStmt(SyntaxNodePtr node);
    bool _ExitForStmt(SyntaxNodePtr node) { return true; }
    bool _EnterForInnerStmt(SyntaxNodePtr node) { return true; }
    bool _ExitForInnerStmt(SyntaxNodePtr node);
    bool _EnterExpStmt(SyntaxNodePtr node) { return true; }
    bool _ExitExpStmt(SyntaxNodePtr node) { return true; }

    bool _EnterBreakStmt(SyntaxNodePtr node) { return true; }
    bool _ExitBreakStmt(SyntaxNodePtr node);
    bool _EnterContinueStmt(SyntaxNodePtr node) { return true; }
    bool _ExitContinueStmt(SyntaxNodePtr node);
    bool _EnterReturnStmt(SyntaxNodePtr node) { return true; }
    bool _ExitReturnStmt(SyntaxNodePtr node);

    bool _EnterInStmt(SyntaxNodePtr node) { return true; }
    bool _ExitInStmt(SyntaxNodePtr node);
    bool _EnterOutStmt(SyntaxNodePtr node) { return true; }
    bool _ExitOutStmt(SyntaxNodePtr node);

    ////////// Exp
    // Since many node just pass value or check value, we can combine them.
    bool _DefaultExitExp(SyntaxNodePtr node);

    bool _EnterExp(SyntaxNodePtr node) { return true; }
    bool _ExitExp(SyntaxNodePtr node);
    bool _EnterConstExp(SyntaxNodePtr node);
    bool _ExitConstExp(SyntaxNodePtr node);
    bool _EnterAddExp(SyntaxNodePtr node) { return true; }
    bool _ExitAddExp(SyntaxNodePtr node) { return true; }
    bool _EnterMulExp(SyntaxNodePtr node) { return true; }
    bool _ExitMulExp(SyntaxNodePtr node) { return true; }
    bool _EnterUnaryExp(SyntaxNodePtr node) { return true; }
    bool _ExitUnaryExp(SyntaxNodePtr node);
    bool _EnterUnaryOp(SyntaxNodePtr node) { return true; }
    bool _ExitUnaryOp(SyntaxNodePtr node);
    bool _EnterPrimaryExp(SyntaxNodePtr node) { return true; }
    bool _ExitPrimaryExp(SyntaxNodePtr node);
    bool _EnterFuncCall(SyntaxNodePtr node) { return true; }
    bool _ExitFuncCall(SyntaxNodePtr node);
    bool _EnterNumber(SyntaxNodePtr node) { return true; }
    bool _ExitNumber(SyntaxNodePtr node);

    bool _EnterOrExp(SyntaxNodePtr node) { return true; }
    bool _ExitOrExp(SyntaxNodePtr node) { return true; }
    bool _EnterAndExp(SyntaxNodePtr node) { return true; }
    bool _ExitAndExp(SyntaxNodePtr node) { return true; }
    bool _EnterEqExp(SyntaxNodePtr node) { return true; }
    bool _ExitEqExp(SyntaxNodePtr node) { return true; }
    bool _EnterRelExp(SyntaxNodePtr node) { return true; }
    bool _ExitRelExp(SyntaxNodePtr node) { return true; }
};

using EnterAction = bool (DefaultSemanticAnalyzer::*)(SyntaxNodePtr);
using ExitAction = bool (DefaultSemanticAnalyzer::*)(SyntaxNodePtr);

class DefaultSemanticAnalyzerActionMapper
{
public:
    // Action mapper
    DefaultSemanticAnalyzerActionMapper();
    ~DefaultSemanticAnalyzerActionMapper() = default;

    EnterAction GetEnterAction(SyntaxType type) const;
    ExitAction GetExitAction(SyntaxType type) const;

private:
    void _Init();

    std::map<SyntaxType, EnterAction> _enterActions;
    std::map<SyntaxType, ExitAction> _exitActions;

    EnterAction _defaultEnter;
    ExitAction _defaultExit;
};

TOMIC_END

#endif // _TOMIC_DEFAULT_SEMANTIC_ANALYZER_H_
