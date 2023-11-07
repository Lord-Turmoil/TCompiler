/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/Module.h>
#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/value/Function.h>

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

TOMIC_LLVM_END
