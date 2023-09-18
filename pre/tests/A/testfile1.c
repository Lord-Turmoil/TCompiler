/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

int should_be_66 = 66;

int side_effect()
{
    should_be_66 = 99;
    return 1;
}

int main()
{
    printf("21371300\n");

    int _true = 1, _false = 0;

    if (_true || side_effect())
    {
        printf("short circuit (||): %d\n", should_be_66);
    }
    if (_false && side_effect());
    else
    {
        printf("short circuit (&&): %d\n", should_be_66);
    }

    return 0;
}
