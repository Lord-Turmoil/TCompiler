/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

// no Decl and FuncDef
// if and 9 cases of for loop
int main()
{
    printf("21371300\n");

    ////////// if test
    // complete
    if (1 + 1 == 2)
    {
        printf("1 + 1 = 2\n");
    }
    else
    {
        printf("1 + 1 != 2 !!!\n");
    }

    // no else
    if (2 * 2 == 4)
    {
        printf("2 * 2 = 4\n");
    }
    if (1);


    ////////// for test
    int i = 0;
    for (i = 0; i < 3; i = i + 1)
    {
        i = i;
    }

    // missing one stmt
    i = 0;
    for (; i < 3; i = i + 1);
    for (i = 0;; i = i + 1)
    {
        if (i == 3) break;
    }
    for (i = 0; i < 3;)
        i = i + 1;

    // missing two stmt's
    i = 0;
    for (;; i = i + 1)
    {
        if (i == 3) break;
    }
    i = 0;
    for (; i < 3;)
    {
        i = i + 1;
    }
    for (i = 0;;)
    {
        if (i == 3) break;
        i = i + 1;
    }

    // missing three stmt's
    i = 0;
    for (;;)
    {
        if (i == 3) break;
        i = i + 1;
    }

    printf("9 for's\n");

    return 0;
}
