/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_IR_FORWARD_H_
#define _TOMIC_IR_FORWARD_H_

#include <tomic/llvm/Llvm.h>

TOMIC_LLVM_BEGIN

class Type;
using TypePtr = Type*;

class Value;
using ValuePtr = Value*;

class User;
using UserPtr = User*;

TOMIC_LLVM_END

#endif // _TOMIC_IR_FORWARD_H_
