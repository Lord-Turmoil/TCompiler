/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/asm/impl/StandardAsmGenerator.h>
#include <tomic/llvm/ir/DerivedTypes.h>
#include <tomic/llvm/ir/LlvmContext.h>
#include <tomic/llvm/ir/Module.h>
#include <tomic/llvm/ir/Type.h>
#include <tomic/llvm/ir/value/Argument.h>
#include <tomic/llvm/ir/value/BasicBlock.h>
#include <tomic/llvm/ir/value/Function.h>
#include <tomic/llvm/ir/value/inst/Instructions.h>
#include <tomic/llvm/ir/value/Value.h>
#include <tomic/parser/ast/SyntaxNode.h>
#include <tomic/parser/ast/SyntaxTree.h>
#include <tomic/parser/table/SymbolTable.h>
#include <tomic/parser/table/SymbolTableBlock.h>
#include <tomic/parser/table/SymbolTableEntry.h>
#include <tomic/utils/SemanticUtil.h>
#include <vector>

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
            _ParseFunction(it);
        }
        else if (it->Type() == SyntaxType::ST_MAIN_FUNC_DEF)
        {
            _ParseMainFunction(it);
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
    FunctionPtr function = Function::New(IntegerType::Get(context, 32), "main");

    // Set the current function and basic block as parents.
    _SetCurrentFunction(function);
    _SetCurrentBasicBlock(function->NewBasicBlock());

    // Generate all instructions in the main function.
    node->Accept(this);

    _module->SetMainFunction(function);

    return function;
}


// node is a FuncDef
FunctionPtr StandardAsmGenerator::_ParseFunction(SyntaxNodePtr node)
{
    // Get return type.
    auto decl = node->FirstChild();
    TypePtr returnType = _GetNodeType(decl);

    /*
     * Since arguments are in symbol table of body, so we have to
     * get them from the symbol table of the body.
     */
    auto block = _GetSymbolTableBlock(node->LastChild());
    std::vector<ArgumentPtr> args;
    auto params = SemanticUtil::GetChildNode(decl, SyntaxType::ST_FUNC_FPARAMS);
    if (params && params->HasChildren())
    {
        int argNo = 0;
        for (auto param = params->FirstChild(); param; param = param->NextSibling())
        {
            if (param->Type() == SyntaxType::ST_FUNC_FPARAM)
            {
                args.push_back(_ParseArgument(param, argNo++, block));
            }
        }
    }

    // Get function.
    const std::string& name = decl->ChildAt(1)->Token()->lexeme;
    auto entry = _GetSymbolTableBlock(node)->FindEntry(name);
    FunctionPtr function = Function::New(returnType, name, args);
    auto body = _InitFunctionParams(function, block);

    // Set the current function and basic block as parents.
    _SetCurrentFunction(function);
    _SetCurrentBasicBlock(body);

    // Before we continue, we should add function to our value map.
    _AddValue(entry, function);

    // Generate all instructions in the main function.
    node->Accept(this);

    _module->AddFunction(function);

    return function;
}


ArgumentPtr StandardAsmGenerator::_ParseArgument(SyntaxNodePtr node, int argNo, SymbolTableBlockPtr block)
{
    std::string name = node->Attribute("name");
    auto entry = block->FindEntry(name);
    TOMIC_ASSERT(entry);
    auto type = _GetEntryType(entry);

    if (type->IsIntegerTy())
    {
        return Argument::New(type, name, argNo);
    }

    return Argument::New(PointerType::Get(type->As<ArrayType>()->ElementType()), name, argNo);
}


