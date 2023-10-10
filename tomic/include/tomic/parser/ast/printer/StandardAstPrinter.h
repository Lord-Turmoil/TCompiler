/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_STANDARD_AST_PRINTER_H_
#define _TOMIC_STANDARD_AST_PRINTER_H_

#include "../../../Common.h"
#include "IAstPrinter.h"
#include "../SyntaxType.h"
#include "../AstVisitor.h"
#include "../mapper/ISyntaxMapper.h"
#include "../../../lexer/token/ITokenMapper.h"

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
