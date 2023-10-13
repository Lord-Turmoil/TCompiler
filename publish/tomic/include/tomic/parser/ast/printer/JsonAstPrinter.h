/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_JSON_AST_PRINTER_H_
#define _TOMIC_JSON_AST_PRINTER_H_

#include <tomic/Common.h>
#include <tomic/parser/ast/printer/IAstPrinter.h>
#include <tomic/parser/ast/AstForward.h>
#include <tomic/parser/ast/AstVisitor.h>
#include <tomic/parser/ast/SyntaxType.h>
#include <tomic/parser/ast/mapper/ISyntaxMapper.h>
#include <tomic/lexer/token/ITokenMapper.h>

TOMIC_BEGIN

class JsonAstPrinter : public IAstPrinter, private AstVisitor
{
public:
    JsonAstPrinter(ISyntaxMapperPtr syntaxMapperPtr, ITokenMapperPtr tokenMapper);
    ~JsonAstPrinter() override = default;

    void Print(SyntaxTreePtr tree, twio::IWriterPtr writer) override;

private:
    bool VisitEnter(SyntaxNodePtr node) override;
    bool VisitExit(SyntaxNodePtr node) override;
    bool Visit(SyntaxNodePtr node) override;
    
    void _VisitNonTerminal(SyntaxNodePtr node);
    void _VisitTerminal(SyntaxNodePtr node);
    void _VisitEpsilon(SyntaxNodePtr node);

    void _PrintIndent(int depth);
    void _PrintOpening(int depth, SyntaxNodePtr node);
    void _PrintClosing(int depth, SyntaxNodePtr node);

private:
    twio::IWriterPtr _writer;
    ISyntaxMapperPtr _syntaxMapper;
    ITokenMapperPtr _tokenMapper;
    int _depth;
    int _indent;
};

TOMIC_END

#endif // _TOMIC_JSON_AST_PRINTER_H_
