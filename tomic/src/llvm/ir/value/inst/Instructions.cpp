/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/value/inst/Instructions.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/LlvmContext.h>

#include <utility>

TOMIC_LLVM_BEGIN

/*
 * ============================== AllocaInst ==============================
 */
//
AllocaInst::AllocaInst(TypePtr type, int alignment)
    : Instruction(ValueType::AllocaInstTy, type->Context()->GetPointerType(type))
{
    _allocatedType = type;
    _alignment = alignment;
}


AllocaInstPtr AllocaInst::New(TypePtr type, int alignment)
{
    auto inst = std::shared_ptr<AllocaInst>(new AllocaInst(type, alignment));

    type->Context()->StoreValue(inst);

    return inst.get();
}


/*
 * ============================== LoadInst ==============================
 */

LoadInst::LoadInst(TypePtr type, ValuePtr address)
    : UnaryInstruction(ValueType::LoadInstTy, type, address),
    _address(address)
{
}


LoadInstPtr LoadInst::New(TypePtr type, ValuePtr address)
{
    auto inst = std::shared_ptr<LoadInst>(new LoadInst(type, address));

    type->Context()->StoreValue(inst);

    return inst.get();
}


LoadInstPtr LoadInst::New(ValuePtr address)
{
    TOMIC_ASSERT(address->GetType()->IsPointerTy() && "Address must be a pointer!");

    return New(address->GetType()->As<PointerType>()->ElementType(), address);
}


ValuePtr LoadInst::Address() const
{
    return _address;
}


/*
 * ============================== StoreInst ==============================
 */

StoreInst::StoreInst(ValuePtr value, ValuePtr address)
    : Instruction(ValueType::StoreInstTy, value->Context()->GetVoidTy())
{
    AddOperand(value);
    AddOperand(address);
}


StoreInstPtr StoreInst::New(ValuePtr value, ValuePtr address)
{
    auto inst = std::shared_ptr<StoreInst>(new StoreInst(value, address));

    value->Context()->StoreValue(inst);

    return inst.get();
}


/*
 * ============================== ReturnInst ==============================
 */

ReturnInst::ReturnInst(TypePtr type) :
    Instruction(ValueType::ReturnInstTy, type),
    _value(nullptr)
{
}


ReturnInst::ReturnInst(TypePtr type, ValuePtr value)
    : Instruction(ValueType::ReturnInstTy, type),
    _value(value)
{
    if (!value->GetType()->IsVoidTy())
    {
        AddOperand(value);
    }
}


ReturnInstPtr ReturnInst::New(LlvmContextPtr context)
{
    auto type = Type::GetVoidTy(context);
    auto inst = std::shared_ptr<ReturnInst>(new ReturnInst(type));

    context->StoreValue(inst);

    return inst.get();
}


ReturnInstPtr ReturnInst::New(LlvmContextPtr context, ValuePtr value)
{
    auto type = Type::GetVoidTy(context);
    auto inst = std::shared_ptr<ReturnInst>(new ReturnInst(type, value));

    context->StoreValue(inst);

    return inst.get();
}


/*
 * ============================== CallInst ==============================
 */

CallInst::CallInst(FunctionPtr function)
    : Instruction(ValueType::CallInstTy, function->GetType()->As<FunctionType>()->ReturnType()),
    _function(function)
{
    AddOperand(_function);
}


CallInst::CallInst(FunctionPtr function, std::vector<ValuePtr> parameters)
    : Instruction(ValueType::CallInstTy, function->GetType()->As<FunctionType>()->ReturnType()),
    _function(function), _parameters(std::move(parameters))
{
    AddOperand(_function);
    for (auto param : _parameters)
    {
        AddOperand(param);
    }
}


CallInstPtr CallInst::New(FunctionPtr function)
{
    auto inst = std::shared_ptr<CallInst>(new CallInst(function));

    function->Context()->StoreValue(inst);

    return inst.get();
}


CallInstPtr CallInst::New(FunctionPtr function, std::vector<ValuePtr> parameters)
{
    auto inst = std::shared_ptr<CallInst>(new CallInst(function, std::move(parameters)));

    function->Context()->StoreValue(inst);

    return inst.get();
}


TOMIC_LLVM_END
