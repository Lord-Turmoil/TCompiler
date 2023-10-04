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

    bool VisitEnter(SyntaxNodePtr node) override;
    bool VisitExit(SyntaxNodePtr node) override;
    bool Visit(SyntaxNodePtr node) override;

public:
    XmlAstPrinter* SetWriter(twio::IWriterPtr writer) override
    {
        _writer = writer;
        return this;
    }

    void Print(SyntaxTreePtr tree) override;

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