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

TokenPtr DefaultSyntacticParser::_Next()
{
    return _lexicalParser->Next();
}

TokenPtr DefaultSyntacticParser::_Lookahead(int n)
{
    TOMIC_ASSERT(n > 0);

    int i;
    TokenPtr token;

    // Read n tokens.
    for (i = 0; i < n; i++)
    {
        token = _Next();
        if (!token)
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

bool DefaultSyntacticParser::_MatchAny(const std::vector<TokenType>& types, TokenPtr token)
{
    for (auto type: types)
    {
        if (_Match(type, token))
        {
            return true;
        }
    }
    return false;
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
    while ((decl = _ParseDecl()))
    {
        root->InsertEndChild(decl);
    }

    // Parse FuncDef
    SyntaxNodePtr funcDef;
    while ((funcDef = _ParseFuncDef()))
    {
        root->InsertEndChild(funcDef);
    }

    // Parse MainFuncDef
    SyntaxNodePtr mainFuncDef = _ParseMainFuncDef();
    if (mainFuncDef)
    {
        root->InsertEndChild(mainFuncDef);
    }

    return root;
}

/*
 * ==================== Decl ====================
 */

SyntaxNodePtr DefaultSyntacticParser::_ParseDecl()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_DECL);

    TokenPtr lookahead = _Lookahead();
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
    if (!child)
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
    TokenPtr token = _Next();
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
    TokenPtr token = _Next();
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

    while (_Match(TokenType::TK_COMMA, _Lookahead()))
    {
        // Skip ','
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        constDef = _ParseConstDef();
        if (!constDef)
        {
            // TODO: Potential Error
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(constDef);
    }

    // Check ;
    token = _Next();
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
    if (!_Match(TokenType::TK_IDENTIFIER, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    SyntaxNodePtr identifier = _tree->NewTerminalNode(_Next());
    root->InsertEndChild(identifier);

    // Dimension
    // TODO: Limit dimension.
    while (_Match(TokenType::TK_LEFT_BRACKET, _Lookahead()))
    {
        // Skip '['
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        SyntaxNodePtr constExp = _ParseConstExp();
        if (!constExp)
        {
            // TODO: Error
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(constExp);
        if (!_Match(TokenType::TK_RIGHT_BRACKET, _Lookahead()))
        {
            // TODO: Error
            _PostParseError(checkpoint, root);
            return nullptr;
        }

        // Skip ']'
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
    }

    // '='
    if (!_Match(TokenType::TK_ASSIGN, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

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

    if (_Match(TokenType::TK_LEFT_BRACE, _Lookahead()))
    {
        // Skip '{'
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        auto constInitVal = _ParseConstInitVal();
        if (!constInitVal)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(constInitVal);
        while (_Match(TokenType::TK_COMMA, _Lookahead()))
        {
            // Skip ','
            root->InsertEndChild(_tree->NewTerminalNode(_Next()));

            constInitVal = _ParseConstInitVal();
            if (!constInitVal)
            {
                _PostParseError(checkpoint, root);
                return nullptr;
            }
            root->InsertEndChild(constInitVal);
        }

        // Check '}'
        if (!_Match(TokenType::TK_RIGHT_BRACE, _Lookahead()))
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        // Skip '}'
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
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

SyntaxNodePtr DefaultSyntacticParser::_ParseVarDecl()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_VAR_DECL);

    // BType
    SyntaxNodePtr type = _ParseBType();
    if (!type)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(type);

    // VarDef
    SyntaxNodePtr varDef = _ParseVarDef();
    if (!varDef)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(varDef);

    while (_Match(TokenType::TK_COMMA, _Lookahead()))
    {
        // Skip ','
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        varDef = _ParseVarDef();
        if (!varDef)
        {
            // TODO: Potential Error
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(varDef);
    }

    // Check ;
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    SyntaxNodePtr semicolon = _tree->NewTerminalNode(_Next());
    root->InsertEndChild(semicolon);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseVarDef()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_VAR_DEF);

    // Identifier
    if (!_Match(TokenType::TK_IDENTIFIER, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    SyntaxNodePtr identifier = _tree->NewTerminalNode(_Next());
    root->InsertEndChild(identifier);

    // Dimension
    // TODO: Limit dimension.
    while (_Match(TokenType::TK_LEFT_BRACKET, _Lookahead()))
    {
        // Skip '['
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        SyntaxNodePtr constExp = _ParseConstExp();
        if (!constExp)
        {
            // TODO: Error
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(constExp);
        if (!_Match(TokenType::TK_RIGHT_BRACKET, _Lookahead()))
        {
            // TODO: Error
            _PostParseError(checkpoint, root);
            return nullptr;
        }

        // Skip ']'
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
    }

    // '='
    if (!_Match(TokenType::TK_ASSIGN, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // init val
    auto initVal = _ParseInitVal();
    if (!initVal)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(initVal);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseInitVal()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_INIT_VAL);

    if (_Match(TokenType::TK_LEFT_BRACE, _Lookahead()))
    {
        // Skip '{'
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        auto initVal = _ParseInitVal();
        if (!initVal)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(initVal);
        while (_Match(TokenType::TK_COMMA, _Lookahead()))
        {
            // Skip ','
            root->InsertEndChild(_tree->NewTerminalNode(_Next()));

            initVal = _ParseInitVal();
            if (!initVal)
            {
                _PostParseError(checkpoint, root);
                return nullptr;
            }
            root->InsertEndChild(initVal);
        }

        // Check '}'
        if (!_Match(TokenType::TK_RIGHT_BRACE, _Lookahead()))
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        // Skip '}'
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
    }
    else
    {
        auto exp = _ParseExp();
        if (!exp)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(exp);
    }

    return root;
}

/*
 * ==================== FuncDef ====================
 */

SyntaxNodePtr DefaultSyntacticParser::_ParseFuncDef()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_FUNC_DEF);

    // FuncType
    SyntaxNodePtr funcType = _ParseFuncType();
    if (!funcType)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(funcType);

    // Identifier
    if (!_Match(TokenType::TK_IDENTIFIER, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    SyntaxNodePtr identifier = _tree->NewTerminalNode(_Next());
    root->InsertEndChild(identifier);

    // FuncFParams

    // '('
    if (!_Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    SyntaxNodePtr funcFParams = _ParseFuncFParams();
    if (!funcFParams)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(funcFParams);

    // ')'
    if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Block
    SyntaxNodePtr block = _ParseBlock();
    if (!block)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(block);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseFuncType()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_FUNC_TYPE);

    TokenPtr lookahead = _Lookahead();
    if (_Match(TokenType::TK_INT, lookahead))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
    }
    else if (_Match(TokenType::TK_VOID, lookahead))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
    }
    else
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseFuncFParams()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_FUNC_FPARAMS);

    auto funcFParam = _ParseFuncFParam();
    if (!funcFParam)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(funcFParam);
    while (_Match(TokenType::TK_COMMA, _Lookahead()))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        funcFParam = _ParseFuncFParam();
        if (!funcFParam)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(funcFParam);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseFuncFParam()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_FUNC_FPARAM);

    // BType
    SyntaxNodePtr type = _ParseBType();
    if (!type)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(type);

    // Identifier
    if (!_Match(TokenType::TK_IDENTIFIER, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // first dimension
    if (_Match(TokenType::TK_LEFT_BRACKET, _Lookahead()))
    {
        // Skip '['
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        if (_Match(TokenType::TK_RIGHT_BRACKET, _Lookahead()))
        {
            // Skip ']'
            root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        }
        else
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        // second dimension
        if (_Match(TokenType::TK_LEFT_BRACKET, _Lookahead()))
        {
            // Skip '['
            root->InsertEndChild(_tree->NewTerminalNode(_Next()));
            auto constExpr = _ParseConstExp();
            if (!constExpr)
            {
                _PostParseError(checkpoint, root);
                return nullptr;
            }

            // Check ']'
            if (_Match(TokenType::TK_RIGHT_BRACKET, _Lookahead()))
            {
                // Skip ']'
                root->InsertEndChild(_tree->NewTerminalNode(_Next()));
            }
            else
            {
                _PostParseError(checkpoint, root);
                return nullptr;
            }
        }
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseFuncAParams()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_FUNC_APARAMS);

    auto exp = _ParseExp();
    if (!exp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(exp);
    while (_Match(TokenType::TK_COMMA, _Lookahead()))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        exp = _ParseExp();
        if (!exp)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(exp);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseBlock()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_BLOCK);

    // '{'
    if (!_Match(TokenType::TK_LEFT_BRACE, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // If the block is empty, return directly.
    if (_Match(TokenType::TK_RIGHT_BRACE, _Lookahead()))
    {
        // '}'
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        return root;
    }

    // BlockItem
    SyntaxNodePtr blockItem;
    while ((blockItem = _ParseBlockItem()))
    {
        root->InsertEndChild(blockItem);
    }

    // '}'
    if (!_Match(TokenType::TK_RIGHT_BRACE, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseBlockItem()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_BLOCK_ITEM);

    SyntaxNodePtr child = nullptr;
    TokenPtr lookahead = _Lookahead();
    if (_Match(TokenType::TK_CONST, lookahead))
    {
        child = _ParseConstDecl();
    }
    else if (_Match(TokenType::TK_INT, lookahead))
    {
        child = _ParseVarDecl();
    }
    else
    {
        child = _ParseStmt();
    }

    // If failed, rollback.
    if (!child)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(child);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseMainFuncDef()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_MAIN_FUNC_DECL);

    // int
    if (!_Match(TokenType::TK_INT, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // main
    if (!_Match(TokenType::TK_MAIN, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // (
    if (!_Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // )
    if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Block
    SyntaxNodePtr block = _ParseBlock();
    if (!block)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(block);

    return root;
}

/*
 * ==================== Stmt ====================
 */

SyntaxNodePtr DefaultSyntacticParser::_ParseStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_STMT);

    SyntaxNodePtr child = nullptr;
    TokenPtr lookahead = _Lookahead();
    if (_Match(TokenType::TK_IDENTIFIER, lookahead))
    {
        // LVal expression starts with identifier but not followed by '('.
        // While expression can be identifier followed by '(', which is a function call.
        if (_Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead(2)))
        {
            child = _ParseExpStmt();
        }
        else
        {
            child = _ParseAssignmentStmt();
        }
    }
    else if (_Match(TokenType::TK_IF, lookahead))
    {
        child = _ParseIfStmt();
    }
    else if (_Match(TokenType::TK_FOR, lookahead))
    {
        child = _ParseForStmt();
    }
    else if (_Match(TokenType::TK_BREAK, lookahead))
    {
        child = _ParseBreakStmt();
    }
    else if (_Match(TokenType::TK_CONTINUE, lookahead))
    {
        child = _ParseContinueStmt();
    }
    else if (_Match(TokenType::TK_RETURN, lookahead))
    {
        child = _ParseReturnStmt();
    }
    else if (_Match(TokenType::TK_GETINT, lookahead))
    {
        child = _ParseInStmt();
    }
    else if (_Match(TokenType::TK_PRINTF, lookahead))
    {
        child = _ParseOutStmt();
    }
    else if (_Match(TokenType::TK_LEFT_BRACE, lookahead))
    {
        child = _ParseBlock();
    }
    else
    {
        // If not any of above, it must be an expression statement.
        child = _ParseExpStmt();
    }

    // If failed, rollback.
    if (!child)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(child);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseAssignmentStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_ASSIGNMENT_STMT);

    // LVal
    SyntaxNodePtr lval = _ParseLVal();
    if (!lval)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(lval);

    // '='
    if (!_Match(TokenType::TK_ASSIGN, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Exp
    SyntaxNodePtr exp = _ParseExp();
    if (!exp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(exp);

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseLVal()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_LVAL);

    // Identifier
    if (!_Match(TokenType::TK_IDENTIFIER, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Dimension
    while (_Match(TokenType::TK_LEFT_BRACKET, _Lookahead()))
    {
        // Skip '['
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        SyntaxNodePtr exp = _ParseExp();
        if (!exp)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(exp);

        // Check ']'
        if (!_Match(TokenType::TK_RIGHT_BRACKET, _Lookahead()))
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        // Skip ']'
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseCond()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_COND);

    SyntaxNodePtr orExp = _ParseOrExp();
    if (!orExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(orExp);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseIfStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_IF_STMT);

    // if
    if (!_Match(TokenType::TK_IF, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // '('
    if (!_Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Cond
    SyntaxNodePtr cond = _ParseCond();
    if (!cond)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(cond);

    // ')'
    if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Stmt
    SyntaxNodePtr stmt = _ParseStmt();
    if (!stmt)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(stmt);

    // else
    if (_Match(TokenType::TK_ELSE, _Lookahead()))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        stmt = _ParseStmt();
        if (!stmt)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(stmt);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseForStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_FOR_STMT);

    // for
    if (!_Match(TokenType::TK_FOR, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // '('
    if (!_Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Check existence of for init stmt
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        auto forInitStmt = _ParseForInitStmt();
        if (!forInitStmt)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(forInitStmt);
    }

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Check existence of Cond
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        auto cond = _ParseCond();
        if (!cond)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(cond);
    }

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Check existence of for step stmt
    if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
    {
        auto forStepStmt = _ParseForStepStmt();
        if (!forStepStmt)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(forStepStmt);
    }

    // ')'
    if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Stmt
    SyntaxNodePtr stmt = _ParseStmt();
    if (!stmt)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(stmt);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseForInitStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_FOR_INIT_STMT);

    // LVal
    SyntaxNodePtr lval = _ParseLVal();
    if (!lval)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(lval);

    // '='
    if (!_Match(TokenType::TK_ASSIGN, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Exp
    SyntaxNodePtr exp = _ParseExp();
    if (!exp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(exp);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseForStepStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_FOR_STEP_STMT);

    // LVal
    SyntaxNodePtr lval = _ParseLVal();
    if (!lval)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(lval);

    // '='
    if (!_Match(TokenType::TK_ASSIGN, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Exp
    SyntaxNodePtr exp = _ParseExp();
    if (!exp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(exp);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseExpStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_EXP_STMT);

    // Check existence of Exp
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        SyntaxNodePtr exp = _ParseExp();
        if (!exp)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(exp);
    }

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseBreakStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_BREAK_STMT);

    // break
    if (!_Match(TokenType::TK_BREAK, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseContinueStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_CONTINUE_STMT);

    // continue
    if (!_Match(TokenType::TK_CONTINUE, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseReturnStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_RETURN_STMT);

    // return
    if (!_Match(TokenType::TK_RETURN, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // Check existence of Exp
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        SyntaxNodePtr exp = _ParseExp();
        if (!exp)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(exp);
    }

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseInStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_IN_STMT);

    // LVal
    SyntaxNodePtr lval = _ParseLVal();
    if (!lval)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(lval);

    // '='
    if (!_Match(TokenType::TK_ASSIGN, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // getint
    if (!_Match(TokenType::TK_GETINT, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // '('
    if (!_Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // ')'
    if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseOutStmt()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_OUT_STMT);

    // printf
    if (!_Match(TokenType::TK_PRINTF, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // '('
    if (!_Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // FormatString
    if (!_Match(TokenType::TK_FORMAT, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // ',' Exp
    while (_Match(TokenType::TK_COMMA, _Lookahead()))
    {
        // Skip ','
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        SyntaxNodePtr exp = _ParseExp();
        if (!exp)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(exp);
    }

    // ')'
    if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }

    // ';'
    if (!_Match(TokenType::TK_SEMICOLON, _Lookahead()))
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_EXP);

    auto addExp = _ParseAddExp();
    if (!addExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(addExp);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseConstExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_CONST_EXP);

    auto addExp = _ParseAddExp();
    if (!addExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(addExp);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseAddExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_ADD_EXP);

    auto mulExp = _ParseMulExp();
    if (!mulExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(mulExp);

    auto addExpAux = _ParseAddExpAux();
    if (!addExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that addExpAux can be epsilon.
    if (!addExpAux->IsEpsilon())
    {
        root->InsertEndChild(addExpAux);
    }

    return root;
}

static std::vector<TokenType> _addExpAuxFirstSet = {
        TokenType::TK_PLUS,
        TokenType::TK_MINUS
};

SyntaxNodePtr DefaultSyntacticParser::_ParseAddExpAux()
{
    if (!_MatchAny(_addExpAuxFirstSet, _Lookahead()))
    {
        return _tree->NewEpsilonNode();
    }

    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_ADD_EXP);

    // '+' or '-'
    // Already checked in _MatchAny().
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // MulExp
    auto mulExp = _ParseMulExp();
    if (!mulExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(mulExp);

    // AddExpAux
    auto addExpAux = _ParseAddExpAux();
    if (!addExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that addExpAux can be epsilon.
    if (!addExpAux->IsEpsilon())
    {
        root->InsertEndChild(addExpAux);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseMulExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_MUL_EXP);

    auto unaryExp = _ParseUnaryExp();
    if (!unaryExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(unaryExp);

    auto mulExpAux = _ParseMulExpAux();
    if (!mulExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that mulExpAux can be epsilon.
    if (!mulExpAux->IsEpsilon())
    {
        root->InsertEndChild(mulExpAux);
    }

    return root;
}

static std::vector<TokenType> _mulExpAuxFirstSet = {
        TokenType::TK_MULTIPLY,
        TokenType::TK_DIVIDE,
        TokenType::TK_MOD
};

SyntaxNodePtr DefaultSyntacticParser::_ParseMulExpAux()
{
    if (!_MatchAny(_mulExpAuxFirstSet, _Lookahead()))
    {
        return _tree->NewEpsilonNode();
    }

    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_MUL_EXP);

    // '*' or '/' or '%'
    // Already checked in _MatchAny().
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // UnaryExp
    auto unaryExp = _ParseUnaryExp();
    if (!unaryExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(unaryExp);

    // MulExpAux
    auto mulExpAux = _ParseMulExpAux();
    if (!mulExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that mulExpAux can be epsilon.
    if (!mulExpAux->IsEpsilon())
    {
        root->InsertEndChild(mulExpAux);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseUnaryExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_UNARY_EXP);

    // UnaryExp -> UnaryOp UnaryExp
    auto unaryOp = _ParseUnaryOp();
    if (unaryOp)
    {
        root->InsertEndChild(unaryOp);
        auto unaryExp = _ParseUnaryExp();
        if (!unaryExp)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(unaryExp);
        return root;
    }

    // UnaryExp -> Ident '(' FuncAParams ')'
    if (_Match(TokenType::TK_IDENTIFIER, _Lookahead()) && _Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead(2)))
    {
        // Ident
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        // '('
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        // Check existence of FuncAParams
        if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
        {
            auto funcAParams = _ParseFuncAParams();
            if (!funcAParams)
            {
                _PostParseError(checkpoint, root);
                return nullptr;
            }
            root->InsertEndChild(funcAParams);
        }

        // ')'
        if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        return root;
    }

    // UnaryExp -> PrimaryExp
    auto primaryExp = _ParsePrimaryExp();
    if (!primaryExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(primaryExp);

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseUnaryOp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_UNARY_OP);

    // UnaryOp -> '+'
    if (_Match(TokenType::TK_PLUS, _Lookahead()))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        return root;
    }

    // UnaryOp -> '-'
    if (_Match(TokenType::TK_MINUS, _Lookahead()))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        return root;
    }

    // UnaryOp -> '!'
    if (_Match(TokenType::TK_NOT, _Lookahead()))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        return root;
    }

    // TODO: Error handling.

    return nullptr;
}

SyntaxNodePtr DefaultSyntacticParser::_ParsePrimaryExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_PRIMARY_EXP);

    // PrimaryExp -> Number
    if (_Match(TokenType::TK_NUMBER, _Lookahead()))
    {
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));
        return root;
    }

    // PrimaryExp -> '(' Exp ')'
    if (_Match(TokenType::TK_LEFT_PARENTHESIS, _Lookahead()))
    {
        // '('
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        // Exp
        auto exp = _ParseExp();
        if (!exp)
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(exp);

        // ')'
        if (!_Match(TokenType::TK_RIGHT_PARENTHESIS, _Lookahead()))
        {
            _PostParseError(checkpoint, root);
            return nullptr;
        }
        root->InsertEndChild(_tree->NewTerminalNode(_Next()));

        return root;
    }

    // PrimaryExp -> LVal
    auto lval = _ParseLVal();
    if (lval)
    {
        root->InsertEndChild(lval);
        return root;
    }

    // TODO: Error handling.

    return nullptr;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseOrExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_OR_EXP);

    auto andExp = _ParseAndExp();
    if (!andExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(andExp);

    auto orExpAux = _ParseOrExpAux();
    if (!orExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that orExpAux can be epsilon.
    if (!orExpAux->IsEpsilon())
    {
        root->InsertEndChild(orExpAux);
    }

    return root;
}

static std::vector<TokenType> _orExpAuxFirstSet = {
        TokenType::TK_OR
};

SyntaxNodePtr DefaultSyntacticParser::_ParseOrExpAux()
{
    if (!_MatchAny(_orExpAuxFirstSet, _Lookahead()))
    {
        return _tree->NewEpsilonNode();
    }

    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_OR_EXP);

    // '||'
    // Already checked in _MatchAny().
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // AndExp
    auto andExp = _ParseAndExp();
    if (!andExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(andExp);

    // OrExpAux
    auto orExpAux = _ParseOrExpAux();
    if (!orExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that orExpAux can be epsilon.
    if (!orExpAux->IsEpsilon())
    {
        root->InsertEndChild(orExpAux);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseAndExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_AND_EXP);

    auto eqExp = _ParseEqExp();
    if (!eqExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(eqExp);

    auto andExpAux = _ParseAndExpAux();
    if (!andExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that andExpAux can be epsilon.
    if (!andExpAux->IsEpsilon())
    {
        root->InsertEndChild(andExpAux);
    }

    return root;
}

static std::vector<TokenType> _andExpAuxFirstSet = {
        TokenType::TK_AND
};

SyntaxNodePtr DefaultSyntacticParser::_ParseAndExpAux()
{
    if (!_MatchAny(_andExpAuxFirstSet, _Lookahead()))
    {
        return _tree->NewEpsilonNode();
    }

    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_AND_EXP);

    // '&&'
    // Already checked in _MatchAny().
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // EqExp
    auto eqExp = _ParseEqExp();
    if (!eqExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(eqExp);

    // AndExpAux
    auto andExpAux = _ParseAndExpAux();
    if (!andExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that andExpAux can be epsilon.
    if (!andExpAux->IsEpsilon())
    {
        root->InsertEndChild(andExpAux);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseEqExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_EQ_EXP);

    auto relExp = _ParseRelExp();
    if (!relExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(relExp);

    auto eqExpAux = _ParseEqExpAux();
    if (!eqExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that eqExpAux can be epsilon.
    if (!eqExpAux->IsEpsilon())
    {
        root->InsertEndChild(eqExpAux);
    }

    return root;
}

static std::vector<TokenType> _eqExpAuxFirstSet = {
        TokenType::TK_EQUAL,
        TokenType::TK_NOT_EQUAL
};

SyntaxNodePtr DefaultSyntacticParser::_ParseEqExpAux()
{
    if (!_MatchAny(_eqExpAuxFirstSet, _Lookahead()))
    {
        return _tree->NewEpsilonNode();
    }

    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_EQ_EXP);

    // '==' or '!='
    // Already checked in _MatchAny().
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // RelExp
    auto relExp = _ParseRelExp();
    if (!relExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(relExp);

    // EqExpAux
    auto eqExpAux = _ParseEqExpAux();
    if (!eqExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that eqExpAux can be epsilon.
    if (!eqExpAux->IsEpsilon())
    {
        root->InsertEndChild(eqExpAux);
    }

    return root;
}

SyntaxNodePtr DefaultSyntacticParser::_ParseRelExp()
{
    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_REL_EXP);

    auto addExp = _ParseAddExp();
    if (!addExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(addExp);

    auto relExpAux = _ParseRelExpAux();
    if (!relExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that relExpAux can be epsilon.
    if (!relExpAux->IsEpsilon())
    {
        root->InsertEndChild(relExpAux);
    }

    return root;
}

static std::vector<TokenType> _relExpAuxFirstSet = {
        TokenType::TK_LESS,
        TokenType::TK_LESS_EQUAL,
        TokenType::TK_GREATER,
        TokenType::TK_GREATER_EQUAL
};

SyntaxNodePtr DefaultSyntacticParser::_ParseRelExpAux()
{
    if (!_MatchAny(_relExpAuxFirstSet, _Lookahead()))
    {
        return _tree->NewEpsilonNode();
    }

    auto checkpoint = _lexicalParser->SetCheckPoint();
    auto root = _tree->NewNonTerminalNode(SyntaxType::ST_REL_EXP);

    // '<' or '<=' or '>' or '>='
    // Already checked in _MatchAny().
    root->InsertEndChild(_tree->NewTerminalNode(_Next()));

    // AddExp
    auto addExp = _ParseAddExp();
    if (!addExp)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    root->InsertEndChild(addExp);

    // RelExpAux
    auto relExpAux = _ParseRelExpAux();
    if (!relExpAux)
    {
        _PostParseError(checkpoint, root);
        return nullptr;
    }
    // Notice that relExpAux can be epsilon.
    if (!relExpAux->IsEpsilon())
    {
        root->InsertEndChild(relExpAux);
    }

    return root;
}

TOMIC_END
