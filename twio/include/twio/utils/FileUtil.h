// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_FILEUTIL_H_
#define _TWIO_FILEUTIL_H_

#include <twio/Macros.h>
#include <cstdio>

TWIO_BEGIN


FILE* OpenFile(const char* path, const char* mode);
void CloseFile(FILE* fp);

// Check file access control.
bool IsReadOnly(FILE* fp);
bool IsWriteOnly(FILE* fp);


TWIO_END

#endif // _TWIO_FILEUTIL_H_
