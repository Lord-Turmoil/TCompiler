/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYMBOL_TABLE_ENTRY_H_
#define _TOMIC_SYMBOL_TABLE_ENTRY_H_

#include <tomic/Shared.h>

#include <string>
#include <utility>
#include <vector>

TOMIC_BEGIN

enum class SymbolTableEntryType
{
    ET_UNKNOWN,
    ET_VARIABLE,
    ET_CONSTANT,
    ET_FUNCTION,

    ET_COUNT
};


enum class SymbolValueType
{
    VT_ANY,
    VT_VOID,
    VT_INT,
    VT_CHAR,
    VT_BOOL,
    VT_ARRAY,

    VT_COUNT
};


class SymbolTableEntry
{
public:
    virtual ~SymbolTableEntry() = default;

    SymbolTableEntryType EntryType() const { return _type; }
    const char* Name() const { return _name.c_str(); }

    // Use with caution! Do it only when name collision occurs.
    void AlterName(std::string name) { _name = std::move(name); }

protected:
    // So it cannot be instantiated directly.
    SymbolTableEntry(const std::string& name, SymbolTableEntryType type)
        : _type(type), _name(name)
    {
    }


    SymbolTableEntryType _type;
    std::string _name;
};


/*
 * ==================== Symbol Table Variable Entry ====================
 */
const int MAX_ARRAY_DIMENSION = 2;


struct VariableEntryProperty
{
    SymbolValueType type;

    // a[10][100] -> size = { 10, 100 }
    int dimension;
    int size[MAX_ARRAY_DIMENSION];


    VariableEntryProperty()
        : type(SymbolValueType::VT_INT), dimension(0), size { 0 }
    {
    }
};


class VariableEntry final : public SymbolTableEntry
{
    friend class VariableEntryBuilder;

public:
    ~VariableEntry() override = default;

public:
    SymbolValueType Type() const { return _props.type; }

    int Dimension() const { return _props.dimension; }


    int ArraySize(int dimension) const
    {
        TOMIC_ASSERT(dimension < MAX_ARRAY_DIMENSION);
        return _props.size[dimension];
    }

private:
    VariableEntry(const std::string& name)
        : SymbolTableEntry(name, SymbolTableEntryType::ET_VARIABLE)
    {
    }


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
    VariableEntryBuilder(const std::string& name) : _name(name)
    {
    }


    ~VariableEntryBuilder() = default;


    VariableEntryBuilder* Type(SymbolValueType type)
    {
        _props.type = type;
        return this;
    }


    VariableEntryBuilder* Size(int n)
    {
        _props.dimension = 1;
        _props.size[0] = n;
        return this;
    }


    VariableEntryBuilder* Size(int n, int m)
    {
        _props.dimension = 2;
        _props.size[0] = n;
        _props.size[1] = m;
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
 * ==================== Symbol Table Constant Entry ====================
 */

struct ConstantEntryProperty
{
    SymbolValueType type;

    int dimension;
    int size[MAX_ARRAY_DIMENSION];
    int value;

    std::vector<std::vector<int>> values;


    ConstantEntryProperty()
        : type(SymbolValueType::VT_INT), dimension(0), size { 0 }
    {
    }
};


class ConstantEntry final : public SymbolTableEntry
{
    friend class ConstantEntryBuilder;

public:
    ~ConstantEntry() override = default;

    SymbolValueType Type() const { return _props.type; }

    int Dimension() const { return _props.dimension; }

    int ArraySize(int dimension) const { return _props.size[dimension]; }

    int Value() const { return _props.value; }
    int Value(int index) const { return _props.values[0][index]; }
    int Value(int index1, int index2) const { return _props.values[index1][index2]; }

private:
    ConstantEntry(const std::string& name)
        : SymbolTableEntry(name, SymbolTableEntryType::ET_CONSTANT)
    {
    }


    static std::shared_ptr<ConstantEntry> New(const std::string& name)
    {
        return std::shared_ptr<ConstantEntry>(new ConstantEntry(name));
    }


    ConstantEntryProperty _props;
};


using ConstantEntryPtr = std::shared_ptr<ConstantEntry>;


class ConstantEntryBuilder
{
public:
    ConstantEntryBuilder(const std::string& name) : _name(name)
    {
    }


    ~ConstantEntryBuilder() = default;


    ConstantEntryBuilder* Type(SymbolValueType type)
    {
        _props.type = type;
        return this;
    }


    ConstantEntryBuilder* Size(int n)
    {
        _props.dimension = 1;
        _props.size[0] = n;
        _props.values.resize(1, std::vector<int>(n, 0));
        return this;
    }


    // a[n][m]
    ConstantEntryBuilder* Size(int n, int m)
    {
        _props.dimension = 2;
        _props.size[0] = n;
        _props.size[1] = m;
        _props.values.resize(n, std::vector<int>(m, 0));
        return this;
    }


    ConstantEntryBuilder* Value(int value)
    {
        _props.value = value;
        return this;
    }


    ConstantEntryBuilder* Value(int index, int value)
    {
        _props.values[0][index] = value;
        return this;
    }


    ConstantEntryBuilder* Value(int index1, int index2, int value)
    {
        _props.values[index1][index2] = value;
        return this;
    }


    ConstantEntryBuilder* Values(const std::vector<std::vector<int>>& values)
    {
        // May disrupt array size!
        _props.values = values;
        return this;
    }


    ConstantEntryPtr Build()
    {
        auto entry = ConstantEntry::New(_name);
        // A copy is occurred here.
        entry->_props = _props;
        return entry;
    }

private:
    std::string _name;
    ConstantEntryProperty _props;
};


/*
 * ==================== Symbol Table Function Entry ====================
 */

struct FunctionParamProperty
{
    SymbolValueType type;
    std::string name;   // for original name
    int dimension;
    int size[MAX_ARRAY_DIMENSION];


    FunctionParamProperty(SymbolValueType _type, std::string _name, int _dimension, int _size1, int _size2)
        : type(_type), name(_name), dimension(_dimension), size { _size1, _size2 }
    {
        TOMIC_ASSERT(_dimension >= 0);
        TOMIC_ASSERT(_dimension <= MAX_ARRAY_DIMENSION);
    }
};


using FunctionParamPropertyPtr = FunctionParamProperty*;
using FunctionParamPropertySmartPtr = std::shared_ptr<FunctionParamProperty>;


struct FunctionEntryProperty
{
    SymbolValueType type;
    std::vector<FunctionParamPropertySmartPtr> params;

    int ArgsCount() const { return params.size(); }
};


class FunctionEntry final : public SymbolTableEntry
{
    friend class FunctionEntryBuilder;

public:
    ~FunctionEntry() override = default;

    SymbolValueType Type() const { return _props.type; }

    int ArgsCount() const { return _props.ArgsCount(); }


    FunctionParamPropertyPtr Param(int index) const
    {
        TOMIC_ASSERT(index < ArgsCount());
        return _props.params[index].get();
    }

private:
    FunctionEntry(const std::string& name)
        : SymbolTableEntry(name, SymbolTableEntryType::ET_FUNCTION)
    {
    }


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
    FunctionEntryBuilder(const std::string& name) : _name(name)
    {
    }


    ~FunctionEntryBuilder() = default;


    FunctionEntryBuilder* Type(SymbolValueType type)
    {
        _props.type = type;
        return this;
    }


    FunctionEntryBuilder* AddParam(SymbolValueType type, std::string name, int dimension, int size)
    {
        _props.params.emplace_back(std::make_shared<FunctionParamProperty>(type, name, dimension, 0, size));
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
