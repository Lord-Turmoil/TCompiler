/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYNTAX_TYPE_H_
#define _TOMIC_SYNTAX_TYPE_H_

#include <tomic/Shared.h>
#include <tomic/Config.h>
#include <unordered_map>
#include <memory>

TOMIC_BEGIN

enum class SyntaxType
{
    ST_UNKNOWN,

    ST_EPSILON,
    ST_TERMINATOR,

    ST_COMP_UNIT,

    // Decl
    ST_DECL,
    ST_BTYPE,
    ST_CONST_DECL,
    ST_CONST_DEF,
    ST_CONST_INIT_VAL,
    ST_VAR_DECL,
    ST_VAR_DEF,
    ST_INIT_VAL,

    // FuncDef
    ST_FUNC_DEF,
    ST_FUNC_DECL,
    ST_FUNC_TYPE,
    ST_FUNC_FPARAMS,
    ST_FUNC_FPARAM,
    ST_FUNC_APARAMS,
    ST_FUNC_APARAM,
    ST_BLOCK,
    ST_BLOCK_ITEM,

    // MainFuncDef
    ST_MAIN_FUNC_DEF,

    // Stmt
    ST_STMT,
    ST_ASSIGNMENT_STMT,
    ST_LVAL,
    ST_COND,
    ST_IF_STMT,
    ST_FOR_STMT,
    ST_FOR_INIT_STMT,
    ST_FOR_STEP_STMT,
    ST_EXP_STMT,
    ST_BREAK_STMT,
    ST_CONTINUE_STMT,
    ST_RETURN_STMT,
    ST_IN_STMT,
    ST_OUT_STMT,

    // Exp
    ST_EXP,
    ST_CONST_EXP,
    ST_ADD_EXP,
    ST_MUL_EXP,
    ST_UNARY_EXP,
    ST_UNARY_OP,
    ST_PRIMARY_EXP,
    ST_FUNC_CALL,
    ST_NUMBER,
    ST_OR_EXP,
    ST_AND_EXP,
    ST_EQ_EXP,
    ST_REL_EXP,

    ST_COUNT
};


TOMIC_END

#endif // _TOMIC_SYNTAX_TYPE_H_
