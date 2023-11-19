/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/DerivedTypes.h>
#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/value/ConstantData.h>

TOMIC_LLVM_BEGIN

ConstantDataPtr ConstantData::New(TypePtr type, int value)
{
    // Temporarily only support integer initializer.
    TOMIC_ASSERT(type->IsIntegerTy() && "Unsupported initializer type");

    auto data = std::shared_ptr<ConstantData>(new ConstantData(type, value));
    type->Context()->StoreValue(data);

    return data.get();
}


ConstantDataPtr ConstantData::New(std::vector<ConstantDataPtr> values)
{
    // Temporarily do not support an empty initializer list.
    TOMIC_ASSERT(!values.empty());

    auto type = ArrayType::Get(values[0]->GetType(), values.size());
    auto data = std::shared_ptr<ConstantData>(new ConstantData(type, std::move(values)));
    type->Context()->StoreValue(data);

    return data.get();
}


ConstantData::ConstantData(TypePtr type, int value)
    : Constant(ValueType::ConstantDataTy, type, nullptr), _isAllZero(value == 0), _value(value)
{

}


ConstantData::ConstantData(TypePtr type, std::vector<ConstantDataPtr> values)
    : Constant(ValueType::ConstantDataTy, type, nullptr), _values(std::move(values))
{
    _isAllZero = true;
    for (auto& value : _values)
    {
        if (!value->IsAllZero())
        {
            _isAllZero = false;
            break;
        }
    }
}


TOMIC_LLVM_END
