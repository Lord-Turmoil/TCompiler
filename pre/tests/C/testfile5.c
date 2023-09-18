/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Expression Test
 */

// ==================== Decl

// ==================== FuncDef

int get_66() { return 66; }

void test_add_exp()
{
    // expression
    int a, b, c;
    a = 2 * 3;
    a = (1 + 2 * (3 + 4));
    b = a;
    a = 4;

    // unary exp
    a = get_66();
    printf("get_66(): %d\n", a);
    c = b + 1;
    b = a - 1;
    a = b * c;
    b = a / 3;
    a = b % 2;
    a = a + b * 2;
    b = a - b / 3;

    // unary op
    a = -b;
    b = +a;

    // something else
    (a * b) + c;
    (a * b) - c;
    (a + b);
}

void test_or_exp()
{
    int _true = 1, _false = 0;
    if (_true);
    if (!_false);

    if (_true < _false);
    if (_true <= _false);
    if (_true > _false);
    if (_true >= _false);

    if (_true == _false);
    if (_true != _false);

    if (_true == 1 && _false == 0);
    if (_false == 1 || _true == 0);
}

// ==================== MainFuncDef
int main()
{
    const int _ka = 1;
    const int _kb = 2, _kc = 3;
    int _a = 1;
    int _b = 2, _c = 3;
    int _d;

    printf("21371300\n");

    test_add_exp();
    test_or_exp();

    return 0;
}
