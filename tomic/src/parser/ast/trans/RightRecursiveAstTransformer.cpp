/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/ast/trans/RightRecursiveAstTransformer.h>
#include <tomic/parser/ast/SyntaxNode.h>
#include <tomic/parser/ast/SyntaxTree.h>
#include <vector>

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
    return true;
}

/*
 * We MUST NOT transform the current node, since it is still being visited.
 * If transformed, the parent node will then get the wrong NextSibling from
 * this node!
 * Therefore, we transform all its children, as we've done visiting them. And
 * one exception, if the current node is the root of the tree, we transform it
 * when we exit it.
 */
bool RightRecursiveAstTransformer::VisitExit(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    // First, we should make a copy of the children list, since we will modify
    // it during the transformation, which will make NextSibling wrong.
    std::vector<SyntaxNodePtr> children;
    for (auto child = node->FirstChild(); child; child = child->NextSibling())
    {
        children.push_back(child);
    }

    // Then we transform all its children.
    for (auto child : children)
    {
        if (_NeedTransform(child))
        {
            _Transform(child);
        }
    }

    // At last, if it is the root, transform it.
    if (!node->Parent() && _NeedTransform(node))
    {
        _Transform(node);
    }

    return true;
}

bool RightRecursiveAstTransformer::Visit(SyntaxNodePtr node)
{
    return true;
}

bool RightRecursiveAstTransformer::_IsRightRecursive(SyntaxNodePtr node)
{
    for (auto type : _rightRecursiveTypes)
    {
        if (node->Type() == type)
        {
            return true;
        }
    }

    return false;
}

bool RightRecursiveAstTransformer::_NeedTransform(SyntaxNodePtr node)
{
    if (!_IsRightRecursive(node))
    {
        return false;
    }

    auto lastChild = node->LastChild();

    return lastChild && (lastChild->Type() == node->Type());
}

void RightRecursiveAstTransformer::_Transform(SyntaxNodePtr node)
{
    if (!_NeedTransform(node))
    {
        return;
    }

    // parent is the parent node for the whole right recursive sub-tree,
    // so it remains unchanged during the transformation.
    // It can be nullptr if the node is the root of the tree, but in most cases,
    // it shouldn't.
    auto parent = node->Parent();
    SyntaxNodePtr mark = nullptr;

    // First, we remove the current node from its parent.
    // We need to remember the location, so we can insert its last child to the
    // same location.
    if (parent)
    {
        mark = parent->RemoveChild(node);
    }

    // Then, we remove the last child of the current node.
    auto lastChild = node->LastChild();
    node->RemoveChild(lastChild);

    // We find the left most child with the same type of the last child
    // to which we will insert the current node.
    auto candidate = lastChild;
    while (candidate->HasChildren() && (candidate->FirstChild()->Type() == lastChild->Type()))
    {
        candidate = candidate->FirstChild();
    }

    // Now, we insert the current node as the first child of the candidate.
    candidate->InsertFirstChild(node);

    // Finally, we insert the last child to the same location of the current node.
    if (parent)
    {
        parent->InsertAfterChild(lastChild, mark);
    }
}


TOMIC_END
