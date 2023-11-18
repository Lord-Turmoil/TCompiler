/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_MODULE_H_
#define _TOMIC_LLVM_MODULE_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/IrForward.h>
#include <string>
#include <vector>

TOMIC_LLVM_BEGIN

class Module final
{
public:
    ~Module();

    static ModuleSmartPtr New(const char* name);

    const char* Name() const { return _name.c_str(); }
    LlvmContextPtr Context() const { return _context; }

public:
    using global_iterator = std::vector<GlobalVariablePtr>::iterator;
    using function_iterator = std::vector<FunctionPtr>::iterator;

    global_iterator GlobalBegin() { return _globalVariables.begin(); }
    global_iterator GlobalEnd() { return _globalVariables.end(); }
    int GlobalCount() const { return _globalVariables.size(); }

    function_iterator FunctionBegin() { return _functions.begin(); }
    function_iterator FunctionEnd() { return _functions.end(); }
    int FunctionCount() const { return _functions.size(); }

    FunctionPtr GetMainFunction() const { return _mainFunction; }

public:
    void AddGlobalVariable(GlobalVariablePtr globalVariable);
    void AddFunction(FunctionPtr function);
    void SetMainFunction(FunctionPtr mainFunction);

private:
    Module(const char* name);

    std::string _name;
    LlvmContextPtr _context;

    // These will be managed by LlvmContext. So we don't need to delete them.
    std::vector<GlobalVariablePtr> _globalVariables;
    std::vector<FunctionPtr> _functions;
    FunctionPtr _mainFunction;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_MODULE_H_
