/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/utils/SemanticUtil.h>
#include <tomic/utils/StringUtil.h>
#include <tomic/parser/ast/SyntaxNode.h>
#include <tomic/parser/table/SymbolTable.h>
#include <tomic/parser/table/SymbolTableBlock.h>
#include <tomic/parser/table/SymbolTableEntry.h>

TOMIC_BEGIN

namespace SemanticUtil
{

// Count the number of direct child nodes of a specific type.
int CountDirectChildNode(const SyntaxNodePtr node, SyntaxType type)
{
    int count = 0;
    for (auto child = node->FirstChild(); child; child = child->NextSibling())
    {
        if (child->Type() == type)
        {
            count++;
        }
    }
    return count;
}

int CountDirectTerminalNode(const SyntaxNodePtr node, TokenType type)
{
    int count = 0;
    for (auto child = node->FirstChild(); child; child = child->NextSibling())
    {
        if (child->IsTerminal() && child->Token()->type == type)
        {
            count++;
        }
    }
    return count;
}

// Get direct child.
static SyntaxNodePtr _FrontGetDirectChildNode(const SyntaxNodePtr node, SyntaxType type, int index);
static SyntaxNodePtr _RearGetDirectChildNode(const SyntaxNodePtr node, SyntaxType type, int index);
SyntaxNodePtr GetDirectChildNode(const SyntaxNodePtr node, SyntaxType type, int index)
{
    TOMIC_ASSERT(index != 0);

    if (node == nullptr)
    {
        return nullptr;
    }

    if (index > 0)
    {
        return _FrontGetDirectChildNode(node, type, index);
    }
    else
    {
        return _RearGetDirectChildNode(node, type, -index);
    }
}

static SyntaxNodePtr _FrontGetDirectChildNode(const SyntaxNodePtr node, SyntaxType type, int index)
{
    int i = 0;
    for (auto child = node->FirstChild(); child; child = child->NextSibling())
    {
        if (child->Type() == type)
        {
            i++;
            if (i == index)
            {
                return child;
            }
        }
    }
    return nullptr;
}

static SyntaxNodePtr _RearGetDirectChildNode(const SyntaxNodePtr node, SyntaxType type, int index)
{
    int i = 0;
    for (auto child = node->LastChild(); child; child = child->PrevSibling())
    {
        if (child->Type() == type)
        {
            i++;
            if (i == index)
            {
                return child;
            }
        }
    }
    return nullptr;
}

bool GetDirectChildNodes(const SyntaxNodePtr node, SyntaxType type, std::vector<SyntaxNodePtr>& list)
{
    bool found = false;
    for (auto child = node->FirstChild(); child; child = child->NextSibling())
    {
        if (child->Type() == type)
        {
            list.push_back(child);
            found = true;
        }
    }

    return found;
}

// Get child node recursively. No re-entering allowed.
static int _currentCount;
static SyntaxNodePtr _FrontGetChildNode(const SyntaxNodePtr node, SyntaxType type, int index);
static SyntaxNodePtr _RearGetChildNode(const SyntaxNodePtr node, SyntaxType type, int index);
SyntaxNodePtr GetChildNode(const SyntaxNodePtr node, SyntaxType type, int index)
{
    TOMIC_ASSERT(index != 0);

    if (node == nullptr)
    {
        return nullptr;
    }

    _currentCount = 0;

    if (index > 0)
    {
        return _FrontGetChildNode(node, type, index);
    }
    else
    {
        return _RearGetChildNode(node, type, -index);
    }
}


static SyntaxNodePtr _FrontGetChildNode(const SyntaxNodePtr node, SyntaxType type, int index)
{
    if (node->Type() == type)
    {
        _currentCount++;
        if (_currentCount == index)
        {
            return node;
        }
        return nullptr;
    }

    for (auto child = node->FirstChild(); child; child = child->NextSibling())
    {
        auto result = _FrontGetChildNode(child, type, index);
        if (result)
        {
            return result;
        }
    }

    return nullptr;
}

static SyntaxNodePtr _RearGetChildNode(const SyntaxNodePtr node, SyntaxType type, int index)
{
    if (node->Type() == type)
    {
        _currentCount++;
        if (_currentCount == index)
        {
            return node;
        }
        return nullptr;
    }

    for (auto child = node->LastChild(); child; child = child->PrevSibling())
    {
        auto result = _RearGetChildNode(child, type, index);
        if (result)
        {
            return result;
        }
    }

    return nullptr;
}

bool HasParent(const SyntaxNodePtr node, SyntaxType type)
{
    SyntaxNodePtr parent = node->Parent();
    while (parent)
    {
        if (parent->Type() == type)
        {
            return true;
        }
        parent = parent->Parent();
    }

    return false;
}

bool HasAttribute(const SyntaxNodePtr node, const char* name)
{
    if (node->HasAttribute(name))
    {
        return true;
    }

    for (auto child = node->FirstChild(); child; child = child->NextSibling())
    {
        if (HasAttribute(child, name))
        {
            return true;
        }
    }

    return false;
}

const char* GetAttribute(const SyntaxNodePtr node, const char* name, const char* defaultValue)
{
    const char* value;

    if (QueryAttribute(node, name, &value, defaultValue))
    {
        return value;
    }

    return defaultValue;
}

int GetIntAttribute(const SyntaxNodePtr node, const char* name, int defaultValue)
{
    int value;

    if (QueryIntAttribute(node, name, &value, defaultValue))
    {
        return value;
    }

    return defaultValue;
}

bool GetBoolAttribute(const SyntaxNodePtr node, const char* name, bool defaultValue)
{
    bool value;

    if (QueryBoolAttribute(node, name, &value, defaultValue))
    {
        return value;
    }

    return defaultValue;
}

bool QueryAttribute(const SyntaxNodePtr node, const char* name, const char** value, const char* defaultValue)
{
    if (node->HasAttribute(name))
    {
        if (value)
        {
            *value = node->Attribute(name, defaultValue);
        }
        return true;
    }

    for (auto child = node->FirstChild(); child; child = child->NextSibling())
    {
        if (QueryAttribute(child, name, value, defaultValue))
        {
            return true;
        }
    }

    return false;
}

bool QueryIntAttribute(const SyntaxNodePtr node, const char* name, int* value, int defaultValue)
{
    const char* attr;

    if (QueryAttribute(node, name, &attr, nullptr))
    {
        if (value)
        {
            if (!StringUtil::ToInt(attr, value))
            {
                *value = defaultValue;
            }
        }
        return true;
    }

    if (value)
    {
        *value = defaultValue;
    }

    return false;
}

bool QueryBoolAttribute(const SyntaxNodePtr node, const char* name, bool* value, bool defaultValue)
{
    const char* attr;

    if (QueryAttribute(node, name, &attr, nullptr))
    {
        if (value)
        {
            if (!StringUtil::ToBool(attr, value))
            {
                *value = defaultValue;
            }
        }
        return true;
    }

    if (value)
    {
        *value = defaultValue;
    }

    return false;
}

// Get Inherited Attribute value
bool HasInheritedAttribute(const SyntaxNodePtr node, const char* name)
{
    SyntaxNodePtr parent = node->Parent();
    while (parent)
    {
        if (parent->HasAttribute(name))
        {
            return true;
        }
        parent = parent->Parent();
    }

    return false;
}

const char* GetInheritedAttribute(const SyntaxNodePtr node, const char* name, const char* defaultValue)
{
    SyntaxNodePtr parent = node->Parent();
    while (parent)
    {
        if (parent->HasAttribute(name))
        {
            return parent->Attribute(name, defaultValue);
        }
        parent = parent->Parent();
    }

    return defaultValue;
}

int GetInheritedIntAttribute(const SyntaxNodePtr node, const char* name, int defaultValue)
{
    SyntaxNodePtr parent = node->Parent();
    while (parent)
    {
        if (parent->HasAttribute(name))
        {
            return parent->IntAttribute(name, defaultValue);
        }
        parent = parent->Parent();
    }

    return defaultValue;
}

bool GetInheritedBoolAttribute(const SyntaxNodePtr node, const char* name, bool defaultValue)
{
    SyntaxNodePtr parent = node->Parent();
    while (parent)
    {
        if (parent->HasAttribute(name))
        {
            return parent->BoolAttribute(name, defaultValue);
        }
        parent = parent->Parent();
    }

    return defaultValue;
}

// Get Synthesized Attribute value.
// Will look for it in front of the current node.
bool HasSynthesizedAttribute(const SyntaxNodePtr node, const char* name)
{
    if (HasAttribute(node, name))
    {
        return true;
    }

    if (node->PrevSibling())
    {
        return HasSynthesizedAttribute(node->PrevSibling(), name);
    }

    return false;
}

const char* GetSynthesizedAttribute(const SyntaxNodePtr node, const char* name, const char* defaultValue)
{
    const char* value;

    if (QueryAttribute(node, name, &value, defaultValue))
    {
        return value;
    }

    if (node->PrevSibling())
    {
        return GetSynthesizedAttribute(node->PrevSibling(), name, defaultValue);
    }

    return defaultValue;
}

int GetSynthesizedIntAttribute(const SyntaxNodePtr node, const char* name, int defaultValue)
{
    int value;

    if (QueryIntAttribute(node, name, &value, defaultValue))
    {
        return value;
    }

    if (node->PrevSibling())
    {
        return GetSynthesizedIntAttribute(node->PrevSibling(), name, defaultValue);
    }

    return defaultValue;
}

bool GetSynthesizedBoolAttribute(const SyntaxNodePtr node, const char* name, bool defaultValue)
{
    bool value;

    if (QueryBoolAttribute(node, name, &value, defaultValue))
    {
        return value;
    }

    if (node->PrevSibling())
    {
        return GetSynthesizedBoolAttribute(node->PrevSibling(), name, defaultValue);
    }

    return defaultValue;
}

// Array serialization
// The format is like this:
// 1,2,3;4,5,6;7,8,9;
std::string SerializeArray(const std::vector<std::vector<int>>& array)
{
    std::string str;
    for (auto& row : array)
    {
        for (auto& col : row)
        {
            str += std::to_string(col);
            if (col != row.back())
            {
                str += ',';
            }
        }
        str += ';';
    }
    return str;
}

std::vector<std::vector<int>> DeserializeArray(const char* str)
{
    std::vector<std::vector<int>> array;
    if (!str)
    {
        array.emplace_back();
        return array;
    }

    std::vector<int> row;
    int value = 0;

    while (*str != '\0')
    {
        if (*str == ',')
        {
            row.push_back(value);
            value = 0;
        }
        else if (*str == ';')
        {
            row.push_back(value);
            value = 0;
            array.emplace_back(row);
            row.clear();
        }
        else
        {
            value = value * 10 + (*str - '0');
        }
        str++;
    }

    return array;
}

// For format string.
int GetFormatStringArgCount(const char* format)
{
    int count = 0;
    while (*format != '\0')
    {
        if (*format == '%')
        {
            format++;
            if (*format == '%')
            {
                format++;
            }
            else
            {
                count++;
            }
        }
        else
        {
            format++;
        }
    }

    return count;
}

// Compile-time calculation.
typedef int (* BinaryFunc)(int, int);
static int _Add(int left, int right) { return left + right; }
static int _Sub(int left, int right) { return left - right; }
static int _Mul(int left, int right) { return left * right; }
static int _Div(int left, int right) { return left / right; }
static int _Mod(int left, int right) { return left % right; }
static int _And(int left, int right) { return left && right; }
static int _Or(int left, int right) { return left || right; }

static int _Less(int left, int right) { return left < right; }
static int _Greater(int left, int right) { return left > right; }
static int _LessEqual(int left, int right) { return left <= right; }
static int _GreaterEqual(int left, int right) { return left >= right; }
static int _Equal(int left, int right) { return left == right; }
static int _NotEqual(int left, int right) { return left != right; }

static BinaryFunc _GetBinaryEvaluator(const char* op);

typedef int (* UnaryFunc)(int);
static int _UnaryAdd(int value) { return value; }
static int _UnarySub(int value) { return -value; }
static int _Not(int value) { return !value; }
static UnaryFunc _GetUnaryEvaluator(const char* op);

int EvaluateBinary(const char* op, int left, int right)
{
    return _GetBinaryEvaluator(op)(left, right);
}

int EvaluateUnary(const char* op, int value)
{
    return _GetUnaryEvaluator(op)(value);
}

static BinaryFunc _GetBinaryEvaluator(const char* op)
{
    // For now, we do not differentiate && and &.
    switch (*op)
    {
    case '+':
        return _Add;
    case '-':
        return _Sub;
    case '*':
        return _Mul;
    case '/':
        return _Div;
    case '%':
        return _Mod;
    case '&':
        if (*(op + 1) == '&')
        {
            return _And;
        }
        else
        {
            TOMIC_ASSERT(false && "Invalid operator");
            return _Add;
        }
    case '|':
        if (*(op + 1) == '|')
        {
            return _Or;
        }
        else
        {
            TOMIC_ASSERT(false && "Invalid operator");
            return _Add;
        }
    case '<':
        if (*(op + 1) == '=')
        {
            return _LessEqual;
        }
        else
        {
            return _Less;
        }
    case '>':
        if (*(op + 1) == '=')
        {
            return _GreaterEqual;
        }
        else
        {
            return _Greater;
        }
    case '=':
        if (*(op + 1) == '=')
        {
            return _Equal;
        }
        else
        {
            TOMIC_ASSERT(false && "Invalid operator");
            return _Add;
        }
    case '!':
        if (*(op + 1) == '=')
        {
            return _NotEqual;
        }
        else
        {
            TOMIC_ASSERT(false && "Invalid operator");
            return _Add;
        }
    default:
        TOMIC_ASSERT(false && "Invalid operator");
        return _Add;
    }
}

static UnaryFunc _GetUnaryEvaluator(const char* op)
{
    switch (*op)
    {
    case '+':
        return _UnaryAdd;
    case '-':
        return _UnarySub;
    case '!':
        return _Not;
    default:
        TOMIC_ASSERT(false && "Invalid operator");
        return _UnaryAdd;
    }
}

int EvaluateNumber(const SyntaxNodePtr node)
{
    const char* number = node->FirstChild()->Token()->lexeme.c_str();
    int value;
    if (StringUtil::ToInt(number, &value))
    {
        return value;
    }
    else
    {
        TOMIC_ASSERT(false && "Invalid number");
        return 0;
    }
}

bool TryEvaluateLVal(const SyntaxNodePtr node, SymbolTableBlockPtr block, int* value)
{
    if (node->IntAttribute("dim", -1) != 0)
    {
        return false;
    }

    const char* name = node->FirstChild()->Token()->lexeme.c_str();
    auto rawEntry = block->FindEntry(name);

    if ((!rawEntry) || (rawEntry->EntryType() != SymbolTableEntryType::ET_CONSTANT))
    {
        return false;
    }

    auto entry = std::static_pointer_cast<ConstantEntry>(rawEntry);
    int dim = entry->Dimension();
    if (dim == 0)
    {
        *value = entry->Value();
        return true;
    }
    else if (dim == 1)
    {
        auto index = GetDirectChildNode(node, SyntaxType::ST_EXP);
        if (!index->BoolAttribute("det"))
        {
            return false;
        }
        *value = entry->Value(index->IntAttribute("value"));
        return true;
    }
    else if (dim == 2)
    {
        auto index1 = GetDirectChildNode(node, SyntaxType::ST_EXP, 1);
        auto index2 = GetDirectChildNode(node, SyntaxType::ST_EXP, 2);
        if (!index1->BoolAttribute("det") || !index2->BoolAttribute("det"))
        {
            return false;
        }
        *value = entry->Value(index1->IntAttribute("value"), index2->IntAttribute("value"));
        return true;
    }

    return false;
}

}

TOMIC_END
