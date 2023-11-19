/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LEFT_RECURSIVE_AST_TRANSFORMER_H_
#define _TOMIC_LEFT_RECURSIVE_AST_TRANSFORMER_H_

#include <tomic/parser/ast/SyntaxType.h>
#include <tomic/parser/ast/trans/IAstTransformer.h>
#include <tomic/Shared.h>

#include <vector>

TOMIC_BEGIN

/*
 * This transformer is used to transform right recursive ast to left recursive ast.
 */
class RightRecursiveAstTransformer : public IAstTransformer
{
public:
    RightRecursiveAstTransformer() = default;
    ~RightRecursiveAstTransformer() override = default;

    SyntaxTreePtr Transform(SyntaxTreePtr tree) override;

    bool VisitEnter(SyntaxNodePtr node) override;
    bool VisitExit(SyntaxNodePtr node) override;
    bool Visit(SyntaxNodePtr node) override;

private:
    bool _IsRightRecursive(SyntaxNodePtr node);
    bool _NeedTransform(SyntaxNodePtr node);
    void _Transform(SyntaxNodePtr node);

    static std::vector<SyntaxType> _rightRecursiveTypes;
};


TOMIC_END

#endif // _TOMIC_LEFT_RECURSIVE_AST_TRANSFORMER_H_
