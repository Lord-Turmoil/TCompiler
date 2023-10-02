/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/parser/ast/SyntacticType.h"

TOMIC_BEGIN

SyntacticTypeMapper::SyntacticTypeMapper()
{
    _Init();
}

const char* SyntacticTypeMapper::Description(SyntacticType type) const
{
    auto iter = _typeToDescription.find(type);
    if (iter == _typeToDescription.end())
    {
        return "Missing";
    }
    return iter->second;
}

void SyntacticTypeMapper::_Init()
{
    _typeToDescription[SyntacticType::ST_UNKNOWN] = "Unknown";
    _typeToDescription[SyntacticType::ST_TERMINATOR] = "Terminator";
    _typeToDescription[SyntacticType::ST_IDENT] = "Ident";
    _typeToDescription[SyntacticType::ST_COMP_UNIT] = "CompUnit";
    _typeToDescription[SyntacticType::ST_DECL] = "Decl";
    _typeToDescription[SyntacticType::ST_BTYPE] = "BType";
    _typeToDescription[SyntacticType::ST_CONST_DECL] = "ConstDecl";
    _typeToDescription[SyntacticType::ST_CONST_DEF] = "ConstDef";
    _typeToDescription[SyntacticType::ST_CONST_INIT_VAL] = "ConstInitVal";
    _typeToDescription[SyntacticType::ST_VAR_DECL] = "VarDecl";
    _typeToDescription[SyntacticType::ST_VAR_DEF] = "VarDef";
    _typeToDescription[SyntacticType::ST_INIT_VAL] = "InitVal";
    _typeToDescription[SyntacticType::ST_FUNC_DEF] = "FuncDef";
    _typeToDescription[SyntacticType::ST_FUNC_TYPE] = "FuncType";
    _typeToDescription[SyntacticType::ST_FUNC_F_PARAMS] = "FuncFParams";
    _typeToDescription[SyntacticType::ST_FUNC_A_PARAMS] = "FuncAParams";
    _typeToDescription[SyntacticType::ST_BLOCK] = "Block";
    _typeToDescription[SyntacticType::ST_BLOCK_ITEM] = "BlockItem";
    _typeToDescription[SyntacticType::ST_MAIN_FUNC_DECL] = "MainFuncDecl";
    _typeToDescription[SyntacticType::ST_STMT] = "Stmt";
    _typeToDescription[SyntacticType::ST_LVAL] = "LVal";
    _typeToDescription[SyntacticType::ST_COND] = "Cond";
    _typeToDescription[SyntacticType::ST_IF_STMT] = "IfStmt";
    _typeToDescription[SyntacticType::ST_FOR_STMT] = "ForStmt";
    _typeToDescription[SyntacticType::ST_BREAK_STMT] = "BreakStmt";
    _typeToDescription[SyntacticType::ST_CONTINUE_STMT] = "ContinueStmt";
    _typeToDescription[SyntacticType::ST_RETURN_STMT] = "ReturnStmt";
    _typeToDescription[SyntacticType::ST_IN_STMT] = "InStmt";
    _typeToDescription[SyntacticType::ST_OUT_STMT] = "OutStmt";
    _typeToDescription[SyntacticType::ST_EXP] = "Exp";
    _typeToDescription[SyntacticType::ST_CONST_EXP] = "ConstExp";
    _typeToDescription[SyntacticType::ST_ADD_EXP] = "AddExp";
    _typeToDescription[SyntacticType::ST_MUL_EXP] = "MulExp";
    _typeToDescription[SyntacticType::ST_UNARY_EXP] = "UnaryExp";
    _typeToDescription[SyntacticType::ST_UNARY_OP] = "UnaryOp";
    _typeToDescription[SyntacticType::ST_PRIMARY_EXP] = "PrimaryExp";
    _typeToDescription[SyntacticType::ST_NUMBER] = "Number";
    _typeToDescription[SyntacticType::ST_OR_EXP] = "OrExp";
    _typeToDescription[SyntacticType::ST_AND_EXP] = "AndExp";
    _typeToDescription[SyntacticType::ST_EQ_EXP] = "EqExp";
    _typeToDescription[SyntacticType::ST_REL_EXP] = "RelExp";
}

TOMIC_END
