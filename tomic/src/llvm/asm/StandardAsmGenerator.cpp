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
 * ==================== Instruction Parsing ====================
 */

/*
 * Node is a Decl, it can be a VarDecl or ConstDecl.
 */

// node is a Decl
void StandardAsmGenerator::_ParseGlobalDecl(SyntaxNodePtr node)
{
    auto child = node->FirstChild();
    if (child->Type() == SyntaxType::ST_VAR_DECL)
    {
        for (auto it = child->FirstChild(); it; it = it->NextSibling())
        {
            if (it->Type() == SyntaxType::ST_VAR_DEF)
            {
                _module->AddGlobalVariable(_ParseGlobalVarDef(it));
            }
        }
    }
    else if (child->Type() == SyntaxType::ST_CONST_DECL)
    {
        for (auto it = child->FirstChild(); it; it = it->NextSibling())
        {
            if (it->Type() == SyntaxType::ST_CONST_DEF)
            {
                _module->AddGlobalVariable(_ParseGlobalConstant(it));
            }
        }
    }
    else
    {
        TOMIC_PANIC("Illegal type for Decl");
    }
}

GlobalVariablePtr StandardAsmGenerator::_ParseGlobalVarDef(SyntaxNodePtr node)
{
    // Get variable name.
    const std::string& name = node->FirstChild()->Token()->lexeme;
    auto entry = _GetSymbolTableBlock(node)->FindEntry(name);
    auto type = _GetEntryType(entry);
    GlobalVariablePtr value;

    if (node->LastChild()->Type() == SyntaxType::ST_INIT_VAL)
    {
        // with init value
        auto initValue = _ParseGlobalInitValue(node->LastChild());
        value = GlobalVariable::New(type, false, name, initValue);
    }
    else
    {
        value = GlobalVariable::New(type, false, name);
    }

    // Add the value to the symbol table.
    _AddValue(entry, value);

    return value;
}

GlobalVariablePtr StandardAsmGenerator::_ParseGlobalConstant(SyntaxNodePtr node)
{
    // Get constant name.
    const std::string& name = node->FirstChild()->Token()->lexeme;
    auto entry = _GetSymbolTableBlock(node)->FindEntry(name);
    auto type = _GetEntryType(entry);
    GlobalVariablePtr value;

    if (node->LastChild()->Type() == SyntaxType::ST_CONST_INIT_VAL)
    {
        // with init value
        auto initValue = _ParseGlobalInitValue(node->LastChild());
        value = GlobalVariable::New(type, true, name, initValue);
    }
    else
    {
        TOMIC_PANIC("Constant must have init value");
    }

    // Add the value to the symbol table.
    _AddValue(entry, value);

    return value;
}

// node is a InitVal or ConstInitVal.
ConstantDataPtr StandardAsmGenerator::_ParseGlobalInitValue(SyntaxNodePtr node)
{
    if (!node->BoolAttribute("det"))
    {
        TOMIC_PANIC("Global initialization value must be deterministic");
    }

    int dim = node->IntAttribute("dim");
    if (dim == 0)
    {
        return ConstantData::New(_module->Context()->GetInt32Ty(), node->IntAttribute("value"));
    }

    std::vector<ConstantDataPtr> values;
    for (auto it = node->FirstChild(); it; it = it->NextSibling())
    {
        if ((it->Type() == SyntaxType::ST_CONST_INIT_VAL) || (it->Type() == SyntaxType::ST_INIT_VAL))
        {
            values.push_back(_ParseGlobalInitValue(it));
        }
    }

    return ConstantData::New(values);
}

ReturnInstPtr StandardAsmGenerator::_ParseReturnStatement(SyntaxNodePtr node)
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
        auto value = _ParseExpression(exp);

        // TODO: Error handling.
        TOMIC_ASSERT(value);

        inst = ReturnInst::New(context, value);
    }

    // Add the instruction to the current basic block.
    _InsertInstruction(inst);

    return inst;
}

ValuePtr StandardAsmGenerator::_ParseExpression(SyntaxNodePtr node)
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


TOMIC_LLVM_END
