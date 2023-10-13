/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/ast/SyntaxNode.h>
#include <tomic/parser/ast/printer/JsonAstPrinter.h>

TOMIC_BEGIN

JsonAstPrinter::JsonAstPrinter(tomic::ISyntaxMapperPtr syntaxMapperPtr, tomic::ITokenMapperPtr tokenMapper)
        : _syntaxMapper(syntaxMapperPtr), _tokenMapper(tokenMapper), _depth(0), _indent(2)
{
    TOMIC_ASSERT(_syntaxMapper);
    TOMIC_ASSERT(_tokenMapper);
}

void JsonAstPrinter::Print(SyntaxTreePtr tree, twio::IWriterPtr writer)
{
    TOMIC_ASSERT(tree);
    TOMIC_ASSERT(writer);

    _writer = writer;

    // In Json, one level has two indents.
    _depth = -2;

    tree->Accept(this);
}

bool JsonAstPrinter::VisitEnter(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    auto descr = _syntaxMapper->Description(node->Type());

    _depth += 2;

    _PrintOpening(_depth, node);

    // name
    _PrintIndent(_depth + 1);
    if (descr)
    {
        _writer->WriteFormat("\"name\": \"%s\",\n", descr);
    }
    else
    {
        _writer->WriteFormat("\"name\": \"MISSING-%d\"\n", _depth);
    }

    // children
    _PrintIndent(_depth + 1);
    if (node->HasChildren())
    {
        _writer->Write("\"children\": [\n");
    }
    else
    {
        _writer->Write("\"children\": []\n");
    }

    return true;
}

bool JsonAstPrinter::VisitExit(tomic::SyntaxNodePtr node)
{
    if (node->HasChildren())
    {
        _PrintIndent(_depth + 1);
        _writer->Write("]\n");
    }

    _PrintClosing(_depth, node);

    _depth -= 2;

    return true;
}

bool JsonAstPrinter::Visit(tomic::SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    _depth += 2;

    if (node->IsTerminal())
    {
        _VisitTerminal(node);
    }
    else if (node->IsEpsilon())
    {
        _VisitEpsilon(node);
    }
    else
    {
        _VisitNonTerminal(node);
    }

    _depth -= 2;

    return true;
}

void JsonAstPrinter::_VisitNonTerminal(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);
    TOMIC_ASSERT(node->IsNonTerminal());

    auto descr = _syntaxMapper->Description(node->Type());

    // Opening.
    _PrintOpening(_depth, node);

    // name
    _PrintIndent(_depth + 1);
    if (descr)
    {
        _writer->WriteFormat("\"name\": \"%s\"\n", descr);
    }
    else
    {
        _writer->WriteFormat("\"name\": \"MISSING-%d\"\n", _depth);
    }

    // Closing.
    _PrintClosing(_depth, node);
}

void JsonAstPrinter::_VisitTerminal(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);
    TOMIC_ASSERT(node->IsTerminal());

    // Opening
    _PrintOpening(_depth, node);

    // name
    auto syntacticDescr = _syntaxMapper->Description(node->Type());
    _PrintIndent(_depth + 1);
    if (syntacticDescr)
    {
        _writer->WriteFormat("\"name\": \"%s\",\n", syntacticDescr);
    }
    else
    {
        _writer->WriteFormat("\"name\": \"MISSING-%d\",\n", _depth);
    }

    // token
    auto token = node->Token();
    auto tokenDescr = _tokenMapper->Description(token->type);
    _PrintIndent(_depth + 1);
    if (tokenDescr)
    {
        _writer->WriteFormat("\"token\": \"%s\",\n", tokenDescr);
    }
    else
    {
        _writer->WriteFormat("\"token\": \"MISSING-%d\",\n", _depth);
    }

    // lexeme
    _PrintIndent(_depth + 1);
    const char* lexeme = token->lexeme.c_str();
    _writer->Write("\"lexeme\": \"");
    for (const char* p = lexeme; *p; p++)
    {
        if (*p == '\"')
        {
            _writer->Write("\\\"");
        }
        else
        {
            _writer->WriteFormat("%c", *p);
        }
    }
    _writer->Write("\"\n");

    // Closing
    _PrintClosing(_depth, node);
}

void JsonAstPrinter::_VisitEpsilon(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);
    TOMIC_ASSERT(node->IsEpsilon());

    // Opening
    _PrintOpening(_depth, node);

    // name
    auto descr = _syntaxMapper->Description(node->Type());
    if (descr)
    {
        _writer->WriteFormat("\"name\": \"%s\"\n", descr);
    }
    else
    {
        _writer->WriteFormat("\"name\": \"MISSING-%d\"\n", _depth);
    }

    // Closing
    _PrintClosing(_depth, node);
}

void JsonAstPrinter::_PrintIndent(int depth)
{
    for (int i = 0; i < depth * _indent; i++)
    {
        _writer->Write(" ");
    }
}

void JsonAstPrinter::_PrintOpening(int depth, SyntaxNodePtr node)
{
    _PrintIndent(depth);
    _writer->Write("{\n");
}

void JsonAstPrinter::_PrintClosing(int depth, SyntaxNodePtr node)
{
    _PrintIndent(depth);
    _writer->Write("}");
    auto parent = node->Parent();
    if (parent && node != parent->LastChild())
    {
        _writer->Write(",");
    }
    _writer->Write("\n");
}

TOMIC_END
