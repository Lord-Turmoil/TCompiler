/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/parser/ast/SyntaxNode.h"
#include "../../../include/tomic/parser/ast/SyntaxTree.h"
#include "../../../include/tomic/parser/ast/AstVisitor.h"
#include "../../../include/tomic/utils/StringUtil.h"

TOMIC_BEGIN

SyntaxNode::SyntaxNode(SyntaxNodeType nodeType, SyntaxType type)
        : _nodeType(nodeType), _type(type),
          _tree(nullptr), _parent(nullptr), _prev(nullptr), _next(nullptr),
          _firstChild(nullptr), _lastChild(nullptr)
{
}

SyntaxNode::SyntaxNode(SyntaxNodeType nodeType, SyntaxType type, TokenPtr token)
        : _nodeType(nodeType), _type(type), _token(token),
          _tree(nullptr), _parent(nullptr), _prev(nullptr), _next(nullptr),
          _firstChild(nullptr), _lastChild(nullptr)
{
}

SyntaxNodePtr SyntaxNode::Root() const
{
    auto node = const_cast<SyntaxNode*>(this);
    while (node->_parent)
    {
        node = node->_parent;
    }
    return node;
}

/*
 * ==================== AST Links ====================
 */

SyntaxNodePtr SyntaxNode::InsertEndChild(SyntaxNodePtr child)
{
    _InsertChildPreamble(child);

    if (_lastChild)
    {
        _lastChild->_next = child;
        child->_prev = _lastChild;
        _lastChild = child;

        child->_next = nullptr;
    }
    else
    {
        _firstChild = _lastChild = child;

        child->_prev = nullptr;
        child->_next = nullptr;
    }
    child->_parent = this;

    return child;
}


SyntaxNodePtr SyntaxNode::InsertFirstChild(SyntaxNodePtr child)
{
    _InsertChildPreamble(child);

    if (_firstChild)
    {
        _firstChild->_prev = child;
        child->_next = _firstChild;
        _firstChild = child;

        child->_prev = nullptr;
    }
    else
    {
        _firstChild = _lastChild = child;

        child->_prev = nullptr;
        child->_next = nullptr;
    }

    child->_parent = this;

    return child;
}

SyntaxNodePtr SyntaxNode::InsertAfterChild(SyntaxNodePtr child, SyntaxNodePtr after)
{
    _InsertChildPreamble(child);

    if (!after)
    {
        return InsertFirstChild(child);
    }
    if (after == _lastChild)
    {
        return InsertEndChild(child);
    }

    child->_prev = after;
    child->_next = after->_next;
    // after is not the last child, so after->_next is not nullptr.
    after->_next->_prev = child;
    after->_next = child;

    child->_parent = this;

    return child;
}

SyntaxNodePtr SyntaxNode::RemoveChild(SyntaxNodePtr child)
{
    TOMIC_ASSERT(child);
    TOMIC_ASSERT(child->_tree == _tree);
    TOMIC_ASSERT(child->_parent == this);

    return _Unlink(child);
}

void SyntaxNode::_InsertChildPreamble(SyntaxNodePtr child)
{
    TOMIC_ASSERT(child);
    TOMIC_ASSERT(child->_tree == _tree);

    if (child->_parent)
    {
        child->_parent->_Unlink(child);
    }
}

SyntaxNodePtr SyntaxNode::_Unlink(SyntaxNodePtr child)
{
    TOMIC_ASSERT(child);
    TOMIC_ASSERT(child->_tree = _tree);
    TOMIC_ASSERT(child->_parent == this);

    if (child == _firstChild)
    {
        _firstChild = child->_next;
    }
    if (child == _lastChild)
    {
        _lastChild = child->_prev;
    }

    if (child->_prev)
    {
        child->_prev->_next = child->_next;
    }
    if (child->_next)
    {
        child->_next->_prev = child->_prev;
    }

    SyntaxNodePtr ret = child->_prev;

    child->_prev = nullptr;
    child->_next = nullptr;
    child->_parent = nullptr;

    return ret;
}

