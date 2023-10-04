/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../../include/tomic/parser/ast/trans/RightRecursiveAstTransformer.h"
#include "../../../../include/tomic/parser/ast/SyntaxNode.h"
#include "../../../../include/tomic/parser/ast/SyntaxTree.h"

TOMIC_BEGIN

std::vector<SyntaxType> RightRecursiveAstTransformer::_rightRecursiveTypes = {
        SyntaxType::ST_ADD_EXP,
        SyntaxType::ST_MUL_EXP,
        SyntaxType::ST_OR_EXP,
        SyntaxType::ST_AND_EXP,
        SyntaxType::ST_EQ_EXP,
        SyntaxType::ST_REL_EXP
};

SyntaxTreePtr RightRecursiveAstTransformer::Transform(SyntaxTreePtr tree)
{
    TOMIC_ASSERT(tree);

    tree->Accept(this);

    return tree;
}

bool RightRecursiveAstTransformer::VisitEnter(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    if (_IsRightRecursive(node))
    {
        _Transform(node);
    }

    return true;
}

bool RightRecursiveAstTransformer::VisitExit(SyntaxNodePtr node)
{
    return true;
}

bool RightRecursiveAstTransformer::Visit(SyntaxNodePtr node)
{
    return true;
}

bool RightRecursiveAstTransformer::_IsRightRecursive(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    for (auto type: _rightRecursiveTypes)
    {
        if (node->Type() == type)
        {
            return true;
        }
    }

    return false;
}

void RightRecursiveAstTransformer::_Transform(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    if (!_IsRightRecursive(node))
    {
        return;
    }

    // parent is the parent node for the whole right recursive sub-tree,
    // so it remains unchanged during the transformation.
    // It can be nullptr if the node is the root of the tree, but in most cases,
    // it shouldn't.
    auto parent = node->Parent();

    // We determine whether the transformation is needed by checking if
    // the last child is of the same recursive type of the current node.
    auto lastChild = node->LastChild();

    // Rotate the tree while the last child of the node is of the same recursive type.
    while (lastChild && (lastChild->Type() == node->Type()))
    {
        // Unlink this child.
        node->RemoveChild(lastChild);

        // Make this node the first child of the 'lastChild' node.
        // And add this 'lastChild' to the old place of the node.
        if (parent)
        {
            auto mark = parent->RemoveChild(node);
            lastChild->InsertFirstChild(node);
            parent->InsertAfterChild(lastChild, mark);
        }
        else
        {
            lastChild->InsertFirstChild(node);
        }

        node = lastChild;
        lastChild = node->LastChild();
    }
}


TOMIC_END
