/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/Common.h"
#include "../../../include/tomic/parser/ast/SyntaxTree.h"
#include "../../../include/tomic/parser/ast/SyntaxNode.h"
#include <algorithm>

TOMIC_BEGIN

SyntaxTree::~SyntaxTree()
{
    _ClearUp();
}

std::shared_ptr<SyntaxTree> SyntaxTree::New()
{
    return std::make_shared<SyntaxTree>();
}

SyntaxNodePtr SyntaxTree::NewTerminalNode(TokenPtr token)
{
    auto node = new TerminalSyntaxNode(token);
    node->_tree = this;
    _nodes.insert(node);

    return node;
}

SyntaxNodePtr SyntaxTree::NewNonTerminalNode(SyntaxType type)
{
    auto node = new NonTerminalSyntaxNode(type);
    node->_tree = this;
    _nodes.insert(node);

    return node;
}

SyntaxNodePtr SyntaxTree::NewEpsilonNode()
{
    auto node = new EpsilonSyntaxNode();
    node->_tree = this;
    _nodes.insert(node);

    return node;
}

void SyntaxTree::DeleteNode(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);
    TOMIC_ASSERT(node->_tree == this);

    if (node == _root)
    {
        _ClearUp();
        return;
    }

    if (node->_parent)
    {
        node->_parent->_Unlink(node);
    }
    while (node->HasChildren())
    {
        DeleteNode(node->FirstChild());
    }

    auto it = _nodes.erase(node);
    delete node;
}

SyntaxNodePtr SyntaxTree::SetRoot(SyntaxNodePtr root)
{
    TOMIC_ASSERT(root);
    TOMIC_ASSERT(root->_tree == this);
    TOMIC_ASSERT(root->_parent == nullptr);

    root->_tree = this;
    _root = root;

    return root;
}

bool SyntaxTree::Accept(ASTVisitorPtr visitor)
{
    TOMIC_ASSERT(visitor);
    TOMIC_ASSERT(_root);

    return _root->Accept(visitor);
}

void SyntaxTree::_ClearUp()
{
    for (auto it = _nodes.begin(); it != _nodes.end(); it++)
    {
        delete *it;
    }
    _nodes.clear();
    _root = nullptr;
}

TOMIC_END
