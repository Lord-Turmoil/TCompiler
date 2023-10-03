/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../include/tomic/parser/impl/DefaultSyntacticParser.h"

TOMIC_BEGIN

DefaultSyntacticParser::DefaultSyntacticParser(ILexicalParserPtr lexicalParser)
        : _lexicalParser(lexicalParser)
{
}

DefaultSyntacticParser* DefaultSyntacticParser::SetReader(twio::IAdvancedReaderPtr reader)
{
    _lexicalParser->SetReader(reader);
    return this;
}

TokenPtr DefaultSyntacticParser::_Lookahead(int n)
{
    TOMIC_ASSERT(n > 0);

    int i;
    TokenPtr token;

    // Read n tokens.
    for (i = 0; i < n; i++)
    {
        token = _lexicalParser->Next();
        if (token == nullptr)
        {
            break;
        }
    }

    // Rewind to the original position.
    for (int j = 0; j < i; j++)
    {
        _lexicalParser->Rewind();
    }

    return token;
}

bool DefaultSyntacticParser::_Match(TokenType type, TokenPtr token)
{
    return Token::Type(token) == type;
}

void DefaultSyntacticParser::_PostParseError(int checkpoint, SyntaxNodePtr node)
{
    if (checkpoint >= 0)
    {
        _lexicalParser->Rollback(checkpoint);
    }
    if (node)
    {
        _tree->DeleteNode(node);
    }
}

SyntaxTreePtr DefaultSyntacticParser::Parse()
{
    _tree = SyntaxTree::New();

    return _tree;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseCompUnit()
{
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_COMP_UNIT);
    auto checkpoint = _lexicalParser->SetCheckPoint();

    // Parse Decl
    SyntaxNodePtr decl;
    while ((decl = _ParseDecl()) != nullptr)
    {
        root->InsertEndChild(decl);
    }

    // Parse FuncDef
    SyntaxNodePtr funcDef;
    while ((funcDef = _ParseFuncDef()) != nullptr)
    {
        root->InsertEndChild(funcDef);
    }

    // Parse MainFuncDef
    SyntaxNodePtr mainFuncDef = _ParseMainFuncDef();
    if (mainFuncDef != nullptr)
    {
        root->InsertEndChild(mainFuncDef);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseDecl()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_DECL);

    TokenPtr lookahead = _Lookahead(1);
    SyntaxNodePtr child = nullptr;

    if (_Match(TokenType::TK_CONST, lookahead))
    {
        child = _ParseConstDecl();
    }
    else
    {
        child = _ParseVarDecl();
    }

    // If failed, rollback.
    if (child == nullptr)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(child);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseBType()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_BTYPE);

    SyntaxNodePtr child;
    TokenPtr token = _lexicalParser->Next();
    if (_Match(TokenType::TK_INT, token))
    {
        child = _tree->NewTerminalNode(token);
    }
    else
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(child);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseConstDecl()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_CONST_DECL);

    // const
    TokenPtr token = _lexicalParser->Next();
    if (!_Match(TokenType::TK_CONST, token))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    SyntaxNodePtr constToken = _tree->NewTerminalNode(token);
    root->InsertEndChild(constToken);

    // BType
    SyntaxNodePtr type = _ParseBType();
    if (!type)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(type);

    // ConstDef
    SyntaxNodePtr constDef = _ParseConstDef();
    if (!constDef)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(constDef);

    TokenPtr lookahead = _Lookahead(1);
    while (_Match(TokenType::TK_COMMA, lookahead))
    {
        constDef = _ParseConstDef();
        if (!constDef)
        {
            break;
        }
        root->InsertEndChild(constDef);
    }

    // Check ;
    token = _lexicalParser->Next();
    if (!_Match(TokenType::TK_SEMICOLON, token))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    SyntaxNodePtr semicolon = _tree->NewTerminalNode(token);
    root->InsertEndChild(semicolon);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseConstDef()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_CONST_DEF);

    // Identifier
    TokenPtr token = _lexicalParser->Next();
    if (!_Match(TokenType::TK_IDENTIFIER, token))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    SyntaxNodePtr identifier = _tree->NewTerminalNode(token);
    root->InsertEndChild(identifier);

    // Dimension
    // TODO: Limit dimension.
    while (_Match(TokenType::TK_LEFT_BRACKET, _Lookahead(1)))
    {
        // Skip '['
        root->InsertEndChild(_tree->NewTerminalNode(_lexicalParser->Next()));

        SyntaxNodePtr constExp = _ParseConstExp();
        if (!constExp)
        {
            // TODO: Error
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(constExp);
        if (!_Match(TokenType::TK_RIGHT_BRACKET, _Lookahead(1)))
        {
            // TODO: Error
            _PostParseError(checkpoint, root);
            return nullptr;
        }

        // Skip ']'
        root->InsertEndChild(_tree->NewTerminalNode(_lexicalParser->Next()));
    }

    // '='
    token = _lexicalParser->Next();
    if (!_Match(TokenType::TK_ASSIGN, token))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(token));

    // const init val
    auto constInitVal = _ParseConstInitVal();
    if (!constInitVal)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(constInitVal);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseConstInitVal()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_CONST_INIT_VAL);

    if (_Match(TokenType::TK_LEFT_BRACE, _Lookahead(1)))
    {
        // Skip '{'
        root->InsertEndChild(_tree->NewTerminalNode(_lexicalParser->Next()));

        auto constInitVal = _ParseConstInitVal();
        if (!constInitVal)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        while (_Match(TokenType::TK_COMMA, _Lookahead(1)))
        {
            // Skip ','
            root->InsertEndChild(_tree->NewTerminalNode(_lexicalParser->Next()));

            constInitVal = _ParseConstInitVal();
            if (!constInitVal)
            {
                _PostParseError(checkpoint, root);
                return nullptr;
            }
        }

        // Check '}'
        if (!_Match(TokenType::TK_RIGHT_BRACE, _Lookahead(1)))
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        // Skip '}'
        root->InsertEndChild(_tree->NewTerminalNode(_lexicalParser->Next()));
    }
    else
    {
        auto constExt = _ParseConstExp();
        if (!constExt)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(constExt);
    }

    return root;
}

TOMIC_END
