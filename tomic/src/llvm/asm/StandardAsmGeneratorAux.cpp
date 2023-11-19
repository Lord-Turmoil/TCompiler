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
#include <tomic/llvm/ir/value/GlobalVariable.h>
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

/*
 * ==================== Overall Parsing ====================
 */
//
bool StandardAsmGenerator::VisitEnter(SyntaxNodePtr node)
{
    if (node->Type() == SyntaxType::ST_BLOCK_ITEM)
    {
        // Generate instruction, and stop visiting if
        // this block item does not nest any other block.
        return _ParseInstructions(node);
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
        if (it->Type() == SyntaxType::ST_DECL)
        {
            _ParseGlobalDecl(it);
        }
        else if (it->Type() == SyntaxType::ST_FUNC_DEF)
        {
            TOMIC_PANIC("Not implemented yet");
        }
        else if (it->Type() == SyntaxType::ST_MAIN_FUNC_DEF)
        {
            auto mainFunc = _ParseMainFunction(it);

            // TODO: Error handling.
            if (!mainFunc)
            {
                return false;
            }

            _module->SetMainFunction(mainFunc);
        }
        else
        {
            TOMIC_PANIC("Illegal child for CompUnit");
        }
    }

    return true;
}


FunctionPtr StandardAsmGenerator::_ParseMainFunction(SyntaxNodePtr node)
{
    auto context = _module->Context();

    // Generate the main function.
    TypePtr returnType = IntegerType::Get(context, 32);
    TypePtr funcType = FunctionType::Get(returnType);
    FunctionPtr function = Function::New(funcType, "main");

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
bool StandardAsmGenerator::_ParseInstructions(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node->Type() == SyntaxType::ST_BLOCK_ITEM);

    auto child = node->FirstChild();

    if (child->Type() == SyntaxType::ST_VAR_DECL)
    {
        _ParseVariableDecl(child);
    }
    else if (child->Type() == SyntaxType::ST_CONST_DECL)
    {
        _ParseVariableDecl(child);
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

        _ParseStatement(statement);
    }
    else
    {
        TOMIC_PANIC("Illegal type for BlockItem");
    }

    return false;
}


// Here the node is a specific statement.
void StandardAsmGenerator::_ParseStatement(SyntaxNodePtr node)
{
    switch (node->Type())
    {
    case  SyntaxType::ST_RETURN_STMT:
        _ParseReturnStatement(node);
        break;
    case SyntaxType::ST_ASSIGNMENT_STMT:
        _ParseAssignStatement(node);
        break;
    default:
        TOMIC_PANIC("Not implemented yet");
    }
}


/*
 * ==================== Utility Functions ====================
 */

SymbolTableBlockPtr StandardAsmGenerator::_GetSymbolTableBlock(SyntaxNodePtr node) const
{
    TOMIC_ASSERT(node);

    int tbl = SemanticUtil::GetInheritedIntAttribute(node, "tbl", -1);
    auto block = _symbolTable->GetBlock(tbl);

    TOMIC_ASSERT(block);

    return block;
}


void StandardAsmGenerator::_AddValue(SymbolTableEntrySmartPtr entry, ValuePtr value)
{
    TOMIC_ASSERT(entry && value);
    _valueMap[entry.get()] = value;
}


ValuePtr StandardAsmGenerator::_GetValue(SymbolTableEntrySmartPtr entry)
{
    TOMIC_ASSERT(entry);

    auto it = _valueMap.find(entry.get());

    if (it != _valueMap.end())
    {
        return it->second;
    }

    TOMIC_PANIC("Value not found");

    return nullptr;
}

// node is a LVal
ValuePtr StandardAsmGenerator::_GetLValValue(SyntaxNodePtr node)
{
    auto block = _GetSymbolTableBlock(node);
    auto entry = block->FindEntry(node->FirstChild()->Token()->lexeme);
    return _GetValue(entry);
}

/*
 * SymbolTableEntry to LLVM type.
 */

static TypePtr _GetVariableEntryType(LlvmContextPtr context, VariableEntryPtr entry);
static TypePtr _GetConstantEntryType(LlvmContextPtr context, ConstantEntryPtr entry);
static TypePtr _GetFunctionEntryType(LlvmContextPtr context, FunctionEntryPtr entry);
static TypePtr _GetFunctionParamType(LlvmContextPtr context, FunctionParamPropertyPtr param);


