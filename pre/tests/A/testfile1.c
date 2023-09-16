/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

// ==================== Decl
int should_be_66 = 66;

// ==================== FuncDef

// this function has side effect, use in short circuit
int side_effect()
{
    should_be_66 = 99;
    return 1;
}

int get_product(int arr[], int size)
{
    int i;
    int result = 1;
    for (i = 0; i < size; i = i + 1)
    {
        result = result * arr[i];
    }
    return result;
}

void print_array(int arr[], int size)
{
    int i = 0;
    for (i = 0; i < size; i = i + 1)
    {
        printf("%d", arr[i]);
        if (i != size - 1)
            printf(", ");
    }
    printf("\n");
}

// ==================== MainFuncDef
int main()
{
    printf("21371300\n");

    // short circuit
    int a = 66, b = 99;
    if (a - b + getint() && side_effect());
    else
    {
        printf("should be 66: %d\n", should_be_66);
    }

    if (a == 66 && (b - a) == 33 || side_effect())
        printf("should still be 66: %d\n", should_be_66);
    else
    {
    }

    // array
    const int SIZE = - + -5;
    int array[5] = { 1, 2, 3, 4, 5 };
    printf("5! should be 120: %d\n", get_product(array, SIZE));

    // arithmetic
    // 9 - 24 + 2 = -13
    int result = (1 + 2) * 3 - get_product(array, SIZE) / 5 + 8 % 3;
    printf("Arithmetic should be -13: %d\n", result);

    // assignment
    array[3] = SIZE - 1;
    array[4] = a;

    // loop and break
    int i = 0;
    while (array[i] * 5 < 20)
    {
        i = i + 1;
        if (i * 33 == should_be_66)
            break;
    }
    printf("i should be 2: %d\n", i);

    while (i < SIZE)
    {
        int a; // shadow
        a = getint();
        while (a != 0 && a != 66)
        {
            array[i] = a;
            a = getint();
        }
        if (a == 66)
            break;
    }
    print_array(array, SIZE);

    return 0;
}
