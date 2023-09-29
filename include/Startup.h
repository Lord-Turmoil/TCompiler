/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_STARTUP_H_
#define _TOMIC_STARTUP_H_

#include "../twio/include/twio.h"

void RegisterComponents();

void Compile(twio::IReaderPtr srcReader, twio::IWriterPtr dstWriter);

#endif
