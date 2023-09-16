/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * For logic expressions.
 */

// ==================== Decl

/*
 * try to cover Cond/Exp
 */

const int _true = 1;
const int _false = 0;
const int _boolean[2] = { 0, 1 };

// ==================== FuncDef
int return_true()
{
    return _true;
}

int return_false()
{
    return _false;
}

// ==================== MainFuncDef
int main()
{
    printf("21371300\n");

    // RelExp
    if (_true)
    {
        printf("UnaryExp -> PrimaryExp -> LVal\n");
    }
    if ((((_true + _false))))
    {
        printf("UnaryExp -> PrimaryExp -> (Exp)\n");
    }
    if (4)
    {
        printf("UnaryExp -> PrimaryExp -> Number\n");
    }
    if (!return_false())
    {
        printf("UnaryExp -> Ident | UnaryOp\n");
    }

    if (_true != return_true())
    {
    }
    else
    {
        printf("OrExp -> AndExp -> EqExp -> EqExp\n");
    }

    // Rel -> Add
    if (3 * 2 + 4 * (5 + 6) * _boolean[0])
    {
        printf("RelExp -> AddExp\n");
    }

    // all RelExp
    if (3 * 2 > 66)
    {
    }
    else if (66 < 3 * 2 + 5)
    {
    }
    else if (_boolean[1] <= _boolean[0] * 66)
    {
    }
    else if (_boolean[0] >= _boolean[1] * 99 - 66)
    {
    }
    else
    {
        printf("OrExp -> AndExp -> EqExp -> RelExp\n");
    }

    if (_boolean[0] != _boolean[1] && _true != _false)
    {
        printf("And\n");
    }
    else
        ;
    if (_boolean[0] == _boolean[1] || _true != _false)
    {
        printf("Or\n");
    }

    return 0;
}
