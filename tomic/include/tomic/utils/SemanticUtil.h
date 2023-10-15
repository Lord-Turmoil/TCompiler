/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SEMANTIC_UTIL_H_
#define _TOMIC_SEMANTIC_UTIL_H_

#include <tomic/Shared.h>
#include <tomic/parser/ast/AstForward.h>
#include <tomic/parser/ast/SyntaxType.h>
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
int CountDirectChildNode(const SyntaxNodePtr node, SyntaxType type);

/*
 * ==================== Attributes ====================
 * Utility functions for attributes.
 * Also adapted from TinyXML2, using query based methods.
 */

// Get attribute of node itself.
bool HasAttribute(const SyntaxNodePtr node, const char* name);
const char* GetAttribute(const SyntaxNodePtr node, const char* name, const char* defaultValue = nullptr);
int GetIntAttribute(const SyntaxNodePtr node, const char* name, int defaultValue = 0);
bool GetBoolAttribute(const SyntaxNodePtr node, const char* name, bool defaultValue = false);

bool QueryAttribute(const SyntaxNodePtr node, const char* name, const char** value, const char* defaultValue = nullptr);
bool QueryIntAttribute(const SyntaxNodePtr node, const char* name, int* value, int defaultValue = 0);
bool QueryBoolAttribute(const SyntaxNodePtr node, const char* name, bool* value, bool defaultValue = false);

// Get Inherited Attribute value
bool HasInheritedAttribute(const SyntaxNodePtr node, const char* name);
const char* GetInheritedAttribute(const SyntaxNodePtr node, const char* name, const char* defaultValue = nullptr);
int GetInheritedIntAttribute(const SyntaxNodePtr node, const char* name, int defaultValue = 0);
bool GetInheritedBoolAttribute(const SyntaxNodePtr node, const char* name, bool defaultValue = false);

// Get Synthesized Attribute value
// Synthesized attributes comes from nodes in front of the current node.
bool HasSynthesizedAttribute(const SyntaxNodePtr node, const char* name);
const char* GetSynthesizedAttribute(const SyntaxNodePtr node, const char* name, const char* defaultValue = nullptr);
int GetSynthesizedIntAttribute(const SyntaxNodePtr node, const char* name, int defaultValue = 0);
bool GetSynthesizedBoolAttribute(const SyntaxNodePtr node, const char* name, bool defaultValue = false);

// Array serialization
// The format is like this:
// 1,2,3;4,5,6;7,8,9
const char* SerializeArray(const std::vector<std::vector<int>>& array);
std::vector<std::vector<int>> DeserializeArray(const char* str);

}

TOMIC_END

#endif // _TOMIC_SEMANTIC_UTIL_H_
