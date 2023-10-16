/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_RESILIENT_SYNTACTIC_PARSER_H_
#define _TOMIC_RESILIENT_SYNTACTIC_PARSER_H_

#include <tomic/Shared.h>
#include <tomic/parser/ISyntacticParser.h>
#include <tomic/lexer/ILexicalParser.h>
#include <tomic/lexer/token/ITokenMapper.h>
#include <tomic/parser/ast/SyntaxNode.h>
#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/ast/mapper/ISyntaxMapper.h>
#include <tomic/logger/debug/ILogger.h>
#include <tomic/logger/error/IErrorLogger.h>
#include <vector>

TOMIC_BEGIN

class ResilientSyntacticParser : public ISyntacticParser
{
public:
    ResilientSyntacticParser(
            ILexicalParserPtr lexicalParser,
            ISyntaxMapperPtr syntaxMapper,
            ITokenMapperPtr tokenMapper,
            IErrorLoggerPtr errorLogger,
            ILoggerPtr logger);
    ~ResilientSyntacticParser() override = default;

    ResilientSyntacticParser* SetReader(twio::IAdvancedReaderPtr reader) override;

    SyntaxTreePtr Parse() override;

private:
    ILexicalParserPtr _lexicalParser;
    ISyntaxMapperPtr _syntaxMapper;
    ITokenMapperPtr _tokenMapper;
    IErrorLoggerPtr _errorLogger;
    ILoggerPtr _logger;

    SyntaxTreePtr _tree;

    // Set to true if we are not sure what lies ahead. Since recursive
    // set may happen, so we make it int, and use it as a counter.
    int _tryParse;

private:
    // Return current token.
    TokenPtr _Current();
    // Return next token.
    TokenPtr _Next();
    // Lookahead for n tokens. Will make no effect to token stream.
    // n must greater than 0!
    TokenPtr _Lookahead(int n = 1);
    bool _Match(TokenType type, TokenPtr token);
    bool _MatchAny(const std::vector<TokenType>& types, TokenPtr token);
    void _PostParseError(int checkpoint, SyntaxNodePtr node);

    void _SetTryParse(bool tryParse);
    bool _IsTryParse() const { return _tryParse > 0; }

    void _Log(LogLevel level, TokenPtr position, const char* format, ...);
    void _Log(LogLevel level, TokenPtr position, const char* format, va_list argv);
    void _Log(LogLevel level, const char* format, ...);
    void _LogFailedToParse(SyntaxType type, LogLevel level = LogLevel::INFO);
    void _LogExpect(TokenType expected, LogLevel level = LogLevel::ERROR);
    void _LogExpect(const std::vector<TokenType>& expected, LogLevel level = LogLevel::ERROR);
    void _LogExpectAfter(TokenType expected, LogLevel level = LogLevel::ERROR);

    // Try to recover from missing token.
    // Will create an error log.
    void _RecoverFromMissingToken(SyntaxNodePtr node, TokenType expected);
    void _MarkCorrupted(SyntaxNodePtr node);

private:
    SyntaxNodePtr _ParseCompUnit();
    bool _MatchDecl();
    bool _MatchFuncDef();

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
    SyntaxNodePtr _ParseFuncFParam();
    SyntaxNodePtr _ParseFuncAParams();
    SyntaxNodePtr _ParseFuncAParam();

    SyntaxNodePtr _ParseBlock();
    SyntaxNodePtr _ParseBlockItem();

    ////////// MainFuncDef
    SyntaxNodePtr _ParseMainFuncDef();

    ////////// Stmt
    SyntaxNodePtr _ParseStmt();
    SyntaxNodePtr _ParseStmtAux();
    SyntaxNodePtr _ParseAssignmentStmt();
    SyntaxNodePtr _ParseLVal();
    SyntaxNodePtr _ParseCond();

    SyntaxNodePtr _ParseIfStmt();
    SyntaxNodePtr _ParseForStmt();
    SyntaxNodePtr _ParseForInitStmt();
    SyntaxNodePtr _ParseForStepStmt();
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
    SyntaxNodePtr _ParseAddExpAux();
    SyntaxNodePtr _ParseMulExp();
    SyntaxNodePtr _ParseMulExpAux();
    SyntaxNodePtr _ParseUnaryExp();
    SyntaxNodePtr _ParseUnaryOp();
    SyntaxNodePtr _ParsePrimaryExp();
    SyntaxNodePtr _ParseFuncCall();
    SyntaxNodePtr _ParseNumber();

    SyntaxNodePtr _ParseOrExp();
    SyntaxNodePtr _ParseOrExpAux();
    SyntaxNodePtr _ParseAndExp();
    SyntaxNodePtr _ParseAndExpAux();
    SyntaxNodePtr _ParseEqExp();
    SyntaxNodePtr _ParseEqExpAux();
    SyntaxNodePtr _ParseRelExp();
    SyntaxNodePtr _ParseRelExpAux();
};

TOMIC_END

#endif // _TOMIC_RESILIENT_SYNTACTIC_PARSER_H_
