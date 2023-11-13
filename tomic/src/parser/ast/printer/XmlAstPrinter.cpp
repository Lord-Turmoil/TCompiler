/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/ast/SyntaxNode.h>
#include <tomic/parser/ast/printer/XmlAstPrinter.h>

TOMIC_BEGIN

XmlAstPrinter::XmlAstPrinter(tomic::ISyntaxMapperPtr syntaxMapperPtr, tomic::ITokenMapperPtr tokenMapper)
        : _syntaxMapper(syntaxMapperPtr), _tokenMapper(tokenMapper), _depth(0), _indent(2)
{
    TOMIC_ASSERT(_syntaxMapper);
    TOMIC_ASSERT(_tokenMapper);
}

void XmlAstPrinter::Print(SyntaxTreePtr tree, twio::IWriterPtr writer)
{
    TOMIC_ASSERT(tree);
    TOMIC_ASSERT(writer);

    _writer = writer;

    // To make first element with depth 0, we set depth to -1.
    _depth = -1;

    tree->Accept(this);
}

bool XmlAstPrinter::VisitEnter(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    auto descr = _syntaxMapper->Description(node->Type());

    if (descr)
    {
        _depth++;
        _PrintIndent(_depth);
        _writer->WriteFormat("<%s", descr);
        for (auto attr : node->Attributes())
        {
            _writer->WriteFormat(" %s=\'%s\'", attr.first.c_str(), attr.second.c_str());
        }
        _writer->Write(">\n");
    }

    return true;
}

bool XmlAstPrinter::VisitExit(tomic::SyntaxNodePtr node)
{
    auto descr = _syntaxMapper->Description(node->Type());

    if (descr)
    {
        _PrintIndent(_depth);
        _writer->WriteFormat("</%s>\n", descr);
        _depth--;
    }

    return true;
}

bool XmlAstPrinter::Visit(tomic::SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    _depth++;

    if (node->IsNonTerminal())
    {
        _VisitNonTerminal(node);
    }
    else if (node->IsTerminal())
    {
        _VisitTerminal(node);
    }
    else if (node->IsEpsilon())
    {
        _VisitEpsilon(node);
    }
    else
    {
        TOMIC_PANIC("What the?");
    }

    _depth--;

    return true;
}

void XmlAstPrinter::_VisitNonTerminal(tomic::SyntaxNodePtr node)
{
    auto descr = _syntaxMapper->Description(node->Type());

    if (descr)
    {
        _PrintIndent(_depth);
        _writer->WriteFormat("<%s", descr);
        for (auto attr : node->Attributes())
        {
            _writer->WriteFormat(" %s=\'%s\'", attr.first.c_str(), attr.second.c_str());
        }
        _writer->Write(" />\n");
    }

    // Visit will not recurse into children.
}

void XmlAstPrinter::_VisitTerminal(tomic::SyntaxNodePtr node)
{
    auto syntacticDescr = _syntaxMapper->Description(node->Type());

    if (!syntacticDescr)
    {
        return;
    }

    _PrintIndent(_depth);
    _writer->WriteFormat("<%s", syntacticDescr);

    const char* tokenDescr = _tokenMapper->Description(node->Token()->type);
    _writer->WriteFormat(" token=\'%s\'", tokenDescr ? tokenDescr : "\'\'");
    const char* lexeme = node->Token()->lexeme.c_str();
    _writer->Write(" lexeme=\'");
    for (const char* p = lexeme; *p; p++)
    {
        if (*p == '&')
        {
            _writer->Write("&amp;");
        }
        else if (*p == '<')
        {
            _writer->Write("&lt;");
        }
        else if (*p == '>')
        {
            _writer->Write("&gt;");
        }
        else
        {
            _writer->WriteFormat("%c", *p);
        }
    }
    _writer->Write("\'");

    _writer->WriteFormat(" line=\'%d\'", node->Token()->lineNo);
    _writer->WriteFormat(" char=\'%d\'", node->Token()->charNo);

    for (auto attr : node->Attributes())
    {
        _writer->WriteFormat(" %s=\'%s\'", attr.first.c_str(), attr.second.c_str());
    }

    _writer->Write(" />\n");
}

void XmlAstPrinter::_VisitEpsilon(tomic::SyntaxNodePtr node)
{
    auto descr = _syntaxMapper->Description(node->Type());
    if (descr)
    {
        _PrintIndent(_depth);
        _writer->WriteFormat("<%s>\n", descr);
    }
}

void XmlAstPrinter::_PrintIndent(int depth)
{
    for (int i = 0; i < depth * _indent; i++)
    {
        _writer->Write(" ");
    }
}

TOMIC_END
