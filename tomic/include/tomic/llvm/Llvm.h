/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_H_
#define _TOMIC_LLVM_H_

#include <tomic/Shared.h>

#define LLVM_VERSION_MAJOR "0"
#define LLVM_VERSION_MINOR "1"
#define LLVM_VERSION_PATCH "1"

#define LLVM_VERSION (LLVM_VERSION_MAJOR "." LLVM_VERSION_MINOR "." LLVM_VERSION_PATCH)

#define LLVM_BEGIN namespace llvm {
#define LLVM_END   }

#define TOMIC_LLVM_BEGIN TOMIC_BEGIN LLVM_BEGIN
#define TOMIC_LLVM_END   LLVM_END   TOMIC_END

#endif // _TOMIC_LLVM_H_
