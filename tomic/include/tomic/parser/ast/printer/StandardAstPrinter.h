/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_STANDARD_AST_PRINTER_H_
#define _TOMIC_STANDARD_AST_PRINTER_H_

#include <tomic/lexer/token/ITokenMapper.h>
#include <tomic/parser/ast/AstVisitor.h>
#include <tomic/parser/ast/mapper/ISyntaxMapper.h>
#include <tomic/parser/ast/printer/IAstPrinter.h>
#include <tomic/Shared.h>

TOMIC_BEGIN

/*
 * This one is for the barnacle online judge. It shows how powerful
 * Dependency Injection is. Just so easy for the pluggable design.
 */
class StandardAstPrinter : public IAstPrinter, private AstVisitor
{
public:
    StandardAstPrinter(ISyntaxMapperPtr syntaxMapperPtr, ITokenMapperPtr tokenMapper);
    ~StandardAstPrinter() override = default;

    void Print(SyntaxTreePtr tree, twio::IWriterPtr writer) override;

private:
    bool VisitEnter(SyntaxNodePtr node) override;
    bool VisitExit(SyntaxNodePtr node) override;
    bool Visit(SyntaxNodePtr node) override;

    void _VisitNonTerminal(SyntaxNodePtr node);
    void _VisitTerminal(SyntaxNodePtr node);
    void _VisitEpsilon(SyntaxNodePtr node);

    twio::IWriterPtr _writer;
    ISyntaxMapperPtr _syntaxMapper;
    ITokenMapperPtr _tokenMapper;
};


TOMIC_END

#endif // _TOMIC_STANDARD_AST_PRINTER_H_
