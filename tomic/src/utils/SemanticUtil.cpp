/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/utils/SemanticUtil.h>
#include <tomic/utils/StringUtil.h>
#include <tomic/parser/ast/SyntaxNode.h>

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
// 1,2,3;4,5,6;7,8,9
std::stirng SerializeArray(const std::vector<std::vector<int>>& array)
{

}

std::vector<std::vector<int>> DeserializeArray(const char* str);

}

TOMIC_END
