/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_SYMBOL_TABLE_BLOCK_H_
#define _TOMIC_SYMBOL_TABLE_BLOCK_H_

#include "../../Common.h"
#include "SymbolTableForward.h"
#include <vector>

TOMIC_BEGIN

class SymbolTableBlock
{
public:
    SymbolTableBlock(int id, SymbolTableBlockPtr parent)
            : _id(id), _parent(parent) {}

    static SymbolTableBlockPtr New(int id, SymbolTableBlockPtr parent)
    {
        return std::make_shared<SymbolTableBlock>(id, parent);
    }

    int Id() const { return _id; }
    SymbolTableBlockPtr Parent() const { return _parent; }

    // Warning: Add entry do not check legality.
    SymbolTableBlock* AddEntry(SymbolTableEntryPtr entry);

    // Find entry in this block and its ancestors.
    SymbolTableEntryPtr FindEntry(const std::string& name) const;
    // Find entry in this block only.
    SymbolTableEntryPtr FindLocalEntry(const std::string& name) const;

private:
    int _id;
    SymbolTableBlockPtr _parent;

    std::vector<SymbolTableEntryPtr> _entries;
};

TOMIC_END

#endif // _TOMIC_SYMBOL_TABLE_BLOCK_H_
