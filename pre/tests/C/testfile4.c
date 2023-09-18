/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

// Function and declaration test.

void f1()
{
}

void f2()
{
    return;
}

int f3(int a)
{
    printf("a = %d\n", a);
    return a;
}

int f4(int a, int b)
{
    printf("%d + %d = %d\n", a, b, a + b);
    return a + b;
}

int main()
{
    printf("21371300\n");

    // io
    int input;
    input = getint();
    printf("input = %d\n", input);

    // functions
    f1();
    f2();
    int a;
    a = f3(3);
    int b;
    f4(a, 4);

    // block
    {
        // empty
    }
    {
        // single
        ;
    }
    {
        // multiple
        ;;
    }

    return 0;
}
