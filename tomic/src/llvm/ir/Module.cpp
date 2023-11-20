/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/Module.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/value/GlobalVariable.h>

TOMIC_LLVM_BEGIN

Module::~Module()
{
    if (_context)
    {
        delete _context;
    }
}


ModuleSmartPtr Module::New(const char* name)
{
    return std::shared_ptr<Module>(new Module(name));
}


Module::Module(const char* name)
    : _context(new LlvmContext()), _mainFunction(nullptr)
{
    if (name)
    {
        _name = name;
    }
    else
    {
        _name = "Default LLVM Module";
    }
}


void Module::AddGlobalVariable(GlobalVariablePtr globalVariable)
{
    globalVariable->SetParent(this);
    _globalVariables.push_back(globalVariable);
}


void Module::AddGlobalString(GlobalStringPtr globalString)
{
    globalString->SetParent(this);
    _globalStrings.push_back(globalString);
}


void Module::AddFunction(FunctionPtr function)
{
    function->SetParent(this);
    _functions.push_back(function);
}


void Module::SetMainFunction(FunctionPtr mainFunction)
{
    mainFunction->SetParent(this);
    _mainFunction = mainFunction;
}


TOMIC_LLVM_END
