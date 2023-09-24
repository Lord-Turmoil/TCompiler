/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <cstdio>
#include <mioc/ServiceContainer.h>
#include <Twio.h>

int main()
{
    auto container = mioc::ServiceContainer::New();

    printf("Hello there!\n");

    return 0;
}
