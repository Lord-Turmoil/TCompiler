// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#define _CRT_SECURE_NO_WARNINGS

#include <twio/utils/FileUtil.h>

#if _TWIO_FOR_WIN32
// ... To be continued... :(
#else
#include <unistd.h>
#include <fcntl.h>
#endif

TWIO_BEGIN

FILE* OpenFile(const char* path, const char* mode)
{
    FILE* fp = fopen(path, mode);

    return fp;
}


void CloseFile(FILE* fp)
{
    if (fp != nullptr)
    {
        TWIO_ASSERT(fclose(fp) == 0);
    }
}

#if _TWIO_FOR_WIN32
bool IsReadOnly(FILE* fp)
{
    // :(
    return true;
}
#else
bool IsReadOnly(FILE* fp)
{
    int fd = fileno(fp);
    int mode = fcntl(fd, F_GETFL);

    return mode == 32768;
}
#endif

#if _TWIO_FOR_WIN32
bool IsWriteOnly(FILE* fp)
{
    // :(
    return true;
}
#else
bool IsWriteOnly(FILE* fp)
{
    int fd = fileno(fp);
    int mode = fcntl(fd, F_GETFL);

    return mode == 32769;
}
#endif


TWIO_END
