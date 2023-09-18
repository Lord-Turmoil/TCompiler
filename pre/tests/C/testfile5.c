/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Expression Test
 */

// ==================== Decl

const int D1 = 2;
const int D2 = 4;

const int _kArrA[4] = { 1, 2, 3, 4 };
const int _kArrB[2] = { 1, 2 }, _kArrC[2] = { 3, 4 };
const int _kArrD[2][2 + 2] = {
        { 1,     2,     3, 4 },
        { 4 + 1, 2 * 3, 7, 8 }
};

int _arrA[4] = { 1, 2, 3, 4 };
int _arrB[2] = { 1, 2 }, _arrC[2] = { 3, 4 };
int _arrD[4 / 2][2 * 2 + 0] = {
        { 5 / 2 - 1, 2, 3, 4 },
        { 5,         6, 7, 8 }
};
int _arrE[3][3];

// ==================== FuncDef

int get_66() { return 66; }

void test_add_exp()
{
    // expression
    int a, b, c;
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
}

// ==================== MainFuncDef
int main()
{
    printf("21371300\n");

    test_add_exp();
    test_or_exp();

    return 0;
}
