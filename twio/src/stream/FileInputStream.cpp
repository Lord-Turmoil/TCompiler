// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#include <twio/stream/FileInputStream.h>
#include <twio/utils/FileUtil.h>
#include <cstdio>   // EOF

TWIO_BEGIN


FileInputStream::FileInputStream(FILE* fp, bool takeOver)
    : _fp(fp), _takeOver(takeOver)
{
    TWIO_ASSERT(_fp);
    // TWIO_ASSERT(IsReadOnly(_fp));
}

FileInputStream::FileInputStream(const char* path)
{
    _fp = OpenFile(path, "r");

    // _fp must not be null
    TWIO_ASSERT(_fp);

    _takeOver = true;
}

FileInputStream::FileInputStream(FileInputStream&& other) noexcept
{
    _fp = other._fp;
    _takeOver = other._takeOver;

    other._fp = nullptr;
}

FileInputStream& FileInputStream::operator=(FileInputStream&& other) noexcept
{
    if (this != &other)
    {
        Close();

        _fp = other._fp;
        _takeOver = other._takeOver;

        other._fp = nullptr;
    }

    return *this;
}

std::shared_ptr<FileInputStream> FileInputStream::New(FILE* fp, bool takeOver)
{
    return std::make_shared<FileInputStream>(fp, takeOver);
}

std::shared_ptr<FileInputStream> FileInputStream::New(const char* path)
{
    return std::make_shared<FileInputStream>(path);
}

FileInputStream::~FileInputStream()
{
    Close();
}

void FileInputStream::Close()
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

bool FileInputStream::IsReady() const
{
    return _fp != nullptr;
}

size_t FileInputStream::Read(char* buffer, size_t size)
{
    TWIO_ASSERT(IsReady());
    const size_t count = fread(buffer, sizeof(char), size, _fp);
    buffer[count] = '\0';
    return count;
}

size_t FileInputStream::Read(char* buffer)
{
    TWIO_ASSERT(IsReady());

    const int ch = fgetc(_fp);
    if (ch == EOF)
    {
        if (*buffer)
        {
            *buffer = '\0';
        }
        return 0;
    }
    if (*buffer)
    {
        *buffer = static_cast<char>(ch);
    }

    return 1;
}

int FileInputStream::Read()
{
    TWIO_ASSERT(IsReady());

    return fgetc(_fp);
}

TWIO_END
