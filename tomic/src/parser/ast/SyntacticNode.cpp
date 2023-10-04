/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/parser/ast/SyntaxNode.h"
#include "../../../include/tomic/parser/ast/SyntaxTree.h"
#include "../../../include/tomic/parser/ast/ASTVisitor.h"

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
 * Non-terminal syntax node.
 */
NonTerminalSyntaxNode::NonTerminalSyntaxNode(tomic::SyntaxType type)
        : SyntaxNode(SyntaxNodeType::NON_TERMINAL, type)
{
}

bool NonTerminalSyntaxNode::Accept(ASTVisitorPtr visitor)
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

bool TerminalSyntaxNode::Accept(ASTVisitorPtr visitor)
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

bool EpsilonSyntaxNode::Accept(ASTVisitorPtr visitor)
{
    TOMIC_ASSERT(visitor);

    return true;
}

TOMIC_END
