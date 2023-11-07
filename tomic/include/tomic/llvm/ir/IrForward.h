/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_IR_FORWARD_H_
#define _TOMIC_IR_FORWARD_H_

#include <tomic/llvm/Llvm.h>
#include <memory>
#include <vector>

TOMIC_LLVM_BEGIN

class Type;
using TypePtr = Type*;

class Value;
using ValuePtr = Value*;
using ValueSmartPtr = std::shared_ptr<Value>;

class User;
using UserPtr = User*;

class Use;
using UsePtr = Use*;
using UseSmartPtr = std::shared_ptr<Use>;
using UseList = std::vector<UsePtr>;
using UseListPtr = UseList*;

class Argument;
using ArgumentPtr = Argument*;

class Function;
using FunctionPtr = Function*;

class BasicBlock;
using BasicBlockPtr = BasicBlock*;

TOMIC_LLVM_END

#endif // _TOMIC_IR_FORWARD_H_
