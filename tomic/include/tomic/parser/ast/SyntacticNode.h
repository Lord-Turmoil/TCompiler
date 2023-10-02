/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Dedicated to TinyXML, which is a great library.
 */

#ifndef _TOMIC_SYNTACTIC_NODE_H_
#define _TOMIC_SYNTACTIC_NODE_H_

#include "../../../../include/tomic/Common.h"
#include "../../lexer/token/Token.h"
#include "SyntacticType.h"
#include <vector>

TOMIC_BEGIN

class SyntacticNode;
using SyntacticNodePtr = SyntacticNode*;

class SyntacticTree;
using SyntacticTreePtr = std::shared_ptr<SyntacticTree>;

class ASTVisitor;
using ASTVisitorPtr = ASTVisitor*;

class SyntacticNode
{
    friend class SyntacticTree;
public:
    // Insert a child node to the end of the children list.
    // Return the added child.
    SyntacticNodePtr InsertEndChild(SyntacticNodePtr child);
    SyntacticNodePtr InsertFirstChild(SyntacticNodePtr child);


    // Get the root of the tree, can be a sub-tree.
    SyntacticNodePtr Root() const;

    // For visitor pattern.
    virtual bool Accept(ASTVisitorPtr visitor) = 0;

private:
    void _InsertChildPreamble(SyntacticNodePtr child);
    void _Unlink(SyntacticNodePtr child);

protected:
    SyntacticNode(SyntacticType type);
    SyntacticNode(SyntacticType type, TokenPtr token);
    virtual ~SyntacticNode() = default;

    // AST links.
    SyntacticTree* _tree;
    SyntacticNodePtr _parent;

    SyntacticNodePtr _firstChild;
    SyntacticNodePtr _lastChild;

    SyntacticNodePtr _prev;
    SyntacticNodePtr _next;

    // AST properties.
    SyntacticType _type;
    TokenPtr _token;
};

TOMIC_END

#endif // _TOMIC_SYNTACTIC_NODE_H_
