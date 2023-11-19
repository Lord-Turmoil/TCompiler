/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_SLOT_TRACKER_H_
#define _TOMIC_LLVM_SLOT_TRACKER_H_

#include <tomic/llvm/ir/IrForward.h>
#include <tomic/llvm/Llvm.h>
#include <unordered_map>

TOMIC_LLVM_BEGIN

/*
 * This is used to track all IDs of LLVM value. I think this name
 * is quite vivid, one value one slot. :)
 *
 * And, for now, does not support derivation.
 */
class SlotTracker final
{
public:
    SlotTracker() = default;
    ~SlotTracker() = default;

    // Trace all values in a function.
    void Trace(FunctionPtr function);

    // Resolve
    int Slot(ValuePtr value);

private:
    std::unordered_map<ValuePtr, int> _slot;
};


using SlotTrackerPtr = SlotTracker*;

TOMIC_LLVM_END

#endif // _TOMIC_LLVM_SLOT_TRACKER_H_
