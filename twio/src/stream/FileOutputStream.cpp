// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#include <twio/stream/FileOutputStream.h>
#include <twio/utils/FileUtil.h>
#include <cstdio>   // EOF

TWIO_BEGIN


FileOutputStream::FileOutputStream(FILE* fp, bool takeOver)
    : _fp(fp), _takeOver(takeOver)
{
    TWIO_ASSERT(_fp);
    // TWIO_ASSERT(IsWriteOnly(_fp));
}

FileOutputStream::FileOutputStream(const char* path)
{
    _fp = OpenFile(path, "w");

    // _fp must not be null
    TWIO_ASSERT(_fp);

    _takeOver = true;
}

std::shared_ptr<FileOutputStream> New(FILE* fp, bool takeOver)
{
    return std::make_shared<FileOutputStream>(fp, takeOver);
}

std::shared_ptr<FileOutputStream> FileOutputStream::New(FILE* fp, bool takeOver)
{
    return std::make_shared<FileOutputStream>(fp, takeOver);
}

std::shared_ptr<FileOutputStream> FileOutputStream::New(const char* path)
{
    return std::make_shared<FileOutputStream>(path);
}

FileOutputStream::~FileOutputStream()
{
    Close();
}

void FileOutputStream::Close()
{
    if (_fp)
    {
        if (_takeOver)
        {
            CloseFile(_fp);
        }
        _fp = nullptr;
    }
}

bool FileOutputStream::IsReady() const
{
    return _fp != nullptr;
}

size_t FileOutputStream::Write(const char* buffer, size_t size)
{
    TWIO_ASSERT(IsReady());
    return fwrite(buffer, sizeof(char), size, _fp);
}

size_t FileOutputStream::Write(const char* buffer)
{
    TWIO_ASSERT(IsReady());

    const size_t size = strlen(buffer);

    return Write(buffer, size);
}

size_t FileOutputStream::Write(char ch)
{
    TWIO_ASSERT(IsReady());

    const int ret = fputc(ch, _fp);

    return (ret == EOF) ? 0 : 1;
}

TWIO_END
