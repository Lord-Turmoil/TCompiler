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
#include <algorithm> // for std::min
#include <utility>

TOMIC_BEGIN

DefaultSemanticAnalyzer::DefaultSemanticAnalyzer(IErrorLoggerPtr errorLogger, ILoggerPtr logger)
        : _errorLogger(errorLogger), _logger(logger),
          _currentBlock(nullptr), _errorCandidate(nullptr)
{
    TOMIC_ASSERT(_errorLogger);
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
    _nodeStack.push(node);

    if (_AnalyzePreamble(node))
    {
        auto action = mapper.GetEnterAction(node->Type());
        return (this->*action)(node);
    }

    return false;
}

bool DefaultSemanticAnalyzer::VisitExit(SyntaxNodePtr node)
{
    auto action = mapper.GetExitAction(node->Type());

    bool ret = (this->*action)(node);
    _nodeStack.pop();

    return ret;
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

bool DefaultSemanticAnalyzer::_AddToSymbolTable(SymbolTableEntryPtr entry)
{
    TOMIC_ASSERT(entry);
    TOMIC_ASSERT(_currentBlock);

    if (_currentBlock->FindLocalEntry(entry->Name()))
    {
        _Log(LogLevel::ERROR, "Redefinition of %s", entry->Name());
        _LogError(ErrorType::ERR_REDEFINED_SYMBOL, "Redefinition of %s", entry->Name());
        entry->AlterName(SymbolTableUtil::GetUniqueName());
        return false;
    }

    _currentBlock->AddEntry(entry);

    return true;
}

int DefaultSemanticAnalyzer::_ValidateConstSubscription(SyntaxNodePtr constExp)
{
    if (!constExp->BoolAttribute("det"))
    {
        _Log(LogLevel::ERROR, "Undetermined expression as subscription.");
        _LogError(ErrorType::ERR_UNKNOWN, "Undetermined expression as subscription.");
    }

    SymbolValueType type = static_cast<SymbolValueType>(SemanticUtil::GetSynthesizedIntAttribute(constExp, "type"));
    if (type != SymbolValueType::VT_INT)
    {
        _Log(LogLevel::ERROR, "Invalid subscription type: %d.", type);
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid subscription type: %d", type);
    }

    int size = constExp->IntAttribute("value");
    if (size < 0)
    {
        _Log(LogLevel::ERROR, "Invalid subscription size: %d", size);
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid subscription size: %d", size);
        size = 0;
    }

    return size;
}

void DefaultSemanticAnalyzer::_ValidateSubscription(SyntaxNodePtr exp)
{
    SymbolValueType type = static_cast<SymbolValueType>(SemanticUtil::GetSynthesizedIntAttribute(exp, "type"));
    if (type != SymbolValueType::VT_INT)
    {
        _Log(LogLevel::ERROR, "Invalid subscription type: %d", type);
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid subscription type: %d", type);
    }
}


/*
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 * Logging
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

void DefaultSemanticAnalyzer::_Log(LogLevel level, const char* format, ...)
{
    static char buffer[1024];

    auto node = _errorCandidate ? _errorCandidate : _nodeStack.top();
    auto terminator = SemanticUtil::GetChildNode(node, SyntaxType::ST_TERMINATOR);
    int line = terminator->Token()->lineNo;
    int column = terminator->Token()->charNo;

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    _logger->LogFormat(level, "(%d:%d) %s", line, column, buffer);
}

void DefaultSemanticAnalyzer::_LogError(ErrorType type, const char* format, ...)
{
    static char buffer[1024];

    auto node = _errorCandidate ? _errorCandidate : _nodeStack.top();
    auto terminator = SemanticUtil::GetChildNode(node, SyntaxType::ST_TERMINATOR);
    int line = terminator->Token()->lineNo;
    int column = terminator->Token()->charNo;

    va_list args;
    va_start(args, format);
    vsprintf(buffer, format, args);
    va_end(args);

    _errorLogger->LogFormat(line, column, type, buffer);
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
    SymbolValueType type = SymbolValueType::VT_INT;
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

    SyntaxNodePtr ident = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR);
    ConstantEntryBuilder builder(ident->Token()->lexeme.c_str());
    if (dim == 0)
    {
        builder.Type(static_cast<SymbolValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")));
    }
    else if (dim == 1)
    {
        int size = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP));
        builder.Type(static_cast<SymbolValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Size(size);
    }
    else if (dim == 2)
    {
        int size1 = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP));
        int size2 = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP, 2));

        builder.Type(static_cast<SymbolValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Size(size1, size2);
    }
    else
    {
        _Log(LogLevel::ERROR, "Invalid dimension: %d", dim);
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid dimension: %d", dim);
    }

    if (constInitVal->BoolAttribute("det"))
    {
        if (dim == 0)
        {
            builder.Value(constInitVal->IntAttribute("value"));
        }
        else
        {
            builder.Values(SemanticUtil::DeserializeArray(constInitVal->Attribute("values")));
        }
    }

    if (dim != constInitVal->IntAttribute("dim"))
    {
        _Log(LogLevel::ERROR, "Dimension mismatch: %d != %d", dim, constInitVal->IntAttribute("dim"));
        _LogError(ErrorType::ERR_UNKNOWN, "Dimension mismatch: %d != %d", dim, constInitVal->IntAttribute("dim"));
    }

    _AddToSymbolTable(builder.Build());

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
            node->SetIntAttribute("value", node->FirstChild()->IntAttribute("value"));
        }
        return true;
    }

    std::vector<SyntaxNodePtr> children;
    SemanticUtil::GetDirectChildNodes(node, SyntaxType::ST_CONST_INIT_VAL, children);
    int size = children.size();
    int childDim = children[0]->IntAttribute("dim");
    int childSize = children[0]->IntAttribute("size");
    bool det = true;

    for (auto& child : children)
    {
        if (child->IntAttribute("dim") != childDim)
        {
            _Log(LogLevel::ERROR, "Dimension mismatch: %d != %d", child->IntAttribute("dim"), childDim);
            _LogError(ErrorType::ERR_UNKNOWN, "Dimension mismatch: %d != %d", child->IntAttribute("dim"), childDim);
        }
        if (child->IntAttribute("size") != childSize)
        {
            _Log(LogLevel::ERROR, "Size mismatch: %d != %d", child->IntAttribute("size"), childSize);
            _LogError(ErrorType::ERR_UNKNOWN, "Size mismatch: %d != %d", child->IntAttribute("size"), childSize);
        }
        if (!child->BoolAttribute("det"))
        {
            det = false;
        }
    }

    int dim = childDim + 1;
    node->SetIntAttribute("dim", dim);
    node->SetIntAttribute("size", size);
    std::vector<std::vector<int>> values;
    if (det)
    {
        node->SetBoolAttribute("det", true);
        if (dim == 1)
        {
            values.emplace_back();
            for (auto& child : children)
            {
                values[0].push_back(child->IntAttribute("value"));
            }
        }
        else if (dim == 2)
        {
            for (auto& child : children)
            {
                values.emplace_back(SemanticUtil::DeserializeArray(child->Attribute("values"))[0]);
            }
        }
        node->SetAttribute("values", SemanticUtil::SerializeArray(values).c_str());
    }
    else
    {
        node->SetBoolAttribute("det", false);
        _Log(LogLevel::ERROR, "Undetermined value in const initial value.");
        _LogError(ErrorType::ERR_UNKNOWN, "Undetermined value in const initial value.");
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitVarDef(SyntaxNodePtr node)
{
    SyntaxNodePtr initVal = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_INIT_VAL);
    bool global = SemanticUtil::GetInheritedBoolAttribute(node, "global");

    if (global)
    {
        if (initVal && !initVal->BoolAttribute("det"))
        {
            _Log(LogLevel::ERROR, "Global variable should be initialized with const value.");
            _LogError(ErrorType::ERR_UNKNOWN, "Global variable should be initialized with const value.");
        }
    }

    int dim = SemanticUtil::CountDirectTerminalNode(node, TokenType::TK_LEFT_BRACKET);
    if (initVal)
    {
        if (dim != initVal->IntAttribute("dim"))
        {
            _Log(LogLevel::ERROR, "Dimension mismatch: %d != %d", dim, initVal->IntAttribute("dim"));
            _LogError(ErrorType::ERR_UNKNOWN, "Dimension mismatch: %d != %d", dim, initVal->IntAttribute("dim"));
        }
    }

    // For var, we don't need the value yet.

    SyntaxNodePtr ident = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR);
    VariableEntryPtr entry;
    VariableEntryBuilder builder(ident->Token()->lexeme.c_str());
    if (dim == 0)
    {
        entry = builder.Type(static_cast<SymbolValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Build();
    }
    else if (dim == 1)
    {
        int size = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP));
        entry = builder.Type(static_cast<SymbolValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Size(size)
                ->Build();
    }
    else if (dim == 2)
    {
        int size1 = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP));
        int size2 = _ValidateConstSubscription(SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP, 2));

        entry = builder.Type(static_cast<SymbolValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type")))
                ->Size(size1, size2)
                ->Build();
    }
    else
    {
        _Log(LogLevel::ERROR, "Invalid dimension: %d", dim);
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid dimension: %d", dim);
    }

    node->SetIntAttribute("dim", dim);

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
            node->SetIntAttribute("value", node->FirstChild()->IntAttribute("value"));
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
                _Log(LogLevel::ERROR, "Dimension mismatch: %d != %d", child->IntAttribute("dim"), dim);
                _LogError(ErrorType::ERR_UNKNOWN, "Dimension mismatch: %d != %d", child->IntAttribute("dim"), dim);
            }
            if (child->IntAttribute("size") != childSize)
            {
                _Log(LogLevel::ERROR, "Size mismatch: %d != %d", child->IntAttribute("size"), childSize);
                _LogError(ErrorType::ERR_UNKNOWN, "Size mismatch: %d != %d", child->IntAttribute("size"), childSize);
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
    // If failed to declare, skip it.
    if (node->BoolAttribute("bad"))
    {
        return true;
    }

    // Check return value of non-void function.
    SymbolValueType type = static_cast<SymbolValueType>(SemanticUtil::GetSynthesizedIntAttribute(node, "type"));
    if (type == SymbolValueType::VT_INT)
    {
        // Set error candidate to '}'.
        _errorCandidate = node->LastChild()->LastChild();

        auto lastStmt = SemanticUtil::GetChildNode(node, SyntaxType::ST_BLOCK_ITEM, -1);
        if (!lastStmt || !SemanticUtil::GetChildNode(lastStmt, SyntaxType::ST_RETURN_STMT))
        {
            _Log(LogLevel::ERROR, "Missing return statement.");
            _LogError(ErrorType::ERR_MISSING_RETURN_STATEMENT, "Missing return statement.");
        }

        _errorCandidate = nullptr;
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncDecl(tomic::SyntaxNodePtr node)
{
    SymbolValueType type = static_cast<SymbolValueType>(SemanticUtil::GetSynthesizedIntAttribute(node, "type"));

    node->SetIntAttribute("type", static_cast<int>(type));
    // Pull this attribute up. This parent must be a FuncDef.
    node->Parent()->SetIntAttribute("type", static_cast<int>(type));

    // Add function to symbol table.
    auto ident = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR);
    const char* name = ident->Token()->lexeme.c_str();
    FunctionEntryBuilder builder(name);
    builder.Type(type);
    auto params = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_FUNC_FPARAMS);
    if (params)
    {
        std::vector<SyntaxNodePtr> paramList;
        SemanticUtil::GetDirectChildNodes(params, SyntaxType::ST_FUNC_FPARAM, paramList);
        for (auto& param : paramList)
        {
            SymbolValueType paramType = static_cast<SymbolValueType>(param->IntAttribute("type"));
            int paramDim = param->IntAttribute("dim");
            const char* paramName = param->Attribute("name");
            int paramSize = param->IntAttribute("size"); // it may not exist, but is OK
            builder.AddParam(paramType, paramName, paramDim, paramSize);
        }
    }

    // If function failed to add to symbol table, we mark it as corrupted,
    // so that we can skip it.
    if (!_AddToSymbolTable(builder.Build()))
    {
        node->Parent()->SetBoolAttribute("bad", true);
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncType(SyntaxNodePtr node)
{
    auto tokenType = node->FirstChild()->Token()->type;
    SymbolValueType type;
    switch (tokenType)
    {
    case TokenType::TK_INT:
        type = SymbolValueType::VT_INT;
        break;
    case TokenType::TK_VOID:
        type = SymbolValueType::VT_VOID;
        break;
    default:
        type = SymbolValueType::VT_ANY;
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
    if (dim > 0)
    {
        node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_ARRAY));
    }
    else
    {
        node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_INT));
    }

    if (dim == 2)
    {
        auto constExp = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_CONST_EXP);
        if (constExp->BoolAttribute("det"))
        {
            int size = constExp->IntAttribute("value");
            if (size < 0)
            {
                _Log(LogLevel::ERROR, "Invalid size: %d", size);
                _LogError(ErrorType::ERR_UNKNOWN, "Invalid size: %d", size);
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
    auto type = static_cast<SymbolValueType>(exp->IntAttribute("type"));
    node->SetIntAttribute("type", static_cast<int>(type));

    if (type == SymbolValueType::VT_ARRAY)
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
        if (node->Parent()->BoolAttribute("bad"))
        {
            // The parent function declaration fails.
            return false;
        }

        // Then it must have a previous sibling of FuncDecl.
        auto funcFParams = SemanticUtil::GetDirectChildNode(node->PrevSibling(), SyntaxType::ST_FUNC_FPARAMS);
        if (funcFParams)
        {
            std::vector<std::pair<SyntaxNodePtr, VariableEntryPtr>> params;
            SymbolTableUtil::BuildParamVariableEntries(funcFParams, params);
            for (auto& param : params)
            {
                _errorCandidate = param.first;
                _AddToSymbolTable(param.second);
                _errorCandidate = nullptr;
            }
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
    node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_INT));
    return true;
}

bool DefaultSemanticAnalyzer::_ExitMainFuncDef(tomic::SyntaxNodePtr node)
{
    // Set error candidate to '}'.
    _errorCandidate = node->LastChild()->LastChild();

    auto lastStmt = SemanticUtil::GetChildNode(node, SyntaxType::ST_BLOCK_ITEM, -1);
    if (!lastStmt || !SemanticUtil::GetChildNode(lastStmt, SyntaxType::ST_RETURN_STMT))
    {
        _Log(LogLevel::ERROR, "Missing return statement.");
        _LogError(ErrorType::ERR_MISSING_RETURN_STATEMENT, "Missing return statement.");
    }

    _errorCandidate = nullptr;

    return true;
}

bool DefaultSemanticAnalyzer::_ExitAssignmentStmt(tomic::SyntaxNodePtr node)
{
    // Check LVal
    auto lval = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_LVAL);
    SymbolValueType type = static_cast<SymbolValueType>(lval->IntAttribute("type"));
    if (type != SymbolValueType::VT_INT)
    {
        _Log(LogLevel::ERROR, "Invalid lvalue of type %d", static_cast<int>(type));
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid lvalue of type %d", static_cast<int>(type));
        return true;
    }
    if (lval->BoolAttribute("const"))
    {
        _Log(LogLevel::ERROR, "Cannot assign to const");
        _LogError(ErrorType::ERR_ASSIGN_TO_CONST, "Cannot assign to const");
    }

    auto exp = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_EXP);
    if (type != static_cast<SymbolValueType>(SemanticUtil::GetSynthesizedIntAttribute(exp, "type")))
    {
        _Log(LogLevel::ERROR, "Type mismatch");
        _LogError(ErrorType::ERR_UNKNOWN, "Type mismatch");
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitLVal(tomic::SyntaxNodePtr node)
{
    SyntaxNodePtr ident = node->FirstChild();
    const char* name = ident->Token()->lexeme.c_str();
    SymbolTableEntryPtr rawEntry = _currentBlock->FindEntry(name);

    // In case any error occurs, we set the type to int by default.
    node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_INT));

    if (!rawEntry)
    {
        _Log(LogLevel::ERROR, "Undefined variable: %s", name);
        _LogError(ErrorType::ERR_UNDEFINED_SYMBOL, "Undefined variable: %s", name);
        node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_ANY));
        return true;
    }

    int expectedDim = 0;
    SymbolValueType type;
    int size;
    if (rawEntry->EntryType() == SymbolTableEntryType::ET_CONSTANT)
    {
        node->SetBoolAttribute("const", true);

        ConstantEntryPtr entry = std::static_pointer_cast<ConstantEntry>(rawEntry);
        type = entry->Type();
        expectedDim = entry->Dimension();
        if (expectedDim == 2)
        {
            size = entry->ArraySize(1);
        }
    }
    else if (rawEntry->EntryType() == SymbolTableEntryType::ET_VARIABLE)
    {
        VariableEntryPtr entry = std::static_pointer_cast<VariableEntry>(rawEntry);
        type = entry->Type();
        expectedDim = entry->Dimension();
        if (expectedDim == 2)
        {
            size = entry->ArraySize(1);
        }
    }
    else
    {
        // Not a variable, so undefined symbol should be reported.
        _Log(LogLevel::ERROR, "Undefined variable: %s", name);
        _LogError(ErrorType::ERR_UNDEFINED_SYMBOL, "Undefined variable: %s", name);
        node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_ANY));
        return true;
    }

    // Now we get the correct LVal
    int actualDim = SemanticUtil::CountDirectChildNode(node, SyntaxType::ST_EXP);

    if (actualDim > expectedDim)
    {
        actualDim = expectedDim;
        _Log(LogLevel::ERROR, "Invalid dimension %d for %d", actualDim, expectedDim);
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid dimension %d for %d", actualDim, expectedDim);
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
    node->SetIntAttribute("dim", finalDim);
    if (finalDim == 0)
    {
        node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_INT));
    }
    else
    {
        node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_ARRAY));
        if (finalDim == 2)
        {
            node->SetIntAttribute("size", size);
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitCond(tomic::SyntaxNodePtr node)
{
    SymbolValueType type = static_cast<SymbolValueType>(SemanticUtil::GetSynthesizedIntAttribute(node, "type"));
    if (type != SymbolValueType::VT_INT)
    {
        _Log(LogLevel::ERROR, "Wrong type for condition");
        _LogError(ErrorType::ERR_UNKNOWN, "Wrong type for condition");
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
    // Check LVal
    auto lval = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_LVAL);
    SymbolValueType type = static_cast<SymbolValueType>(lval->IntAttribute("type"));
    if (type != SymbolValueType::VT_INT)
    {
        _Log(LogLevel::ERROR, "Invalid lvalue of type %d", static_cast<int>(type));
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid lvalue of type %d", static_cast<int>(type));
        return true;
    }
    if (lval->BoolAttribute("const"))
    {
        _Log(LogLevel::ERROR, "Cannot assign to const");
        _LogError(ErrorType::ERR_ASSIGN_TO_CONST, "Cannot assign to const");
    }

    // Check Exp.
    auto exp = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_EXP);
    if (type != static_cast<SymbolValueType>(SemanticUtil::GetSynthesizedIntAttribute(exp, "type")))
    {
        _Log(LogLevel::ERROR, "Type mismatch");
        _LogError(ErrorType::ERR_UNKNOWN, "Type mismatch");
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitBreakStmt(tomic::SyntaxNodePtr node)
{
    if (!SemanticUtil::GetInheritedBoolAttribute(node, "loop"))
    {
        _Log(LogLevel::ERROR, "Break outside loop.");
        _LogError(ErrorType::ERR_ILLEGAL_BREAK, "Break outside loop.");
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitContinueStmt(tomic::SyntaxNodePtr node)
{
    if (!SemanticUtil::GetInheritedBoolAttribute(node, "loop"))
    {
        _Log(LogLevel::ERROR, "Continue outside loop.");
        _LogError(ErrorType::ERR_ILLEGAL_CONTINUE, "Continue outside loop.");
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitReturnStmt(tomic::SyntaxNodePtr node)
{
    auto exp = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_EXP);
    SymbolValueType type;
    if (exp)
    {
        type = static_cast<SymbolValueType>(exp->IntAttribute("type"));
    }
    else
    {
        type = SymbolValueType::VT_VOID;
    }
    node->SetIntAttribute("type", static_cast<int>(type));

    // Check return value in void function.
    SymbolValueType funcType = static_cast<SymbolValueType>(SemanticUtil::GetInheritedIntAttribute(node, "type"));
    // if (funcType == ValueType::VT_VOID && ((type != ValueType::VT_VOID) || exp))
    if (funcType == SymbolValueType::VT_VOID && (type != SymbolValueType::VT_VOID))
    {
        _Log(LogLevel::ERROR, "Return value in void function.");
        _LogError(ErrorType::ERR_RETURN_TYPE_MISMATCH, "Return value in void function.");
    }
    else if (funcType == SymbolValueType::VT_INT && type != SymbolValueType::VT_INT)
    {
        _Log(LogLevel::ERROR, "Return type mismatch.");
        _LogError(ErrorType::ERR_RETURN_TYPE_MISMATCH, "Return type mismatch.");
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitInStmt(tomic::SyntaxNodePtr node)
{
    auto lval = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_LVAL);
    SymbolValueType type = static_cast<SymbolValueType>(SemanticUtil::GetSynthesizedIntAttribute(lval, "type"));
    if (type != SymbolValueType::VT_INT)
    {
        _Log(LogLevel::ERROR, "Invalid lvalue of type %d", static_cast<int>(type));
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid lvalue of type %d", static_cast<int>(type));
    }
    if (lval->BoolAttribute("const"))
    {
        _Log(LogLevel::ERROR, "Cannot assign to const");
        _LogError(ErrorType::ERR_ASSIGN_TO_CONST, "Cannot assign to const");
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitOutStmt(tomic::SyntaxNodePtr node)
{
    auto formatStr = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR, 3);
    if (formatStr->Token()->type != TokenType::TK_FORMAT)
    {
        _Log(LogLevel::ERROR, "Invalid format string");
        _LogError(ErrorType::ERR_UNKNOWN, "Invalid format string");
        return true;
    }
    const char* format = formatStr->Token()->lexeme.c_str();

    int formatArgc = SemanticUtil::GetFormatStringArgCount(format);
    std::vector<SyntaxNodePtr> args;
    SemanticUtil::GetDirectChildNodes(node, SyntaxType::ST_EXP, args);
    int argc = args.size();

    if (argc != formatArgc)
    {
        _Log(LogLevel::ERROR, "printf count mismatch: %d != %d", argc, formatArgc);
        _LogError(ErrorType::ERR_PRINTF_EXTRA_ARGUMENTS, "printf argument count mismatch: %d != %d", argc, formatArgc);
    }
    for (auto& arg : args)
    {
        SymbolValueType type = static_cast<SymbolValueType>(arg->IntAttribute("type"));
        if (type != SymbolValueType::VT_INT)
        {
            _Log(LogLevel::ERROR, "Invalid argument type in OutStmt: %d", static_cast<int>(type));
            _LogError(ErrorType::ERR_ARGUMENT_TYPE_MISMATCH,
                      "Invalid argument type in OutStmt: %d",
                      static_cast<int>(type));
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_DefaultExitExp(SyntaxNodePtr node)
{
    // In case any error occurs, this node will have a default int type.
    node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_INT));

    if (node->HasManyChildren())
    {
        // Combine all children's type.
        auto left = node->FirstChild();
        SymbolValueType leftType = static_cast<SymbolValueType>(left->IntAttribute("type"));
        auto right = node->LastChild();
        SymbolValueType rightType = static_cast<SymbolValueType>(right->IntAttribute("type"));

        if (leftType != rightType)
        {
            _Log(LogLevel::ERROR, "Arithmetic type mismatch: %d != %d",
                 static_cast<int>(leftType),
                 static_cast<int>(rightType));
            _LogError(ErrorType::ERR_UNKNOWN, "Arithmetic type mismatch: %d != %d",
                      static_cast<int>(leftType),
                      static_cast<int>(rightType));
        }
        else
        {
            if (leftType == SymbolValueType::VT_ARRAY)
            {
                _Log(LogLevel::ERROR, "Array cannot be operated.");
                _LogError(ErrorType::ERR_UNKNOWN, "Array cannot be operated.");
            }
            else
            {
                node->SetIntAttribute("type", static_cast<int>(leftType));
                if (left->BoolAttribute("det") && right->BoolAttribute("det"))
                {
                    node->SetBoolAttribute("det", true);

                    int leftValue = left->IntAttribute("value");
                    int rightValue = right->IntAttribute("value");
                    auto opNode = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_TERMINATOR);
                    const char* op = opNode->Token()->lexeme.c_str();
                    int value = SemanticUtil::EvaluateBinary(op, leftValue, rightValue);

                    node->SetIntAttribute("value", value);
                }
            }
        }
    }
    else
    {
        // Simple get the type from its single child.
        SymbolValueType type = static_cast<SymbolValueType>(node->FirstChild()->IntAttribute("type"));
        node->SetIntAttribute("type", static_cast<int>(type));
        if (type == SymbolValueType::VT_ARRAY)
        {
            int dim = node->FirstChild()->IntAttribute("dim");
            node->SetIntAttribute("dim", dim);
            if (node->IntAttribute("dim") == 2)
            {
                node->SetIntAttribute("size", node->FirstChild()->IntAttribute("size"));
            }
        }
        else if (type == SymbolValueType::VT_INT)
        {
            if (node->FirstChild()->BoolAttribute("det"))
            {
                node->SetBoolAttribute("det", true);
                node->SetIntAttribute("value", node->FirstChild()->IntAttribute("value"));
            }
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitExp(SyntaxNodePtr node)
{
    auto child = node->FirstChild();
    SymbolValueType type = static_cast<SymbolValueType>(child->IntAttribute("type"));

    node->SetIntAttribute("type", static_cast<int>(type));

    if (child->BoolAttribute("det"))
    {
        node->SetBoolAttribute("det", true);
        node->SetIntAttribute("value", child->IntAttribute("value"));
    }
    else
    {
        if (type == SymbolValueType::VT_ARRAY)
        {
            node->SetIntAttribute("dim", child->IntAttribute("dim"));
            if (node->IntAttribute("dim") == 2)
            {
                node->SetIntAttribute("size", child->IntAttribute("size"));
            }
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_EnterConstExp(SyntaxNodePtr node)
{
    node->SetBoolAttribute("const", true);
    return true;
}

bool DefaultSemanticAnalyzer::_ExitConstExp(tomic::SyntaxNodePtr node)
{
    _ExitExp(node);

    if (!node->BoolAttribute("det"))
    {
        _Log(LogLevel::ERROR, "Undetermined const expression.");
        _LogError(ErrorType::ERR_UNKNOWN, "Undetermined const expression.");
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitUnaryExp(tomic::SyntaxNodePtr node)
{
    // Set a default type.
    node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_INT));

    if (node->HasManyChildren())
    {
        auto exp = node->LastChild();
        SymbolValueType type = static_cast<SymbolValueType>(exp->IntAttribute("type"));
        if (type != SymbolValueType::VT_INT)
        {
            _Log(LogLevel::ERROR, "Invalid operand type: %d", static_cast<int>(type));
            _LogError(ErrorType::ERR_UNKNOWN, "Invalid operand type: %d", static_cast<int>(type));
        }
        else if (exp->BoolAttribute("det"))
        {
            node->SetBoolAttribute("det", true);
            // Compile-time calculation
            int value = exp->IntAttribute("value");
            const char* op = node->FirstChild()->Attribute("op");

            // TODO: check '!' in non-condition statement.
            node->SetIntAttribute("value", SemanticUtil::EvaluateUnary(op, value));
        }
    }
    else
    {
        auto child = node->FirstChild();
        SymbolValueType type = static_cast<SymbolValueType>(child->IntAttribute("type"));
        node->SetIntAttribute("type", static_cast<int>(type));
        if (type == SymbolValueType::VT_INT)
        {
            if (child->BoolAttribute("det"))
            {
                node->SetBoolAttribute("det", true);
                node->SetIntAttribute("value", child->IntAttribute("value"));
            }
        }
        else if (type == SymbolValueType::VT_ARRAY)
        {
            node->SetIntAttribute("dim", child->IntAttribute("dim"));
            if (node->IntAttribute("dim") == 2)
            {
                node->SetIntAttribute("size", child->IntAttribute("size"));
            }
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitUnaryOp(tomic::SyntaxNodePtr node)
{
    node->SetAttribute("op", node->FirstChild()->Token()->lexeme.c_str());
    return true;
}

bool DefaultSemanticAnalyzer::_ExitPrimaryExp(tomic::SyntaxNodePtr node)
{
    node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_INT));

    SyntaxNodePtr child = node->FirstChild();
    if (node->HasManyChildren())
    {
        // Skip '('
        child = child->NextSibling();
    }

    if (child->Type() == SyntaxType::ST_LVAL)
    {
        SymbolValueType type = static_cast<SymbolValueType>(child->IntAttribute("type"));
        if (type != SymbolValueType::VT_INT)
        {
            node->SetIntAttribute("type", static_cast<int>(type));
            node->SetIntAttribute("dim", child->IntAttribute("dim"));
            if (node->IntAttribute("dim") == 2)
            {
                node->SetIntAttribute("size", child->IntAttribute("size"));
            }
        }
        else
        {
            int value;
            if (SemanticUtil::TryEvaluateLVal(child, _currentBlock, &value))
            {
                node->SetBoolAttribute("det", true);
                node->SetIntAttribute("value", value);
            }
        }
    }
    else if (child->Type() == SyntaxType::ST_NUMBER)
    {
        node->SetBoolAttribute("det", true);
        node->SetIntAttribute("value", child->IntAttribute("value"));
    }
    else    // Exp
    {
        SymbolValueType type = static_cast<SymbolValueType>(child->IntAttribute("type"));
        if (type != SymbolValueType::VT_INT)
        {
            node->SetIntAttribute("type", static_cast<int>(type));
            node->SetIntAttribute("dim", child->IntAttribute("dim"));
            if (node->IntAttribute("dim") == 2)
            {
                node->SetIntAttribute("size", child->IntAttribute("size"));
            }
        }
        else if (child->BoolAttribute("det"))
        {
            node->SetBoolAttribute("det", true);
            node->SetIntAttribute("value", child->IntAttribute("value"));
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitFuncCall(tomic::SyntaxNodePtr node)
{
    const char* name = node->FirstChild()->Token()->lexeme.c_str();
    auto rawEntry = _currentBlock->FindEntry(name);
    if (!rawEntry || (rawEntry->EntryType() != SymbolTableEntryType::ET_FUNCTION))
    {
        _Log(LogLevel::ERROR, "Undefined function: %s", name);
        _LogError(ErrorType::ERR_UNDEFINED_SYMBOL, "Undefined function: %s", name);
        node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_ANY));
        return true;
    }

    auto entry = std::static_pointer_cast<FunctionEntry>(rawEntry);

    node->SetIntAttribute("type", static_cast<int>(entry->Type()));

    // Check argc.
    int argc = SemanticUtil::GetSynthesizedIntAttribute(node, "argc");
    if (argc != entry->ArgsCount())
    {
        _Log(LogLevel::ERROR, "Argument count mismatch: %d != %d", argc, entry->ArgsCount());
        _LogError(ErrorType::ERR_ARGUMENT_COUNT_MISMATCH, "Argument count mismatch: %d != %d", argc,
                  entry->ArgsCount());
    }

    // Check argument type.
    int upper = std::min(argc, entry->ArgsCount());
    if (upper > 0)
    {
        std::vector<SyntaxNodePtr> args;
        auto params = SemanticUtil::GetDirectChildNode(node, SyntaxType::ST_FUNC_APARAMS);
        SemanticUtil::GetDirectChildNodes(params, SyntaxType::ST_FUNC_APARAM, args);
        for (int i = 0; i < upper; i++)
        {
            auto param = entry->Param(i);
            SymbolValueType argType = static_cast<SymbolValueType>(args[i]->IntAttribute("type"));
            if ((argType != param->type) && (argType != SymbolValueType::VT_ANY))
            {
                _Log(LogLevel::ERROR,
                     "Argument type mismatch: %d != %d",
                     static_cast<int>(argType),
                     static_cast<int>(param->type));
                _LogError(ErrorType::ERR_ARGUMENT_TYPE_MISMATCH,
                          "Argument type mismatch: %d != %d",
                          static_cast<int>(argType),
                          static_cast<int>(param->type));
                continue;
            }
            if (argType == SymbolValueType::VT_ARRAY)
            {
                if (args[i]->IntAttribute("dim") != param->dimension)
                {
                    _Log(LogLevel::ERROR,
                         "Argument dimension mismatch: %d != %d",
                         args[i]->IntAttribute("dim"),
                         param->dimension);
                    _LogError(ErrorType::ERR_ARGUMENT_TYPE_MISMATCH,
                              "Argument dimension mismatch: %d != %d",
                              args[i]->IntAttribute("dim"),
                              param->dimension);
                    continue;
                }
                if (param->dimension == 2)
                {
                    if (args[i]->IntAttribute("size") != param->size[1])
                    {
                        _Log(LogLevel::ERROR,
                             "Argument size mismatch: %d != %d",
                             args[i]->IntAttribute("size"),
                             param->size[1]);
                        _LogError(ErrorType::ERR_ARGUMENT_TYPE_MISMATCH,
                                  "Argument size mismatch: %d != %d",
                                  args[i]->IntAttribute("size"),
                                  param->size[1]);
                        continue;
                    }
                }
                if (SemanticUtil::GetSynthesizedBoolAttribute(args[i], "const"))
                {
                    _Log(LogLevel::ERROR, "Cannot pass const array as argument.");
                    _LogError(ErrorType::ERR_ARGUMENT_TYPE_MISMATCH, "Cannot pass const array as argument.");
                }
            }
        }
    }

    return true;
}

bool DefaultSemanticAnalyzer::_ExitNumber(tomic::SyntaxNodePtr node)
{
    node->SetIntAttribute("type", static_cast<int>(SymbolValueType::VT_INT));
    node->SetBoolAttribute("det", true);

    int value;
    if (!StringUtil::ToInt(node->FirstChild()->Token()->lexeme.c_str(), &value))
    {
        value = 0;
    }
    node->SetIntAttribute("value", value);

    return true;
}

TOMIC_END
