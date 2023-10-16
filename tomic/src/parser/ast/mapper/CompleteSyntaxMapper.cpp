/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/ast/mapper/CompleteSyntaxMapper.h>

TOMIC_BEGIN

CompleteSyntaxMapper::CompleteSyntaxMapper()
{
    _Init();
}

const char* CompleteSyntaxMapper::Description(SyntaxType type) const
{
    auto iter = _typeToDescription.find(type);
    if (iter == _typeToDescription.end())
    {
        return "Missing";
    }
    return iter->second;
}

void CompleteSyntaxMapper::_Init()
{
    _typeToDescription[SyntaxType::ST_UNKNOWN] = "Unknown";
    _typeToDescription[SyntaxType::ST_EPSILON] = "Epsilon";
    _typeToDescription[SyntaxType::ST_TERMINATOR] = "Terminator";

    _typeToDescription[SyntaxType::ST_COMP_UNIT] = "CompUnit";

    _typeToDescription[SyntaxType::ST_DECL] = "Decl";
    _typeToDescription[SyntaxType::ST_BTYPE] = "BType";
    _typeToDescription[SyntaxType::ST_CONST_DECL] = "ConstDecl";
    _typeToDescription[SyntaxType::ST_CONST_DEF] = "ConstDef";
    _typeToDescription[SyntaxType::ST_CONST_INIT_VAL] = "ConstInitVal";
    _typeToDescription[SyntaxType::ST_VAR_DECL] = "VarDecl";
    _typeToDescription[SyntaxType::ST_VAR_DEF] = "VarDef";
    _typeToDescription[SyntaxType::ST_INIT_VAL] = "InitVal";

    _typeToDescription[SyntaxType::ST_FUNC_DEF] = "FuncDef";
    _typeToDescription[SyntaxType::ST_FUNC_TYPE] = "FuncType";
    _typeToDescription[SyntaxType::ST_FUNC_FPARAMS] = "FuncFParams";
    _typeToDescription[SyntaxType::ST_FUNC_FPARAM] = "FuncFParam";
    _typeToDescription[SyntaxType::ST_FUNC_APARAMS] = "FuncAParams";
    _typeToDescription[SyntaxType::ST_FUNC_APARAM] = "FuncAParam";
    _typeToDescription[SyntaxType::ST_BLOCK] = "Block";
    _typeToDescription[SyntaxType::ST_BLOCK_ITEM] = "BlockItem";

    _typeToDescription[SyntaxType::ST_MAIN_FUNC_DEF] = "MainFuncDef";

    _typeToDescription[SyntaxType::ST_STMT] = "Stmt";
    _typeToDescription[SyntaxType::ST_ASSIGNMENT_STMT] = "AssignmentStmt";
    _typeToDescription[SyntaxType::ST_LVAL] = "LVal";
    _typeToDescription[SyntaxType::ST_COND] = "Cond";
    _typeToDescription[SyntaxType::ST_IF_STMT] = "IfStmt";
    _typeToDescription[SyntaxType::ST_FOR_STMT] = "ForStmt";
    _typeToDescription[SyntaxType::ST_FOR_INIT_STMT] = "ForInitStmt";
    _typeToDescription[SyntaxType::ST_FOR_STEP_STMT] = "ForStepStmt";
    _typeToDescription[SyntaxType::ST_EXP_STMT] = "ExpStmt";
    _typeToDescription[SyntaxType::ST_BREAK_STMT] = "BreakStmt";
    _typeToDescription[SyntaxType::ST_CONTINUE_STMT] = "ContinueStmt";
    _typeToDescription[SyntaxType::ST_RETURN_STMT] = "ReturnStmt";
    _typeToDescription[SyntaxType::ST_IN_STMT] = "InStmt";
    _typeToDescription[SyntaxType::ST_OUT_STMT] = "OutStmt";

    _typeToDescription[SyntaxType::ST_EXP] = "Exp";
    _typeToDescription[SyntaxType::ST_CONST_EXP] = "ConstExp";
    _typeToDescription[SyntaxType::ST_ADD_EXP] = "AddExp";
    _typeToDescription[SyntaxType::ST_MUL_EXP] = "MulExp";
    _typeToDescription[SyntaxType::ST_UNARY_EXP] = "UnaryExp";
    _typeToDescription[SyntaxType::ST_UNARY_OP] = "UnaryOp";
    _typeToDescription[SyntaxType::ST_PRIMARY_EXP] = "PrimaryExp";
    _typeToDescription[SyntaxType::ST_FUNC_CALL] = "FunctionCall";
    _typeToDescription[SyntaxType::ST_NUMBER] = "Number";
    _typeToDescription[SyntaxType::ST_OR_EXP] = "OrExp";
    _typeToDescription[SyntaxType::ST_AND_EXP] = "AndExp";
    _typeToDescription[SyntaxType::ST_EQ_EXP] = "EqExp";
    _typeToDescription[SyntaxType::ST_REL_EXP] = "RelExp";
}

TOMIC_END
