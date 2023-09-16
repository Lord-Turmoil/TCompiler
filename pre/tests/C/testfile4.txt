/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * For basic syntax.
 */

// ==================== Decl

const int _constA = 1;
const int _constB = 2, _constC = 3;

int _varA = 1;
int _varB = 2, _varC = 3;
int _varD;


// ==================== FuncDef
void _f0()
{
    printf("_f0()\n");
}

void _f1(int param1)
{
    printf("_f1(param1: %d)\n", param1);
}

int _f2(int param1, int param2)
{
    printf("_f2(param1: %d, param2: %d)\n", param1, param2);
    return param1 + param2;
}

// ==================== MainFuncDef
int main()
{
    printf("21371300\n");

    int n_11;
    n_11 = getint();
    int n_33 = n_11;

    int i;

    for (i = 0; i < 22; i = i + 1)
    {
        n_33 = n_33 + 1;
    }

    _f0();
    _f1(n_33);
    int n_66 = n_33 * _constB;
    int n_99 = _f2(n_33, n_66);
    printf("n_99: %d\n", n_99);

    // 1, 0
    while (n_11 > 0)
    {
        n_11 = n_11 / 10;
        printf("n_11: %d\n", n_11);
    }

    // for loop
    int temp = 1;
    for (; temp > 0; temp = temp - 1)
    {
        printf("temp: %d\n", temp);
        continue;
    }
    for (temp = 1; ; temp = temp - 1)
    {
        if (temp == 0)
            break;
    }
    for (temp = 0; ; )
    {
        break;
    }
    for (; ; )
    {
        printf("Empty for\n");
        break;
    }

    // trivial
    {
    };;;

    return 0;
}