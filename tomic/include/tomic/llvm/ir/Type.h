/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Reference:
 *   https://llvm.org/doxygen/classllvm_1_1Type.html
 *   https://llvm.org/doxygen/Type_8h_source.html
 */

#ifndef _TOMIC_LLVM_TYPE_H_
#define _TOMIC_LLVM_TYPE_H_

#include <tomic/llvm/Llvm.h>
#include <tomic/llvm/ir/IrForward.h>
#include <vector>
#include <tomic/llvm/asm/IAsmWriter.h>

TOMIC_LLVM_BEGIN

class Type
{
    friend class LlvmContext;
public:
    Type() : _typeId(VoidTyID) {}
    virtual ~Type() = default;

    /*
     * All implementations will be placed under llvm/asm.
     */
    virtual void PrintAsm(IAsmWriter writer);

    enum TypeID
    {
        // Primitive types
        VoidTyID,
        LabelTyID,

        // Derived types
        IntegerTyID,
        FunctionTyID,
        ArrayTyID,
        PointerTyID
    };

    TypeID TypeId() const { return _typeId; }
    LlvmContextPtr Context() const { return _context; }

    bool IsVoidTy() const { return _typeId == VoidTyID; }
    bool IsLabelTy() const { return _typeId == LabelTyID; }

    bool IsIntegerTy() const { return _typeId == IntegerTyID; }
    bool IsFunctionTy() const { return _typeId == FunctionTyID; }
    bool IsArrayTy() const { return _typeId == ArrayTyID; }
    bool IsPointerTy() const { return _typeId == PointerTyID; }

public:
    // Cast this type to a specific type.
    // You should use this function only when you are sure that this type is
    // actually the type you want to cast to.
    template<typename _Ty>
    _Ty* Cast() { return static_cast<_Ty*>(this); }

protected:
    // Prohibit direct instantiation.
    Type(LlvmContextPtr context, TypeID typeId) : _context(context), _typeId(typeId) {}

    int _ContainedTypeCount() const { return _containedTypes.size(); }
    void _AddContainedType(TypePtr type) { _containedTypes.push_back(type); }

    /*
     * A container for types contained by this type. For example, this includes
     * the arguments of a function type, the element type of array, etc.
     */
    std::vector<TypePtr> _containedTypes;

private:
    TypeID _typeId;
    LlvmContextPtr _context;
};

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_TYPE_H_
