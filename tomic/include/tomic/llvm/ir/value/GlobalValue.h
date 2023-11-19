/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_GLOBAL_VALUE_H_
#define _TOMIC_LLVM_GLOBAL_VALUE_H_

#include <tomic/llvm/ir/value/Constant.h>
#include <tomic/llvm/ir/value/ValueType.h>
#include <tomic/llvm/Llvm.h>

TOMIC_LLVM_BEGIN

class GlobalValue : public Constant
{
    friend class Module;

public:
    ~GlobalValue() override = default;


    static bool classof(const ValueType type)
    {
        return type >= ValueType::GlobalValueTy;
    }


    void PrintUse(IAsmWriterPtr writer) override;
    void PrintName(IAsmWriterPtr writer) override;


    bool IsGlobalValue() const override { return true; }

    ModulePtr Parent() const { return _parent; }

private:
    void SetParent(ModulePtr parent) { _parent = parent; }

protected:
    GlobalValue(ValueType valueType, TypePtr type, const std::string& name)
        : Constant(valueType, type, nullptr), _parent(nullptr)
    {
        SetName(name);
    };

    ModulePtr _parent;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_GLOBAL_VALUE_H_
