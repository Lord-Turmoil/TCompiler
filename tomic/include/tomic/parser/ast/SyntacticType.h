/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYNTACTIC_TYPE_H_
#define _TOMIC_SYNTACTIC_TYPE_H_

#include "../../Common.h"
#include <unordered_map>

TOMIC_BEGIN

enum class SyntacticType
{
    ST_UNKNOWN,

    ST_TERMINATOR,
    ST_IDENT,

    ST_COMP_UNIT,

    // Decl
    ST_DECL, ST_BTYPE,
    ST_CONST_DECL, ST_CONST_DEF, ST_CONST_INIT_VAL,
    ST_VAR_DECL, ST_VAR_DEF, ST_INIT_VAL,

    // FuncDef
    ST_FUNC_DEF, ST_FUNC_TYPE,
    ST_FUNC_F_PARAMS, ST_FUNC_A_PARAMS,
    ST_BLOCK, ST_BLOCK_ITEM,

    // MainFuncDef
    ST_MAIN_FUNC_DECL,

    // Stmt
    ST_STMT,
    ST_LVAL,
    ST_COND,
    ST_IF_STMT, ST_FOR_STMT,
    ST_BREAK_STMT, ST_CONTINUE_STMT, ST_RETURN_STMT,
    ST_IN_STMT, ST_OUT_STMT,

    // Exp
    ST_EXP, ST_CONST_EXP,
    ST_ADD_EXP, ST_MUL_EXP,
    ST_UNARY_EXP, ST_UNARY_OP,
    ST_PRIMARY_EXP, ST_NUMBER,
    ST_OR_EXP, ST_AND_EXP,
    ST_EQ_EXP, ST_REL_EXP,

    ST_COUNT
};

class ISyntacticTypeMapper
{
public:
    virtual ~ISyntacticTypeMapper() = default;

    virtual const char* Description(SyntacticType type) const = 0;
};

class SyntacticTypeMapper : public ISyntacticTypeMapper
{
public:
    SyntacticTypeMapper();

    const char* Description(SyntacticType type) const override;
private:
    void _Init();
    std::unordered_map<SyntacticType, const char*> _typeToDescription;
};

TOMIC_END

#endif // _TOMIC_SYNTACTIC_TYPE_H_
