/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/ir/Initializer.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/DerivedTypes.h>

TOMIC_LLVM_BEGIN

InitializerPtr Initializer::New(TypePtr type, int value)
{
    return std::shared_ptr<Initializer>(new Initializer(type, value));
}

InitializerPtr Initializer::New(std::vector<InitializerPtr> values)
{
    return std::shared_ptr<Initializer>(new Initializer(values));
}

Initializer::Initializer(TypePtr type, int value)
        : _type(type), _value(value), _isAllZero(value == 0)
{
    // Temporarily only support integer initializer.
    TOMIC_ASSERT(type->IsIntegerTy() && "Unsupported initializer type");
}

Initializer::Initializer(std::vector<InitializerPtr> values)
        : _values(std::move(values))
{
    // Temporarily do not support an empty initializer list.
    TOMIC_ASSERT(!_values.empty());

    auto subType = _values[0]->GetType();
    _type = ArrayType::Get(subType, _values.size());

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
