/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYMBOL_TABLE_ENTRY_H_
#define _TOMIC_SYMBOL_TABLE_ENTRY_H_

#include <tomic/Shared.h>
#include <tomic/parser/table/SymbolTableForward.h>
#include <string>

TOMIC_BEGIN

enum class SymbolTableEntryType
{
    ET_UNKNOWN,
    ET_VARIABLE,
    ET_FUNCTION,

    ET_COUNT
};

enum class ValueType
{
    VT_VOID,
    VT_INT,
    VT_CHAR,
    VT_BOOL,

    VT_COUNT
};

class SymbolTableEntry
{
public:
    virtual ~SymbolTableEntry() = default;

    SymbolTableEntryType Type() const { return _type; }
    const char* Name() const { return _name.c_str(); }

protected:
    // So it cannot be instantiated directly.
    SymbolTableEntry(const std::string& name, SymbolTableEntryType type)
            : _name(name), _type(type) {}

    SymbolTableEntryType _type;
    std::string _name;
};


/*
 * ==================== Symbol Table Variable Entry ====================
 */
const int MAX_ARRAY_DIMENSION = 2;

struct VariableEntryProperty
{
    ValueType type;
    bool isConst;

    // a[10][100] -> size = { 100, 1 }
    int dimension;
    int size[MAX_ARRAY_DIMENSION];

    VariableEntryProperty()
            : type(ValueType::VT_INT), isConst(false), dimension(0), size{ 0 } {}
};

class VariableEntry final : public SymbolTableEntry
{
    friend class VariableEntryBuilder;
public:
    ~VariableEntry() override = default;

public:
    bool IsConst() const { return _props.isConst; }

    int Dimension() const { return _props.dimension; }

    int ArraySize(int dimension) const
    {
        TOMIC_ASSERT(dimension < MAX_ARRAY_DIMENSION);
        return _props.size[dimension];
    }

private:
    VariableEntry(const std::string& name)
            : SymbolTableEntry(name, SymbolTableEntryType::ET_VARIABLE) {}

    static std::shared_ptr<VariableEntry> New(const std::string& name)
    {
        return std::shared_ptr<VariableEntry>(new VariableEntry(name));
    }

    VariableEntryProperty _props;
};

using VariableEntryPtr = std::shared_ptr<VariableEntry>;

class VariableEntryBuilder
{
public:
    VariableEntryBuilder(const std::string& name) : _name(name) {}
    ~VariableEntryBuilder() = default;

    VariableEntryBuilder* IsConst(bool isConst)
    {
        _props.isConst = isConst;
        return this;
    }

    VariableEntryBuilder* Dimension(int dimension)
    {
        _props.dimension = dimension;
        return this;
    }

    VariableEntryBuilder* ArraySize(int dimension, int size)
    {
        TOMIC_ASSERT(dimension < MAX_ARRAY_DIMENSION);
        _props.size[dimension] = size;
        return this;
    }

    VariableEntryPtr Build()
    {
        auto entry = VariableEntry::New(_name);
        entry->_props = _props;
        return entry;
    }

private:
    std::string _name;
    VariableEntryProperty _props;
};


/*
 * ==================== Symbol Table Function Entry ====================
 */

struct FunctionParamProperty
{
    ValueType type;
    int dimension;
    int size[MAX_ARRAY_DIMENSION];

    FunctionParamProperty(ValueType type, int dimension, int size1, int size2)
            : type(type), dimension(dimension), size{ size1, size2 }
    {
        TOMIC_ASSERT(dimension >= 0);
        TOMIC_ASSERT(dimension <= MAX_ARRAY_DIMENSION);
    }
};

struct FunctionEntryProperty
{
    int isVoid;
    std::vector<FunctionParamProperty> params;

    int ArgsCount() const { return params.size(); }
};

class FunctionEntry final : public SymbolTableEntry
{
    friend class FunctionEntryBuilder;
public:
    ~FunctionEntry() override = default;

    bool IsVoid() const { return _props.isVoid; }
    int ArgsCount() const { return _props.ArgsCount(); }
    FunctionParamProperty Param(int index) const
    {
        TOMIC_ASSERT(index < ArgsCount());
        return _props.params[index];
    }

private:
    FunctionEntry(const std::string& name)
            : SymbolTableEntry(name, SymbolTableEntryType::ET_FUNCTION) {}

    static std::shared_ptr<FunctionEntry> New(const std::string& name)
    {
        return std::shared_ptr<FunctionEntry>(new FunctionEntry(name));
    }

    FunctionEntryProperty _props;
};

using FunctionEntryPtr = std::shared_ptr<FunctionEntry>;

class FunctionEntryBuilder
{
public:
    FunctionEntryBuilder(const std::string& name) : _name(name) {}

    ~FunctionEntryBuilder() = default;

    FunctionEntryBuilder* IsVoid(bool isVoid)
    {
        _props.isVoid = isVoid;
        return this;
    }

    FunctionEntryBuilder* AddParam(ValueType type, int dimension, int size)
    {
        _props.params.emplace_back(type, dimension, size, 1);
        return this;
    }

    FunctionEntryPtr Build()
    {
        auto entry = FunctionEntry::New(_name);
        entry->_props = _props;
        return entry;
    }

private:
    std::string _name;
    FunctionEntryProperty _props;
};

TOMIC_END

#endif // _TOMIC_SYMBOL_TABLE_ENTRY_H_
