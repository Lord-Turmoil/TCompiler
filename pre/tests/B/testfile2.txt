/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

void print_1d(int arr[])
{
    printf("1D: %d %d\n", arr[0], arr[1]);
}

void print_2d(int arr[][4])  // [2][4]
{
    printf("2D: %d %d\n", arr[0][0], arr[1][3]);
}

int main()
{
    const int _kArrA[4] = { 1, 2, 3, 4 };
    const int _kArrB[2] = { 1, 2 }, _kArrC[2] = { 3, 4 };
    const int _kArrD[2][2 + 2] = {
            { 1,     2,     3, 4 },
            { 4 + 1, 2 * 3, 7, 8 }
    };

    int arrA[4] = { 1, 2, 3, 4 };
    int _arrB[2] = { 1, 2 }, _arrC[2] = { 3, 4 };
    int arrD[4 / 2][2 * 2 + 0] = {
            { 5 / 2 - 1, 2, 3, 4 },
            { 5,         6, 7, 8 }
    };
    int _arrE[3][3];

    printf("21371300\n");

    print_1d(arrA);
    print_2d(arrD);
    print_1d(arrD[0]);

    arrA[0] = 66;
    arrD[1 / 2][3 % 3] = 99;

    print_1d(arrA);
    print_2d(arrD);
    print_1d(arrD[0]);

    return 0;
}