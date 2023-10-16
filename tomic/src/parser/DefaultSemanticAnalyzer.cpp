/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/impl/DefaultSemanticAnalyzer.h>
#include <tomic/parser/ast/SyntaxNode.h>
#include <tomic/parser/table/SymbolTableEntry.h>
#include <tomic/utils/SemanticUtil.h>
#include <tomic/utils/StringUtil.h>
#include <tomic/utils/SymbolTableUtil.h>

TOMIC_BEGIN

DefaultSemanticAnalyzer::DefaultSemanticAnalyzer(IErrorLoggerPtr logger)
        : _logger(logger), _currentBlock(nullptr)
{
    TOMIC_ASSERT(_logger);
}

SymbolTablePtr DefaultSemanticAnalyzer::Analyze(SyntaxTreePtr tree)
{
    TOMIC_ASSERT(tree);
    _table = SymbolTable::New();

    tree->Accept(this);

    return _table;
}


/*
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Basic Visit Methods
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

// Not that good, since a global variable is used.
static DefaultSemanticAnalyzerActionMapper mapper;

bool DefaultSemanticAnalyzer::VisitEnter(SyntaxNodePtr node)
{
    if (_AnalyzePreamble(node))
    {
        auto action = mapper.GetEnterAction(node->Type());
        if (action)
        {
            return (this->*action)(node);
        }
        return _DefaultEnter(node);
    }

    return true;
}

bool DefaultSemanticAnalyzer::VisitExit(SyntaxNodePtr node)
{
    auto action = mapper.GetExitAction(node->Type());

    if (action)
    {
        return (this->*action)(node);
    }

    return _DefaultExit(node);
}

// Not sure if Visit is needed here.
bool DefaultSemanticAnalyzer::Visit(SyntaxNodePtr node)
{
    return _AnalyzePreamble(node);
}


/*
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Utility functions
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

bool DefaultSemanticAnalyzer::_AnalyzePreamble(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);
    TOMIC_ASSERT(_table);

    bool corrupted = node->BoolAttribute("corrupted");

    return !corrupted;
}

SymbolTableBlockPtr DefaultSemanticAnalyzer::_GetOrCreateBlock(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);
    TOMIC_ASSERT(_table);

    int blockId = node->IntAttribute("tbl", -1);
    if (blockId != -1)
    {
        return _table->GetBlock(blockId);
    }

    SymbolTableBlockPtr block;
    if (_currentBlock)
    {
        block = _currentBlock->NewChild();
        node->SetIntAttribute("tbl", block->Id());
    }
    else
    {
        block = _table->NewRoot();
        node->SetIntAttribute("tbl", block->Id());
    }

    _currentBlock = block;

    return block;
}

void DefaultSemanticAnalyzer::_AddToSymbolTable(SymbolTableEntryPtr entry)
{
    TOMIC_ASSERT(entry);
    TOMIC_ASSERT(_currentBlock);

    if (_currentBlock->FindLocalEntry(entry->Name()))
    {
        // TODO: Error handing - redefinition
        entry->AlterName(SymbolTableUtil::GetUniqueName());
    }

    _currentBlock->AddEntry(entry);
}

int DefaultSemanticAnalyzer::_ValidateConstSubscription(SyntaxNodePtr constExp)
{
    if (!constExp->BoolAttribute("det"))
    {
        // TODO: Error handing - undetermined expression as subscription.
    }

    ValueType type = static_cast<ValueType>(SemanticUtil::GetSynthesizedIntAttribute(constExp, "type"));
    if (type != ValueType::VT_INT)
    {
        // TODO: Error handing - invalid subscription type.
    }

    int size = constExp->IntAttribute("value");
    if (size < 0)
    {
        // TODO: Error handing - invalid init value.
        size = 0;
    }

    return size;
}

void DefaultSemanticAnalyzer::_ValidateSubscription(SyntaxNodePtr exp)
{
    ValueType type = static_cast<ValueType>(SemanticUtil::GetSynthesizedIntAttribute(exp, "type"));
    if (type != ValueType::VT_INT)
    {
        // TODO: Error handing - invalid subscription type.
    }
}


/*
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Specific Visit Methods
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

bool DefaultSemanticAnalyzer::_EnterCompUnit(SyntaxNodePtr node)
{
    _GetOrCreateBlock(node);
    return true;
}

bool DefaultSemanticAnalyzer::_ExitCompUnit(SyntaxNodePtr node)
{
    TOMIC_ASSERT(_currentBlock);
    _currentBlock = _currentBlock->Parent();
    return true;
}

bool DefaultSemanticAnalyzer::_EnterDecl(SyntaxNodePtr node)
{
    auto parent = node->Parent();

    if (parent->Type() == SyntaxType::ST_COMP_UNIT)
    {
        // Global variable.
        node->SetBoolAttribute("global", true);
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitBType(SyntaxNodePtr node)
{
    ValueType type = ValueType::VT_INT;
    node->SetIntAttribute("type", static_cast<int>(type));

    // Well, BType should have a parent.
    node->Parent()->SetIntAttribute("type", static_cast<int>(type));

    return true;
}

bool DefaultSemanticAnalyzer::_EnterConstDecl(SyntaxNodePtr node)
{
    node->SetBoolAttribute("const", true);
    return true;
}

bool DefaultSemanticAnalyzer::_ExitConstDef(SyntaxNodePtr node)
{
    int dim = SemanticUtil::CountDirectTerminalNode(node, TokenType::TK_LEFT_BRACKET);
    SyntaxNodePtr constInitVal = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_INIT_VAL);

    if (!constInitVal->BoolAttribute("det"))
    {
        // TODO: Error handing
        // const expression not determined
        return true;
    }

    if (dim != constInitVal->IntAttribute("dim"))
    {
        // TODO: Error handing
        // Wrong dimension.
        return true;
    }

    SyntaxNodePtr ident = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR);
    ConstantEntryPtr entry;
    ConstantEntryBuilder builder(ident->Token()->lexeme.c_str());
    if (dim == 0)
    {
        entry = builder.Type(static_cast<ValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Value(constInitVal->IntAttribute("value"))->Build();
    }
    else if (dim == 1)
    {
        int size = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP));
        entry = builder.Type(static_cast<ValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Size(size)
                ->Values(SemanticUtil::DeserializeArray(constInitVal->Attribute("values")))
                ->Build();
    }
    else if (dim == 2)
    {
        int size1 = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP));
        int size2 = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP, 2));

        entry = builder.Type(static_cast<ValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Size(size1, size2)
                ->Values(SemanticUtil::DeserializeArray(constInitVal->Attribute("values")))
                ->Build();
    }
    else
    {
        // TODO: Error handing.
    }

    _AddToSymbolTable(entry);

    return true;
}

bool DefaultSemanticAnalyzer::_ExitConstInitVal(SyntaxNodePtr node)
{
    if (node->FirstChild()->Type() == SyntaxType::ST_CONST_EXP)
    {
        node->SetIntAttribute("dim", 0);
        if (node->FirstChild()->BoolAttribute("det"))
        {
            node->SetBoolAttribute("det", true);
        }
    }
    else
    {
        int size = SemanticUtil::CountDirectChildNode(node, SyntaxType::ST_CONST_INIT_VAL);
        auto child = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_INIT_VAL);
        int dim = child->IntAttribute("dim");
        int childSize = child->IntAttribute("size");
        bool det = true;
        for (child = node->FirstChild(); child; child = child->NextSibling())
        {
            if (child->Type() != SyntaxType::ST_CONST_INIT_VAL)
            {
                continue;
            }
            if (child->IntAttribute("dim") != dim)
            {
                // TODO: Error handing - dimension mismatch
            }
            if (child->IntAttribute("size") != childSize)
            {
                // TODO: Error handing - size mismatch
            }
            if (!child->BoolAttribute("det"))
            {
                det = false;
            }
        }
        node->SetIntAttribute("dim", dim + 1);
        node->SetIntAttribute("size", size);
        if (det)
        {
            node->SetBoolAttribute("det", true);
        }
    }

    if (node->BoolAttribute("det"))
    {
        // TODO: Initialize the value.
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitVarDef(SyntaxNodePtr node)
{
    SyntaxNodePtr initVal = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_INIT_VAL);
    bool global = SemanticUtil::GetInheritedBoolAttribute(node, "global");

    if (global)
    {
        if (!initVal)
        {
            // TODO: Error - missing initial value
        }
        else if (!initVal->BoolAttribute("det"))
        {
            // TODO: Error - global var should be initialized with const
        }
    }

    int dim = SemanticUtil::CountDirectTerminalNode(node, TokenType::TK_LEFT_BRACKET);
    if (initVal)
    {
        if (dim != initVal->IntAttribute("dim"))
        {
            // TODO: Error handling - dimension mismatch
        }
    }

    // For var, we don't need the value yet.

    SyntaxNodePtr ident = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR);
    VariableEntryPtr entry;
    VariableEntryBuilder builder(ident->Token()->lexeme.c_str());
    if (dim == 0)
    {
        entry = builder.Type(static_cast<ValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Build();
    }
    else if (dim == 1)
    {
        int size = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP));
        entry = builder.Type(static_cast<ValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Size(size)
                ->Build();
    }
    else if (dim == 2)
    {
        int size1 = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP));
        int size2 = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP, 2));

        entry = builder.Type(static_cast<ValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Size(size1, size2)
                ->Build();
    }
    else
    {
        // TODO: Error handing.
    }

    // TODO: Add to symbol table.
    _AddToSymbolTable(entry);

    return true;
}

bool DefaultSemanticAnalyzer::_ExitInitVal(SyntaxNodePtr node)
{
    if (node->FirstChild()->Type() == SyntaxType::ST_EXP)
    {
        node->SetIntAttribute("dim", 0);
        if (node->FirstChild()->BoolAttribute("det"))
        {
            node->SetBoolAttribute("det", true);
        }
    }
    else
    {
        int size = SemanticUtil::CountDirectChildNode(node, SyntaxType::ST_INIT_VAL);
        auto child = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_INIT_VAL);
        int dim = child->IntAttribute("dim");
        int childSize = child->IntAttribute("size");
        bool det = true;
        for (child = node->FirstChild(); child; child = child->NextSibling())
        {
            if (child->Type() != SyntaxType::ST_CONST_INIT_VAL)
            {
                continue;
            }
            if (child->IntAttribute("dim") != dim)
            {
                // TODO: Error handing - dimension mismatch
            }
            if (child->IntAttribute("size") != childSize)
            {
                // TODO: Error handing - size mismatch
            }
            if (!child->BoolAttribute("det"))
            {
                det = false;
            }
        }
        node->SetIntAttribute("dim", dim + 1);
        node->SetIntAttribute("size", size);
        if (det)
        {
            node->SetBoolAttribute("det", true);
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncDef(SyntaxNodePtr node)
{
    // Check return value of non-void function.
    ValueType type = static_cast<ValueType>(node->IntAttribute("type"));
    if (type == ValueType::VT_INT)
    {
        auto lastStmt = SemanticUtil::GetChildNode(node, SyntaxType::ST_BLOCK_ITEM, -1);
        if (!lastStmt || !SemanticUtil::GetChildNode(lastStmt, SyntaxType::ST_RETURN_STMT))
        {
            // TODO: Error handing - missing return statement.
        }
    }

    // Add function to symbol table.
    auto ident = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR, 2);
    const char* name = ident->Token()->lexeme.c_str();
    auto params = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_FUNC_FPARAMS);
    std::vector<SyntaxNodePtr> paramList;
    SemanticUtil::GetDirectChildNodes(params, SyntaxType::ST_FUNC_FPARAM, paramList);

    FunctionEntryBuilder builder(name);
    builder.Type(type);
    for (auto& param : paramList)
    {
        ValueType paramType = static_cast<ValueType>(param->IntAttribute("type"));
        int paramDim = param->IntAttribute("dim");
        const char* paramName = param->Attribute("name");
        int paramSize = param->IntAttribute("size"); // it may not exist, but is OK
        builder.AddParam(paramType, paramName, paramDim, paramSize);
    }
    _AddToSymbolTable(builder.Build());

    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncType(SyntaxNodePtr node)
{
    auto tokenType = node->FirstChild()->Token()->type;
    ValueType type;
    switch (tokenType)
    {
    case TokenType::TK_INT:
        type = ValueType::VT_INT;
        break;
    case TokenType::TK_VOID:
        type = ValueType::VT_VOID;
        break;
    default:
        type = ValueType::VT_ANY;
        break;
    }

    node->SetIntAttribute("type", static_cast<int>(type));

    // The same as BType, FType should have a parent.
    node->Parent()->SetIntAttribute("type", static_cast<int>(type));

    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncFParams(SyntaxNodePtr node)
{
    int count = SemanticUtil::CountDirectChildNode(node, SyntaxType::ST_FUNC_FPARAM);
    node->SetIntAttribute("argc", count);

    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncFParam(SyntaxNodePtr node)
{
    auto ident = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR);
    node->SetAttribute("name", ident->Token()->lexeme.c_str());
    int dim = SemanticUtil::CountDirectTerminalNode(node, TokenType::TK_LEFT_BRACKET);
    node->SetIntAttribute("dim", dim);
    if (dim == 2)
    {
        auto constExp = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP);
        if (!constExp->BoolAttribute("det"))
        {
            // TODO: Error handing - undetermined expression as subscription.
        }
        else
        {
            int size = constExp->IntAttribute("value");
            if (size < 0)
            {
                // TODO: Error handing - invalid size.
            }
            else
            {
                node->SetIntAttribute("size", size);
            }
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncAParams(SyntaxNodePtr node)
{
    int argc = SemanticUtil::CountDirectChildNode(node, SyntaxType::ST_FUNC_APARAM);
    node->SetIntAttribute("argc", argc);
    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncAParam(SyntaxNodePtr node)
{
    auto exp = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_EXP);
    auto type = static_cast<ValueType>(exp->IntAttribute("type"));
    node->SetIntAttribute("type", static_cast<int>(type));

    if (type == ValueType::VT_ARRAY)
    {
        int dim = exp->IntAttribute("dim");
        node->SetIntAttribute("dim", dim);
        if (dim == 2)
        {
            node->SetIntAttribute("size", exp->IntAttribute("size"));
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_EnterBlock(SyntaxNodePtr node)
{
    _currentBlock = _GetOrCreateBlock(node);

    // Add function parameter if necessary
    if (node->Parent()->Type() == SyntaxType::ST_FUNC_DEF)
    {
        std::vector<VariableEntryPtr> params;
        SymbolTableUtil::BuildParamVariableEntries(node, params);
        for (auto& param : params)
        {
            _AddToSymbolTable(param);
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitBlock(tomic::SyntaxNodePtr node)
{
    TOMIC_ASSERT(_currentBlock);
    _currentBlock = _currentBlock->Parent();
    return true;
}

bool DefaultSemanticAnalyzer::_EnterMainFuncDef(tomic::SyntaxNodePtr node)
{
    node->SetIntAttribute("type", static_cast<int>(ValueType::VT_INT));
    return true;
}

bool DefaultSemanticAnalyzer::_ExitAssignmentStmt(tomic::SyntaxNodePtr node)
{
    auto exp = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_EXP);
    ValueType type = static_cast<ValueType>(SemanticUtil::GetSynthesizedIntAttribute(exp, "type"));

    if (type != ValueType::VT_INT)
    {
        // TODO: Error handling - Type mismatch
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitLVal(tomic::SyntaxNodePtr node)
{
    SyntaxNodePtr ident = node->FirstChild();
    const char* name = ident->Token()->lexeme.c_str();
    SymbolTableEntryPtr rawEntry = _currentBlock->FindEntry(name);
    if (!rawEntry)
    {
        // TODO: Error handing - undefined variable.
        return true;
    }
    if (rawEntry->EntryType() == SymbolTableEntryType::ET_CONSTANT)
    {
        // TODO: Error handing - constant cannot be assigned.
    }
    if (rawEntry->EntryType() != SymbolTableEntryType::ET_VARIABLE)
    {
        // TODO: Error handing - invalid lvalue.
    }

    // Now we get the correct LVal
    VariableEntryPtr entry = std::static_pointer_cast<VariableEntry>(rawEntry);
    int expectedDim = entry->Dimension();
    int actualDim = SemanticUtil::CountDirectChildNode(node, SyntaxType::ST_EXP);

    if (actualDim > expectedDim)
    {
        // TODO: Error handling - dimension mismatch.
    }

    /*
     *  actual  expected    final
     *    0       0           0
     *    0       1           1
     *    1       1           0
     *    0       2           2
     *    1       2           1
     *    2       2           0
     */
    int finalDim = expectedDim - actualDim;
    if (finalDim == 0)
    {
        node->SetIntAttribute("type", static_cast<int>(entry->EntryType()));
    }
    else
    {
        node->SetIntAttribute("type", static_cast<int>(ValueType::VT_ARRAY));
        node->SetIntAttribute("dim", finalDim);
        if (finalDim == 2)
        {
            node->SetIntAttribute("size", entry->ArraySize(1));
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitCond(tomic::SyntaxNodePtr node)
{
    ValueType type = static_cast<ValueType>(SemanticUtil::GetSynthesizedIntAttribute(node, "type"));
    if (type != ValueType::VT_INT)
    {
        // TODO: Error handing - invalid condition.
    }

    return true;
}

bool DefaultSemanticAnalyzer::_EnterForStmt(SyntaxNodePtr node)
{
    node->SetBoolAttribute("loop", true);
    return true;
}

bool DefaultSemanticAnalyzer::_ExitForInnerStmt(tomic::SyntaxNodePtr node)
{
    auto exp = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_EXP);
    ValueType type = static_cast<ValueType>(SemanticUtil::GetSynthesizedIntAttribute(exp, "type"));

    if (type != ValueType::VT_INT)
    {
        // TODO: Error handling - Type mismatch
    }

    return true;
}

TOMIC_END
