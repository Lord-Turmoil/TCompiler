/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

// For forward declarations.

#ifndef _TOMIC_AST_H_
#define _TOMIC_AST_H_

#include <tomic/Common.h>

TOMIC_BEGIN

class SyntaxNode;
using SyntaxNodePtr = SyntaxNode*;

class SyntaxTree;
using SyntaxTreePtr = std::shared_ptr<SyntaxTree>;

class AstVisitor;
using AstVisitorPtr = AstVisitor*;

TOMIC_END

#endif // _TOMIC_AST_H_
