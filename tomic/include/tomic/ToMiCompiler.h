/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_COMPILER_H_
#define _TOMIC_COMPILER_H_

#include <tomic/Config.h>

TOMIC_BEGIN

/*
 * This is the main class of ToMiCompiler.
 * Uses IoC to control compilation process.
 */
class ToMiCompilerImpl;


class ToMiCompiler
{
public:
    ToMiCompiler();
    ~ToMiCompiler();

    // Disable copy and move.
    ToMiCompiler(const ToMiCompiler&) = delete;
    ToMiCompiler(ToMiCompiler&&) = delete;
    ToMiCompiler operator=(const ToMiCompiler&) = delete;
    ToMiCompiler operator=(ToMiCompiler&&) = delete;

    // Configure the compiler.
    ToMiCompiler* Configure(ConfigPtr config);

    void Compile();

private:
    ToMiCompilerImpl* _impl;
};


TOMIC_END

#endif // _TOMIC_COMPILER_H_
