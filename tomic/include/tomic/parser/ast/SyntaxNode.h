/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/*
 * Dedicated to TinyXML, which is a great library.
 */

#ifndef _TOMIC_SYNTAX_NODE_H_
#define _TOMIC_SYNTAX_NODE_H_

#include "../../../../include/tomic/Common.h"
#include "../../lexer/token/Token.h"
#include "SyntaxType.h"
#include "AstForward.h"
#include <vector>
#include <string>
#include <map>

TOMIC_BEGIN

class SyntaxNode
{
    friend class SyntaxTree;
public:
    bool IsNonTerminal() const { return _nodeType == SyntaxNodeType::NON_TERMINAL; }
    bool IsTerminal() const { return _nodeType == SyntaxNodeType::TERMINAL; }
    bool IsEpsilon() const { return _nodeType == SyntaxNodeType::EPSILON; }

    // For visitor pattern.
    virtual bool Accept(AstVisitorPtr visitor) = 0;

    /*
     * ==================== AST Links ====================
     */
public:
    // Insert a child node to the end of the children list.
    // Return the added child.
    SyntaxNodePtr InsertEndChild(SyntaxNodePtr child);
    SyntaxNodePtr InsertFirstChild(SyntaxNodePtr child);
    SyntaxNodePtr InsertAfterChild(SyntaxNodePtr child, SyntaxNodePtr after);

    // Remove a child. This will not delete the node.
    // Return the node before the removed node.
    // To delete a node, use SyntaxTree::DeleteNode.
    SyntaxNodePtr RemoveChild(SyntaxNodePtr child);

    bool HasChildren() const { return _firstChild; }

    // Get the root of the tree, can be a sub-tree.
    SyntaxNodePtr Root() const;
    SyntaxNodePtr Parent() const { return _parent; }
    SyntaxNodePtr FirstChild() const { return _firstChild; }
    SyntaxNodePtr LastChild() const { return _lastChild; }
    SyntaxNodePtr NextSibling() const { return _next; }
    SyntaxNodePtr PrevSibling() const { return _prev; }

private:
    void _InsertChildPreamble(SyntaxNodePtr child);
    SyntaxNodePtr _Unlink(SyntaxNodePtr child);

    /*
     * ==================== AST Attributes ====================
     */
public:
    bool HasAttribute(const char* name) const;

    // Get the attribute value, or defaultValue if not found.
    const char* Attribute(const char* name, const char* value = nullptr) const;
    int IntAttribute(const char* name, int defaultValue = 0) const;
    bool BoolAttribute(const char* name, bool defaultValue = false) const;

    // Set the attribute value.
    SyntaxNodePtr SetAttribute(const char* name, const char* value);
    SyntaxNodePtr SetIntAttribute(const char* name, int value);
    SyntaxNodePtr SetBoolAttribute(const char* name, bool value);

    // Remove the attribute.
    SyntaxNodePtr RemoveAttribute(const char* name);

private:
    bool _FindAttribute(const char* name, std::map<std::string, std::string>::iterator* attr);
    bool _FindOrCreateAttribute(const char* name, std::map<std::string, std::string>::iterator* attr);

public:
    SyntaxType Type() const { return _type; }
    TokenPtr Token() const { return _token; }


protected:
    enum class SyntaxNodeType
    {
        NON_TERMINAL,
        TERMINAL,
        EPSILON
    };

    SyntaxNode(SyntaxNodeType nodeType, SyntaxType type);
    SyntaxNode(SyntaxNodeType nodeType, SyntaxType type, TokenPtr token);
    virtual ~SyntaxNode() = default;

    // AST links.
    SyntaxTree* _tree;

    SyntaxNodePtr _parent;
    SyntaxNodePtr _prev;
    SyntaxNodePtr _next;

    SyntaxNodePtr _firstChild;
    SyntaxNodePtr _lastChild;

    // AST properties.
    SyntaxType _type;
    TokenPtr _token;
    std::map<std::string, std::string> _attributes;

private:
    SyntaxNodeType _nodeType;
};

class NonTerminalSyntaxNode : public SyntaxNode
{
    friend class SyntaxTree;
protected:
    NonTerminalSyntaxNode(SyntaxType type);

    ~NonTerminalSyntaxNode() override = default;

    bool Accept(AstVisitorPtr visitor) override;
};


class TerminalSyntaxNode : public SyntaxNode
{
    friend class SyntaxTree;
protected:
    TerminalSyntaxNode(TokenPtr token);

    ~TerminalSyntaxNode() override = default;

    bool Accept(AstVisitorPtr visitor) override;
};

class EpsilonSyntaxNode : public SyntaxNode
{
    friend class SyntaxTree;
protected:
    EpsilonSyntaxNode();

    ~EpsilonSyntaxNode() override = default;

    bool Accept(AstVisitorPtr visitor) override;
};

TOMIC_END

#endif // _TOMIC_SYNTAX_NODE_H_