BasicBlockPtr StandardAsmGenerator::_InitFunctionParams(FunctionPtr function, SymbolTableBlockPtr block)
{
    auto body = BasicBlock::New(function);

    std::vector<ValuePtr> allocas;

    for (auto it = function->ArgBegin(); it != function->ArgEnd(); ++it)
    {
        auto alloca = AllocaInst::New((*it)->GetType());
        auto entry = block->FindEntry((*it)->GetName());
        body->InsertInstruction(alloca);
        allocas.push_back(alloca);
        _AddValue(entry, alloca);
    }
    for (auto it = function->ArgBegin(); it != function->ArgEnd(); ++it)
    {
        body->InsertInstruction(StoreInst::New(*it, allocas[it - function->ArgBegin()]));
    }

    function->InsertBasicBlock(body);

    return body;
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
    case SyntaxType::ST_RETURN_STMT:
        _ParseReturnStatement(node);
        break;
    case SyntaxType::ST_ASSIGNMENT_STMT:
        _ParseAssignStatement(node);
        break;
    case SyntaxType::ST_EXP_STMT:
        _ParseExpression(node->FirstChild());
        break;
    case SyntaxType::ST_IN_STMT:
        _ParseInputStatement(node);
        break;
    case SyntaxType::ST_OUT_STMT:
        _ParseOutputStatement(node);
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


void StandardAsmGenerator::_AddValue(SymbolTableEntryPtr entry, ValuePtr value)
{
    TOMIC_ASSERT(entry && value);
    _valueMap[entry] = value;
}


ValuePtr StandardAsmGenerator::_GetValue(SymbolTableEntryPtr entry)
{
    TOMIC_ASSERT(entry);

    auto it = _valueMap.find(entry);

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


// node is a FunctionCall
FunctionPtr StandardAsmGenerator::_GetFunction(SyntaxNodePtr node)
{
    auto block = _GetSymbolTableBlock(node);
    auto entry = block->FindEntry(node->FirstChild()->Token()->lexeme);

    return _GetValue(entry)->As<Function>();
}


/*
 * SymbolTableEntry to LLVM type.
 */

static TypePtr _GetVariableEntryType(LlvmContextPtr context, VariableEntryPtr entry);
static TypePtr _GetConstantEntryType(LlvmContextPtr context, ConstantEntryPtr entry);
static TypePtr _GetFunctionEntryType(LlvmContextPtr context, FunctionEntryPtr entry);
static TypePtr _GetFunctionParamType(LlvmContextPtr context, FunctionParamPropertyPtr param);


TypePtr StandardAsmGenerator::_GetEntryType(SymbolTableEntryPtr entry)
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


TypePtr StandardAsmGenerator::_GetNodeType(SyntaxNodePtr node)
{
    TOMIC_ASSERT(node->HasAttribute("type"));

    switch (static_cast<SymbolValueType>(node->IntAttribute("type")))
    {
    case SymbolValueType::VT_INT:
        return IntegerType::Get(_module->Context(), 32);
    case SymbolValueType::VT_VOID:
        return Type::GetVoidTy(_module->Context());
    default:
        TOMIC_PANIC("Type not supported");
    }

    return nullptr;
}


static TypePtr _GetVariableEntryType(LlvmContextPtr context, VariableEntryPtr entry)
{
    switch (entry->Dimension())
    {
    case 0:
        return IntegerType::Get(context, 32);
    case 1:
        return ArrayType::Get(IntegerType::Get(context, 32), entry->ArraySize(0));
    case 2:
        return ArrayType::Get(
            ArrayType::Get(IntegerType::Get(context, 32), entry->ArraySize(1)),
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
        return IntegerType::Get(context, 32);
    case 1:
        return ArrayType::Get(IntegerType::Get(context, 32), entry->ArraySize(0));
    case 2:
        return ArrayType::Get(
            ArrayType::Get(IntegerType::Get(context, 32), entry->ArraySize(1)),
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
        returnType = IntegerType::Get(context, 32);
        break;
    case SymbolValueType::VT_VOID:
        returnType = Type::GetVoidTy(context);
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
        return IntegerType::Get(context, 32);
    case 1:
        return PointerType::Get(IntegerType::Get(context, 32));
    case 2:
        return PointerType::Get(ArrayType::Get(IntegerType::Get(context, 32), param->size[1]));
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

    /*
     * 2023/11/19 TS:
     * Add the basic block to the function if necessary.
     * The block may already be in the function!
     */
    if (block->Parent() != _currentFunction)
    {
        _currentFunction->InsertBasicBlock(block);
    }

    return old;
}


InstructionPtr StandardAsmGenerator::_InsertInstruction(InstructionPtr instruction)
{
    TOMIC_ASSERT(_currentBlock && "Missing current basic block");

    _currentBlock->InsertInstruction(instruction);

    return instruction;
}


TOMIC_LLVM_END
