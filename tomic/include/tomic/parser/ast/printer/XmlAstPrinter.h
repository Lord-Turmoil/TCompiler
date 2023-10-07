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
#include "../SyntaxType.h"
#include "../mapper/ISyntaxMapper.h"
#include "../../../lexer/token/ITokenMapper.h"

TOMIC_BEGIN

class XmlAstPrinter : public IAstPrinter
{
public:
    XmlAstPrinter(ISyntaxMapperPtr syntaxMapperPtr, ITokenMapperPtr tokenMapper);
    ~XmlAstPrinter() override = default;

    void Print(SyntaxTreePtr tree, twio::IWriterPtr writer) override;

    bool VisitEnter(SyntaxNodePtr node) override;
    bool VisitExit(SyntaxNodePtr node) override;
    bool Visit(SyntaxNodePtr node) override;

private:
    void _VisitNonTerminal(SyntaxNodePtr node);
    void _VisitTerminal(SyntaxNodePtr node);
    void _VisitEpsilon(SyntaxNodePtr node);

    void _PrintIndent(int depth);

private:
    twio::IWriterPtr _writer;
    ISyntaxMapperPtr _syntaxMapper;
    ITokenMapperPtr _tokenMapper;
    int _depth;
    int _indent;
};


TOMIC_END

#endif // _TOMIC_XML_AST_PRINTER_H_