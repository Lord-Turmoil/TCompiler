/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_DEFAULT_SYNTACTIC_PARSER_
#define _TOMIC_DEFAULT_SYNTACTIC_PARSER_

#include "../../Common.h"
#include "../ISyntacticParser.h"
#include "../../lexer/ILexicalParser.h"
#include "../ast/SyntaxNode.h"
#include "../ast/SyntaxTree.h"
#include <vector>

TOMIC_BEGIN

class DefaultSyntacticParser : public ISyntacticParser
{
public:
    DefaultSyntacticParser(ILexicalParserPtr lexicalParser);
    ~DefaultSyntacticParser() override = default;

    DefaultSyntacticParser* SetReader(twio::IAdvancedReaderPtr reader) override;

    SyntaxTreePtr Parse() override;

private:
    ILexicalParserPtr _lexicalParser;
    SyntaxTreePtr _tree;

private:
    // Lookahead for n tokens. Will make no effect to token stream.
    // n must greater than 0!
    TokenPtr _Lookahead(int n);
    bool _Match(TokenType type, TokenPtr token);
    void _PostParseError(int checkpoint, SyntaxNodePtr node);

    SyntaxNodePtr _ParseCompUnit();

    ////////// Decl
    SyntaxNodePtr _ParseDecl();
    SyntaxNodePtr _ParseBType();
    // ConstDecl
    SyntaxNodePtr _ParseConstDecl();
    SyntaxNodePtr _ParseConstDef();
    SyntaxNodePtr _ParseConstInitVal();
    // VarDecl
    SyntaxNodePtr _ParseVarDecl();
    SyntaxNodePtr _ParseVarDef();
    SyntaxNodePtr _ParseInitVal();

    ////////// FuncDef
    SyntaxNodePtr _ParseFuncDef();
    SyntaxNodePtr _ParseFuncType();
    SyntaxNodePtr _ParseFuncFParams();
    SyntaxNodePtr _ParseFuncAParams();

    SyntaxNodePtr _ParseBlock();
    SyntaxNodePtr _ParseBlockItem();

    ////////// MainFuncDef
    SyntaxNodePtr _ParseMainFuncDef();

    ////////// Stmt
    SyntaxNodePtr _ParseStmt();
    SyntaxNodePtr _ParseLVal();
    SyntaxNodePtr _ParseCond();

    SyntaxNodePtr _ParseIfStmt();
    SyntaxNodePtr _ParseForStmt();
    SyntaxNodePtr _ParseForCondStmt();
    SyntaxNodePtr _ParseExpStmt();

    SyntaxNodePtr _ParseBreakStmt();
    SyntaxNodePtr _ParseContinueStmt();
    SyntaxNodePtr _ParseReturnStmt();

    SyntaxNodePtr _ParseInStmt();
    SyntaxNodePtr _ParseOutStmt();

    ////////// Exp
    SyntaxNodePtr _ParseExp();
    SyntaxNodePtr _ParseConstExp();
    SyntaxNodePtr _ParseAddExp();
    SyntaxNodePtr _ParseMulExp();
    SyntaxNodePtr _ParseUnaryExp();
    SyntaxNodePtr _ParseUnaryOp();
    SyntaxNodePtr _ParsePrimaryExp();
    SyntaxNodePtr _ParseNumber();
    SyntaxNodePtr _ParseOrExp();
    SyntaxNodePtr _ParseAndExp();
    SyntaxNodePtr _ParseEqExp();
    SyntaxNodePtr _ParseRelExp();
};

TOMIC_END

#endif // _TOMIC_DEFAULT_SYNTACTIC_PARSER_
