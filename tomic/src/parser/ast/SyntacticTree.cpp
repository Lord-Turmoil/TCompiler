/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/Common.h"
#include "../../../include/tomic/parser/ast/SyntacticTree.h"
#include "../../../include/tomic/parser/ast/SyntacticNode.h"
#include <algorithm>

TOMIC_BEGIN

SyntacticTree::~SyntacticTree()
{
    for (auto node: _nodes)
    {
        delete node;
    }
}

std::shared_ptr<SyntacticTree> SyntacticTree::New()
{
    return std::make_shared<SyntacticTree>();
}

SyntacticNodePtr SyntacticTree::SetRoot(SyntacticNodePtr root)
{
    TOMIC_ASSERT(root);
    TOMIC_ASSERT(root->_tree == nullptr);

    root->_tree = this;
    _root = root;

    return root;
}

TOMIC_END
