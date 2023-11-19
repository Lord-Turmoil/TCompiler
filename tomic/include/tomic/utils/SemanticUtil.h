/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SEMANTIC_UTIL_H_
#define _TOMIC_SEMANTIC_UTIL_H_

#include <tomic/parser/ast/SyntaxNode.h>
#include <tomic/parser/ast/SyntaxType.h>
#include <tomic/parser/table/SymbolTableForward.h>

#include <string>
#include <vector>

TOMIC_BEGIN

/*
 * This namespace contains some utility functions for semantic analysis.
 * The reason why we put them here is to reduce the complexity of our
 * Semantic Analyzer.
 */
namespace SemanticUtil
{

// Count the number of direct child nodes of a specific type.
int CountDirectChildNode(SyntaxNodePtr node, SyntaxType type);
int CountDirectTerminalNode(SyntaxNodePtr node, TokenType type);

SyntaxNodePtr GetDirectChildNode(SyntaxNodePtr node, SyntaxType type, int index = 1);
bool GetDirectChildNodes(SyntaxNodePtr node, SyntaxType type, std::vector<SyntaxNodePtr>& list);

SyntaxNodePtr GetChildNode(SyntaxNodePtr node, SyntaxType type, int index = 1);

bool HasParent(SyntaxNodePtr node, SyntaxType type);

/*
 * ==================== Attributes ====================
 * Utility functions for attributes.
 * Also adapted from TinyXML2, using query based methods.
 */

// Get attribute of node itself.
bool HasAttribute(SyntaxNodePtr node, const char* name);
const char* GetAttribute(SyntaxNodePtr node, const char* name, const char* defaultValue = nullptr);
int GetIntAttribute(SyntaxNodePtr node, const char* name, int defaultValue = 0);
bool GetBoolAttribute(SyntaxNodePtr node, const char* name, bool defaultValue = false);

bool QueryAttribute(SyntaxNodePtr node, const char* name, const char** value, const char* defaultValue = nullptr);
bool QueryIntAttribute(SyntaxNodePtr node, const char* name, int* value, int defaultValue = 0);
bool QueryBoolAttribute(SyntaxNodePtr node, const char* name, bool* value, bool defaultValue = false);

// Get Inherited Attribute value
bool HasInheritedAttribute(SyntaxNodePtr node, const char* name);
const char* GetInheritedAttribute(SyntaxNodePtr node, const char* name, const char* defaultValue = nullptr);
int GetInheritedIntAttribute(SyntaxNodePtr node, const char* name, int defaultValue = 0);
bool GetInheritedBoolAttribute(SyntaxNodePtr node, const char* name, bool defaultValue = false);

// Get Synthesized Attribute value
// Synthesized attributes comes from nodes in front of the current node.
bool HasSynthesizedAttribute(SyntaxNodePtr node, const char* name);
const char* GetSynthesizedAttribute(SyntaxNodePtr node, const char* name, const char* defaultValue = nullptr);
int GetSynthesizedIntAttribute(SyntaxNodePtr node, const char* name, int defaultValue = 0);
bool GetSynthesizedBoolAttribute(SyntaxNodePtr node, const char* name, bool defaultValue = false);

// Array serialization
// The format is like this:
// 1,2,3;4,5,6;7,8,9
std::string SerializeArray(const std::vector<std::vector<int>>& array);
std::vector<std::vector<int>> DeserializeArray(const char* str);

// For format string.
int GetFormatStringArgCount(const char* format);

// Compile-time calculation.
int EvaluateBinary(const char* op, int left, int right);
int EvaluateUnary(const char* op, int value);

int EvaluateNumber(SyntaxNodePtr node);

bool TryEvaluateLVal(SyntaxNodePtr node, SymbolTableBlockPtr block, int* value);


}

TOMIC_END

#endif // _TOMIC_SEMANTIC_UTIL_H_
