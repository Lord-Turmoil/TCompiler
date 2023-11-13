/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/asm/impl/StandardAsmGenerator.h>
#include <tomic/llvm/ir/Module.h>
#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/DerivedTypes.h>
#include <tomic/llvm/ir/value/Value.h>
#include <tomic/llvm/ir/value/Argument.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/value/BasicBlock.h>
#include <tomic/llvm/ir/value/User.h>
#include <tomic/llvm/ir/value/ConstantData.h>
#include <tomic/llvm/ir/value/inst/Instructions.h>

#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/ast/SyntaxNode.h>

#include <tomic/parser/table/SymbolTable.h>
#include <tomic/parser/table/SymbolTableBlock.h>
#include <tomic/parser/table/SymbolTableEntry.h>

#include <tomic/utils/SymbolTableUtil.h>
#include <tomic/utils/SemanticUtil.h>

TOMIC_LLVM_BEGIN

StandardAsmGenerator::StandardAsmGenerator()
        : _currentFunction(nullptr), _currentBlock(nullptr)
{
}

ModuleSmartPtr StandardAsmGenerator::Generate(
        SyntaxTreePtr syntaxTree,
        SymbolTablePtr symbolTable,
        const char* name)
{
    _syntaxTree = syntaxTree;
    _symbolTable = symbolTable;

    // Module will check if the name is nullptr or not.
    _module = Module::New(name);

    if (!_ParseCompilationUnit())
    {
        return nullptr;
    }

    return _module;
}

/*
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *                      Overall Generation
 * ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

bool StandardAsmGenerator::VisitEnter(SyntaxNodePtr node)
{
    if (node->Type() == SyntaxType::ST_BLOCK_ITEM)
    {
        // Generate instruction, and stop visiting if
        // this block item does not nest any other block.
        return _GenerateInstructions(node);
    }

    return true;
}

bool StandardAsmGenerator::_ParseCompilationUnit()
{
    auto root = _syntaxTree->Root();

    TOMIC_ASSERT(root);
    TOMIC_ASSERT(root->Type() == SyntaxType::ST_COMP_UNIT);

    for (auto it = root->FirstChild(); it; it = it->NextSibling())
    {
        if (it->Type() == SyntaxType::ST_MAIN_FUNC_DEF)
        {
            auto mainFunc = _GenerateMainFunction(it);

            // TODO: Error handling.
            if (!mainFunc)
            {
                return false;
            }

            _module->SetMainFunction(mainFunc);
        }
    }

    return true;
}

FunctionPtr StandardAsmGenerator::_GenerateMainFunction(SyntaxNodePtr node)
{
    auto context = _module->Context();

    // Generate the main function.
    TypePtr returnType = IntegerType::Get(context, 32);
    FunctionPtr function = Function::New(returnType, "main");

    // Generate the first basic block.
    BasicBlockPtr block = BasicBlock::New(function);

    // Set the current function and basic block as parents.
    _SetCurrentFunction(function);
    _SetCurrentBasicBlock(block);

    // Generate all instructions in the main function.
    node->Accept(this);

    return function;
}

// Here, node is a BlockItem.
bool StandardAsmGenerator::_GenerateInstructions(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node->Type() == SyntaxType::ST_BLOCK_ITEM);

    auto child = node->FirstChild();

    if (child->Type() == SyntaxType::ST_DECL)
    {
        // Generate Decl
    }
    else if (child->Type() == SyntaxType::ST_STMT)
    {
        // Generate Stmt
        auto statement = child->FirstChild();
        // If is a nested block, return true to continue visiting.
        if (statement->Type() == SyntaxType::ST_BLOCK)
        {
            return true;
        }

        _GenerateStatement(statement);
    }
    else
    {
        TOMIC_PANIC("Illegal type for BlockItem");
    }

    return false;
}

// Here the node is a specific statement.
void StandardAsmGenerator::_GenerateStatement(SyntaxNodePtr node)
{
    if (node->Type() == SyntaxType::ST_RETURN_STMT)
    {
        _GenerateReturnStatement(node);
    }
}


/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *                     Concrete Parsing
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

ReturnInstPtr StandardAsmGenerator::_GenerateReturnStatement(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node->Type() == SyntaxType::ST_RETURN_STMT);
    auto context = _module->Context();
    ReturnInstPtr inst;

    // Generate return value.
    auto exp = SemanticUtil::GetChildNode(node, SyntaxType::ST_EXP);
    if (exp == nullptr)
    {
        // return;
        inst = ReturnInst::New(context);
    }
    else
    {
        auto value = _GenerateExpression(exp);

        // TODO: Error handling.
        TOMIC_ASSERT(value);

        inst = ReturnInst::New(context, value);
    }

    return inst;
}

ValuePtr StandardAsmGenerator::_GenerateExpression(SyntaxNodePtr node)
{
    auto context = _module->Context();

    if (node->BoolAttribute("det"))
    {
        int value = node->IntAttribute("value");
        auto type = context->GetInt32Ty();
        return ConstantData::New(type, value);
    }

    TOMIC_PANIC("Not implemented yet");

    return nullptr;
}

/*
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 *                    Helper Functions
 * +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 */

SymbolTableBlockPtr StandardAsmGenerator::_GetSymbolTableBlock(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node);

    int tbl = SemanticUtil::GetInheritedIntAttribute(node, "tbl", -1);
    auto block = _symbolTable->GetBlock(tbl);

    TOMIC_ASSERT(block);

    return block;
}

FunctionPtr StandardAsmGenerator::_SetCurrentFunction(FunctionPtr function)
{
    auto old = _currentFunction;
    _currentFunction = function;
    return old;
}

BasicBlockPtr StandardAsmGenerator::_SetCurrentBasicBlock(BasicBlockPtr block)
{
    auto old = _currentBlock;
    _currentBlock = block;
    return old;
}

TOMIC_LLVM_END
