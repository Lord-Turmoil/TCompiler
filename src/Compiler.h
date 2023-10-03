/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include "../3rd-party/twio/include/twio.h"
#include "../3rd-party/mioc/include/mioc.h"

void RegisterComponents();

void Compile(twio::IReaderPtr srcReader, twio::IWriterPtr dstWriter);

#endif