/*
 * ==================== AST Attributes ====================
 */

bool SyntaxNode::HasAttribute(const char* name) const
{
    return _attributes.find(name) != _attributes.end();
}

const char* SyntaxNode::Attribute(const char* name, const char* value) const
{
    auto it = _attributes.find(name);
    if (it != _attributes.end())
    {
        return it->second.c_str();
    }
    return value;
}

int SyntaxNode::IntAttribute(const char* name, int defaultValue) const
{
    auto it = _attributes.find(name);
    if (it != _attributes.end())
    {
        int value;
        if (StringUtil::ToInt(it->second.c_str(), &value))
        {
            return value;
        }
    }
    return defaultValue;
}

bool SyntaxNode::BoolAttribute(const char* name, bool defaultValue) const
{
    auto it = _attributes.find(name);
    if (it != _attributes.end())
    {
        bool value;
        if (StringUtil::ToBool(it->second.c_str(), &value))
        {
            return value;
        }
    }
    return defaultValue;
}

SyntaxNodePtr SyntaxNode::SetAttribute(const char* name, const char* value)
{
    std::map<std::string, std::string>::iterator it;
    if (_FindOrCreateAttribute(name, &it))
    {
        it->second = value;
    }
    return this;
}

SyntaxNodePtr SyntaxNode::SetIntAttribute(const char* name, int value)
{
    return SetAttribute(name, StringUtil::ToString(value));
}

SyntaxNodePtr SyntaxNode::SetBoolAttribute(const char* name, bool value)
{
    return SetAttribute(name, StringUtil::ToString(value));
}

SyntaxNodePtr SyntaxNode::RemoveAttribute(const char* name)
{
    std::map<std::string, std::string>::iterator it;
    if (_FindAttribute(name, &it))
    {
        _attributes.erase(it);
    }
    return this;
}

bool SyntaxNode::_FindAttribute(const char* name, std::map<std::string, std::string>::iterator* attr)
{
    auto it = _attributes.find(name);
    if (it != _attributes.end())
    {
        if (attr)
        {
            *attr = it;
        }
        return true;
    }

    return false;
}

bool SyntaxNode::_FindOrCreateAttribute(const char* name, std::map<std::string, std::string>::iterator* attr)
{
    std::map<std::string, std::string>::iterator it;
    if (_FindAttribute(name, &it))
    {
        if (attr)
        {
            *attr = it;
        }
        return true;
    }
    auto ret = _attributes.emplace(name, "");
    if (ret.second)
    {
        if (attr)
        {
            *attr = ret.first;
        }
        return true;
    }

    return false;
}


/*
 * Non-terminal syntax node.
 */
NonTerminalSyntaxNode::NonTerminalSyntaxNode(tomic::SyntaxType type)
        : SyntaxNode(SyntaxNodeType::NON_TERMINAL, type)
{
}

bool NonTerminalSyntaxNode::Accept(AstVisitorPtr visitor)
{
    TOMIC_ASSERT(visitor);

    if (visitor->VisitEnter(this))
    {
        for (auto node = FirstChild(); node; node = node->NextSibling())
        {
            if (!node->Accept(visitor))
            {
                break;
            }
        }
    }

    return visitor->VisitExit(this);
}

/*
 * Terminal syntax node.
 */
TerminalSyntaxNode::TerminalSyntaxNode(tomic::TokenPtr token)
        : SyntaxNode(SyntaxNodeType::TERMINAL, SyntaxType::ST_TERMINATOR, token)
{
}

bool TerminalSyntaxNode::Accept(AstVisitorPtr visitor)
{
    TOMIC_ASSERT(visitor);

    return visitor->Visit(this);
}

/*
 * Epsilon syntax node.
 */
EpsilonSyntaxNode::EpsilonSyntaxNode()
        : SyntaxNode(SyntaxNodeType::EPSILON, SyntaxType::ST_EPSILON)
{
}

bool EpsilonSyntaxNode::Accept(AstVisitorPtr visitor)
{
    TOMIC_ASSERT(visitor);

    return true;
}

TOMIC_END
