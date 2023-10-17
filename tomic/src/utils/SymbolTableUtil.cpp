/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/utils/SymbolTableUtil.h>
#include <tomic/parser/ast/AstForward.h>
#include <tomic/parser/table/SymbolTableEntry.h>
#include <tomic/utils/SemanticUtil.h>


TOMIC_BEGIN

namespace SymbolTableUtil
{

// Since # is not a valid identifier, this function is safe.
std::string GetUniqueName()
{
    static int counter = 0;

    return std::string("#") + std::to_string(counter++);
}

void BuildParamVariableEntries(SyntaxNodePtr funcParams,
                               std::vector<std::pair<SyntaxNodePtr, VariableEntryPtr>>& list)
{
    if (funcParams == nullptr)
    {
        return;
    }

    std::vector<SyntaxNodePtr> params;
    SemanticUtil::GetDirectChildNodes(funcParams, SyntaxType::ST_FUNC_FPARAM, params);

    for (auto& param : params)
    {
        int dim = param->IntAttribute("dim");
        ValueType type = static_cast<ValueType>(param->IntAttribute("type"));
        VariableEntryBuilder builder(param->Attribute("name"));
        builder.Type(type);

        // Since the first dimension is of no use, so... just ignore it.
        if (dim == 1)
        {
            builder.Size(0);
        }
        else if (dim == 2)
        {
            builder.Size(0, param->IntAttribute("size"));
        }

        list.emplace_back(param, builder.Build());
    }
}

}

TOMIC_END
