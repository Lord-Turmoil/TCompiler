/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_XML_AST_PRINTER_H_
#define _TOMIC_XML_AST_PRINTER_H_

#include "../../../Common.h"
#include "IAstPrinter.h"
#include "../AstForward.h"

TOMIC_BEGIN

class XmlAstPrinter : public IAstPrinter
{
public:
    XmlAstPrinter();
    ~XmlAstPrinter() override = default;

    void Print(SyntaxTreePtr tree, twio::IWriterPtr writer) override;

    bool VisitEnter(SyntaxNodePtr node) override;
    bool VisitExit(SyntaxNodePtr node) override;
    bool Visit(SyntaxNodePtr node) override;

private:
    void _VisitNonTerminal(SyntaxNodePtr node);
    void _VisitTerminal(SyntaxNodePtr node);
    void _VisitEpsilon(SyntaxNodePtr node);

    twio::IWriterPtr _writer;
    int _depth;
    int _indent;
};


TOMIC_END

#endif // _TOMIC_XML_AST_PRINTER_H_