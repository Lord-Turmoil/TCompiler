/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../../include/tomic/parser/ast/SyntaxTree.h"
#include "../../../../include/tomic/parser/ast/SyntaxNode.h"
#include "../../../../include/tomic/parser/ast/printer/XmlAstPrinter.h"

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

    _depth++;

    _PrintIndent(_depth);
    if (descr)
    {
        _writer->WriteFormat("<%s>\n", descr);
    }
    else
    {
        _writer->WriteFormat("<MISSING-%d>\n", _depth);
    }

    return true;
}

bool XmlAstPrinter::VisitExit(tomic::SyntaxNodePtr node)
{
    auto descr = _syntaxMapper->Description(node->Type());

    _PrintIndent(_depth);
    if (descr)
    {
        _writer->WriteFormat("</%s>\n", descr);
    }
    else
    {
        _writer->WriteFormat("</MISSING-%d>\n", _depth);
    }

    _depth--;

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
        TOMIC_ASSERT(false && "What the?");
    }

    _depth--;

    return true;
}

void XmlAstPrinter::_VisitNonTerminal(tomic::SyntaxNodePtr node)
{
    auto descr = _syntaxMapper->Description(node->Type());

    _PrintIndent(_depth);
    if (descr)
    {
        _writer->WriteFormat("<%s />\n", descr);
    }
    else
    {
        _writer->WriteFormat("<MISSING-%d />\n", _depth);
    }

    // Visit will not recurse into children.
}

void XmlAstPrinter::_VisitTerminal(tomic::SyntaxNodePtr node)
{
    auto syntacticDescr = _syntaxMapper->Description(node->Type());

    _PrintIndent(_depth);
    if (syntacticDescr)
    {
        _writer->WriteFormat("<%s", syntacticDescr);
    }
    else
    {
        _writer->WriteFormat("<MISSING-%d", _depth);
    }

    const char* tokenDescr = _tokenMapper->Description(node->Token()->type);
    if (tokenDescr)
    {
        _writer->WriteFormat(" token=\'%s\'", tokenDescr);
    }
    else
    {
        _writer->WriteFormat(" token=\'MISSING-%d\'", _depth);
    }

    const char* lexeme = node->Token()->lexeme.c_str();
    _writer->Write(" lexeme=\'");
    for (const char* p = lexeme; *p; p++)
    {
        if (*p == '&')
        {
            _writer->Write("&amp;");
        }
        else
        {
            _writer->WriteFormat("%c", *p);
        }
    }
    _writer->Write("\' />\n");
}

void XmlAstPrinter::_VisitEpsilon(tomic::SyntaxNodePtr node)
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto mapper = container->Resolve<ISyntaxMapper>();
    auto descr = mapper->Description(node->Type());

    _PrintIndent(_depth);
    if (descr)
    {
        _writer->WriteFormat("<%s>\n", descr);
    }
    else
    {
        _writer->WriteFormat("<EPSILON: %d>\n", _depth);
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
