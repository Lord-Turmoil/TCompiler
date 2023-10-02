/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/parser/ast/SyntacticNode.h"
#include "../../../include/tomic/parser/ast/SyntacticTree.h"

TOMIC_BEGIN

SyntacticNode::SyntacticNode(SyntacticType type)
        : _type(type), _tree(nullptr), _parent(nullptr), _token(nullptr)
{
}

SyntacticNode::SyntacticNode(SyntacticType type, TokenPtr token)
        : _type(type), _tree(nullptr), _parent(nullptr), _token(token)
{
}

SyntacticNodePtr SyntacticNode::Root() const
{
    auto node = const_cast<SyntacticNode*>(this);
    while (node->_parent)
    {
        node = node->_parent;
    }
    return node;
}

SyntacticNodePtr SyntacticNode::InsertEndChild(SyntacticNodePtr child)
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


SyntacticNodePtr SyntacticNode::InsertFirstChild(SyntacticNodePtr child)
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

void SyntacticNode::_InsertChildPreamble(SyntacticNodePtr child)
{
    TOMIC_ASSERT(child);
    TOMIC_ASSERT(child->_tree == _tree);

    if (child->_parent)
    {
        child->_parent->_Unlink(child);
    }
}

void SyntacticNode::_Unlink(SyntacticNodePtr child)
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

    child->_prev = nullptr;
    child->_next = nullptr;
    child->_parent = nullptr;
}

TOMIC_END