TypePtr StandardAsmGenerator::_GetEntryType(SymbolTableEntrySmartPtr entry)
{
    auto context = _module->Context();

    switch (entry->EntryType())
    {
    case SymbolTableEntryType::ET_VARIABLE:
        return _GetVariableEntryType(context, std::static_pointer_cast<VariableEntry>(entry));
    case SymbolTableEntryType::ET_CONSTANT:
        return _GetConstantEntryType(context, std::static_pointer_cast<ConstantEntry>(entry));
    case SymbolTableEntryType::ET_FUNCTION:
        return _GetFunctionEntryType(context, std::static_pointer_cast<FunctionEntry>(entry));
    default:
        TOMIC_PANIC("Not implemented yet");
    }

    return nullptr;
}


static TypePtr _GetVariableEntryType(LlvmContextPtr context, VariableEntryPtr entry)
{
    switch (entry->Dimension())
    {
    case 0:
        return context->GetInt32Ty();
    case 1:
        return ArrayType::Get(context->GetInt32Ty(), entry->ArraySize(0));
    case 2:
        return ArrayType::Get(
            ArrayType::Get(context->GetInt32Ty(), entry->ArraySize(1)),
            entry->ArraySize(0));
    default:
        TOMIC_PANIC("Not implemented yet.");
    }

    return nullptr;
}


static TypePtr _GetConstantEntryType(LlvmContextPtr context, ConstantEntryPtr entry)
{
    switch (entry->Dimension())
    {
    case 0:
        return context->GetInt32Ty();
    case 1:
        return ArrayType::Get(context->GetInt32Ty(), entry->ArraySize(0));
    case 2:
        return ArrayType::Get(
            ArrayType::Get(context->GetInt32Ty(), entry->ArraySize(1)),
            entry->ArraySize(0));
    default:
        TOMIC_PANIC("Not implemented yet.");
    }

    return nullptr;
}


static TypePtr _GetFunctionEntryType(LlvmContextPtr context, FunctionEntryPtr entry)
{
    TypePtr returnType;
    switch (entry->Type())
    {
    case SymbolValueType::VT_INT:
        returnType = context->GetInt32Ty();
        break;
    case SymbolValueType::VT_VOID:
        returnType = context->GetVoidTy();
        break;
    default:
        TOMIC_PANIC("Not implemented yet");
    }

    std::vector<TypePtr> paramTypes;
    for (int i = 0; i < entry->ArgsCount(); i++)
    {
        paramTypes.push_back(_GetFunctionParamType(context, entry->Param(i)));
    }

    return FunctionType::Get(returnType, paramTypes);
}


static TypePtr _GetFunctionParamType(LlvmContextPtr context, FunctionParamPropertyPtr param)
{
    switch (param->dimension)
    {
    case 0:
        return context->GetInt32Ty();
    case 1:
        return ArrayType::Get(context->GetInt32Ty(), param->size[0]);
    case 2:
        return ArrayType::Get(
            ArrayType::Get(context->GetInt32Ty(), param->size[1]),
            param->size[0]);
    default:
        TOMIC_PANIC("Not implemented yet");
    }

    return nullptr;
}


/*
 * ==================== Other utility functions ====================
 */
FunctionPtr StandardAsmGenerator::_SetCurrentFunction(FunctionPtr function)
{
    auto old = _currentFunction;
    _currentFunction = function;
    return old;
}


BasicBlockPtr StandardAsmGenerator::_SetCurrentBasicBlock(BasicBlockPtr block)
{
    TOMIC_ASSERT(_currentFunction && "Missing current function");

    auto old = _currentBlock;
    _currentBlock = block;

    // Add the basic block to the function.
    _currentFunction->InsertBasicBlock(block);

    return old;
}


InstructionPtr StandardAsmGenerator::_InsertInstruction(InstructionPtr instruction)
{
    TOMIC_ASSERT(_currentBlock && "Missing current basic block");

    _currentBlock->InsertInstruction(instruction);

    return instruction;
}


TOMIC_LLVM_END
