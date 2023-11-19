/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_AST_VISITOR_H_
#define _TOMIC_AST_VISITOR_H_

#include <tomic/parser/ast/AstForward.h>
#include <tomic/Shared.h>

TOMIC_BEGIN

class AstVisitor
{
public:
    virtual ~AstVisitor() = default;

    // Called when enter a syntax node.
    virtual bool VisitEnter(SyntaxNodePtr node) { return true; }

    // Called when exit a syntax node.
    virtual bool VisitExit(SyntaxNodePtr node) { return true; }

    // Called when visit a syntax node, usually a terminal one.
    virtual bool Visit(SyntaxNodePtr node) { return true; }
};


TOMIC_END

#endif // _TOMIC_AST_VISITOR_H_
