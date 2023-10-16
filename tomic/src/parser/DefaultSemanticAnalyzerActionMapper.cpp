/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/impl/DefaultSemanticAnalyzer.h>

TOMIC_BEGIN

DefaultSemanticAnalyzerActionMapper::DefaultSemanticAnalyzerActionMapper()
{
    _Init();
}

EnterAction DefaultSemanticAnalyzerActionMapper::GetEnterAction(tomic::SyntaxType type) const
{
    auto it = _enterActions.find(type);
    if (it != _enterActions.end())
    {
        return it->second;
    }
    return nullptr;
}

ExitAction DefaultSemanticAnalyzerActionMapper::GetExitAction(tomic::SyntaxType type) const
{
    auto it = _exitActions.find(type);
    if (it != _exitActions.end())
    {
        return it->second;
    }
    return nullptr;
}

void DefaultSemanticAnalyzerActionMapper::_Init()
{
    _enterActions[SyntaxType::ST_COMP_UNIT] = &DefaultSemanticAnalyzer::_EnterCompUnit;
    _exitActions[SyntaxType::ST_COMP_UNIT] = &DefaultSemanticAnalyzer::_ExitCompUnit;

    _enterActions[SyntaxType::ST_DECL] = &DefaultSemanticAnalyzer::_EnterDecl;
    _exitActions[SyntaxType::ST_DECL] = &DefaultSemanticAnalyzer::_DefaultExit;

    _enterActions[SyntaxType::ST_BTYPE] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_BTYPE] = &DefaultSemanticAnalyzer::_ExitBType;

    _enterActions[SyntaxType::ST_CONST_DECL] = &DefaultSemanticAnalyzer::_EnterConstDecl;
    _exitActions[SyntaxType::ST_CONST_DECL] = &DefaultSemanticAnalyzer::_DefaultExit;

    _enterActions[SyntaxType::ST_CONST_DEF] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_CONST_DEF] = &DefaultSemanticAnalyzer::_ExitConstDef;

    _enterActions[SyntaxType::ST_CONST_INIT_VAL] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_CONST_INIT_VAL] = &DefaultSemanticAnalyzer::_ExitConstInitVal;

    _enterActions[SyntaxType::ST_VAR_DECL] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_VAR_DECL] = &DefaultSemanticAnalyzer::_DefaultExit;

    _enterActions[SyntaxType::ST_VAR_DEF] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_VAR_DEF] = &DefaultSemanticAnalyzer::_ExitVarDef;

    _enterActions[SyntaxType::ST_INIT_VAL] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_INIT_VAL] = &DefaultSemanticAnalyzer::_ExitInitVal;

    _enterActions[SyntaxType::ST_FUNC_DEF] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FUNC_DEF] = &DefaultSemanticAnalyzer::_ExitFuncDef;

    _enterActions[SyntaxType::ST_FUNC_TYPE] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FUNC_TYPE] = &DefaultSemanticAnalyzer::_ExitFuncType;

    _enterActions[SyntaxType::ST_FUNC_FPARAMS] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FUNC_FPARAMS] = &DefaultSemanticAnalyzer::_ExitFuncFParams;

    _enterActions[SyntaxType::ST_FUNC_FPARAM] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FUNC_FPARAM] = &DefaultSemanticAnalyzer::_ExitFuncFParam;

    _enterActions[SyntaxType::ST_FUNC_APARAMS] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FUNC_APARAMS] = &DefaultSemanticAnalyzer::_ExitFuncAParams;

    _enterActions[SyntaxType::ST_FUNC_APARAM] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FUNC_APARAM] = &DefaultSemanticAnalyzer::_ExitFuncAParam;

    _enterActions[SyntaxType::ST_BLOCK] = &DefaultSemanticAnalyzer::_EnterBlock;
    _exitActions[SyntaxType::ST_BLOCK] = &DefaultSemanticAnalyzer::_ExitBlock;

    _enterActions[SyntaxType::ST_BLOCK_ITEM] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_BLOCK_ITEM] = &DefaultSemanticAnalyzer::_DefaultExit;

    _enterActions[SyntaxType::ST_MAIN_FUNC_DEF] = &DefaultSemanticAnalyzer::_EnterMainFuncDef;
    _exitActions[SyntaxType::ST_MAIN_FUNC_DEF] = &DefaultSemanticAnalyzer::_DefaultExit;

    _enterActions[SyntaxType::ST_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_STMT] = &DefaultSemanticAnalyzer::_DefaultExit;

    _enterActions[SyntaxType::ST_ASSIGNMENT_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_ASSIGNMENT_STMT] = &DefaultSemanticAnalyzer::_ExitAssignmentStmt;

    _enterActions[SyntaxType::ST_LVAL] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_LVAL] = &DefaultSemanticAnalyzer::_ExitLVal;

    _enterActions[SyntaxType::ST_COND] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_COND] = &DefaultSemanticAnalyzer::_ExitCond;

    _enterActions[SyntaxType::ST_IF_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_IF_STMT] = &DefaultSemanticAnalyzer::_DefaultExit;

    _enterActions[SyntaxType::ST_FOR_STMT] = &DefaultSemanticAnalyzer::_EnterForStmt;
    _exitActions[SyntaxType::ST_FOR_STMT] = &DefaultSemanticAnalyzer::_DefaultExit;

    _enterActions[SyntaxType::ST_FOR_INIT_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FOR_INIT_STMT] = &DefaultSemanticAnalyzer::_ExitForInnerStmt;

    _enterActions[SyntaxType::ST_FOR_STEP_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FOR_STEP_STMT] = &DefaultSemanticAnalyzer::_ExitForInnerStmt;

    _enterActions[SyntaxType::ST_EXP_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_EXP_STMT] = &DefaultSemanticAnalyzer::_ExitExp;

    _enterActions[SyntaxType::ST_CONST_EXP] = &DefaultSemanticAnalyzer::_EnterConstExp;
    _exitActions[SyntaxType::ST_CONST_EXP] = &DefaultSemanticAnalyzer::_ExitConstExp;

    _enterActions[SyntaxType::ST_BREAK_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_BREAK_STMT] = &DefaultSemanticAnalyzer::_ExitBreakStmt;

    _enterActions[SyntaxType::ST_CONTINUE_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_CONTINUE_STMT] = &DefaultSemanticAnalyzer::_ExitContinueStmt;

    _enterActions[SyntaxType::ST_RETURN_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_RETURN_STMT] = &DefaultSemanticAnalyzer::_ExitReturnStmt;

    _enterActions[SyntaxType::ST_IN_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_IN_STMT] = &DefaultSemanticAnalyzer::_ExitInStmt;

    _enterActions[SyntaxType::ST_OUT_STMT] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_OUT_STMT] = &DefaultSemanticAnalyzer::_ExitOutStmt;

    _enterActions[SyntaxType::ST_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_EXP] = &DefaultSemanticAnalyzer::_DefaultExitExp;

    _enterActions[SyntaxType::ST_ADD_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_ADD_EXP] = &DefaultSemanticAnalyzer::_DefaultExitExp;

    _enterActions[SyntaxType::ST_MUL_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_MUL_EXP] = &DefaultSemanticAnalyzer::_DefaultExitExp;

    _enterActions[SyntaxType::ST_UNARY_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_UNARY_EXP] = &DefaultSemanticAnalyzer::_ExitUnaryExp;

    _enterActions[SyntaxType::ST_UNARY_OP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_UNARY_OP] = &DefaultSemanticAnalyzer::_ExitUnaryOp;

    _enterActions[SyntaxType::ST_PRIMARY_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_PRIMARY_EXP] = &DefaultSemanticAnalyzer::_ExitPrimaryExp;

    _enterActions[SyntaxType::ST_FUNC_CALL] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_FUNC_CALL] = &DefaultSemanticAnalyzer::_ExitFuncCall;

    _enterActions[SyntaxType::ST_NUMBER] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_NUMBER] = &DefaultSemanticAnalyzer::_ExitNumber;

    _enterActions[SyntaxType::ST_OR_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_OR_EXP] = &DefaultSemanticAnalyzer::_DefaultExitExp;

    _enterActions[SyntaxType::ST_AND_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_AND_EXP] = &DefaultSemanticAnalyzer::_DefaultExitExp;

    _enterActions[SyntaxType::ST_EQ_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_EQ_EXP] = &DefaultSemanticAnalyzer::_DefaultExitExp;

    _enterActions[SyntaxType::ST_REL_EXP] = &DefaultSemanticAnalyzer::_DefaultEnter;
    _exitActions[SyntaxType::ST_REL_EXP] = &DefaultSemanticAnalyzer::_DefaultExitExp;
}

TOMIC_END
