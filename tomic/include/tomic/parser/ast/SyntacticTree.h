/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TMOIC_SYNTACTIC_TREE_H_
#define _TMOIC_SYNTACTIC_TREE_H_

#include "../../Common.h"
#include "../../lexer/token/Token.h"
#include "SyntacticType.h"
#include <memory>
#include <vector>

TOMIC_BEGIN

class SyntacticNode;
using SyntacticNodePtr = SyntacticNode*;

/*
 * Syntactic Tree will manage the memory allocation of all the syntactic nodes.
 * So the syntactic nodes should not be created by the user directly, that is, the
 * constructors and destructors of the syntactic nodes are private. Therefore,
 * smart pointer are not used here.
 */
class SyntacticTree
{
public:
    SyntacticTree() = default;
    ~SyntacticTree();

    // Prohibit copying and cloning.
    SyntacticTree(const SyntacticTree&) = delete;
    SyntacticTree& operator=(const SyntacticTree&) = delete;
    SyntacticTree(SyntacticTree&&) = delete;
    SyntacticTree& operator=(SyntacticTree&&) = delete;

    // Create a new syntactic tree.
    static std::shared_ptr<SyntacticTree> New();

    // Create a new syntactic node, which is self-managed by the syntactic tree.
    template<typename TNode>
    SyntacticNodePtr NewNode(SyntacticType type);
    template<typename TNode>
    SyntacticNodePtr NewNode(SyntacticType type, TokenPtr token);

    SyntacticNodePtr Root() const { return _root; }

    SyntacticNodePtr SetRoot(SyntacticNodePtr root);

private:
    SyntacticNodePtr _root;
    std::vector<SyntacticNodePtr> _nodes;
};

using SyntacticTreePtr = std::shared_ptr<SyntacticTree>;


template<typename TNode>
SyntacticNodePtr SyntacticTree::NewNode(SyntacticType type)
{
    auto node = new TNode(type);
    _nodes.push_back(node);
    node->_tree = this;
    return node;
}

template<typename TNode>
SyntacticNodePtr SyntacticTree::NewNode(SyntacticType type, TokenPtr token)
{
    auto node = new TNode(type, token);
    _nodes.push_back(node);
    node->_tree = this;
    return node;
}

TOMIC_END

#endif // _TMOIC_SYNTACTIC_TREE_H_
