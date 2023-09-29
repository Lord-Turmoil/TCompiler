// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#define _CRT_SECURE_NO_WARNINGS

#include <twio/stream/BufferOutputStream.h>
#include <cstring>

TWIO_BEGIN


const size_t BufferOutputStream::DEFAULT_BLOCK_SIZE = 1024;

BufferOutputStream::BufferOutputStream(size_t size)
{
    _size = TWIO_ALIGN(size, DEFAULT_BLOCK_SIZE);
    _buffer = std::make_unique<char[]>(_size);
    _next = 0;
}

std::shared_ptr<BufferOutputStream> BufferOutputStream::New(size_t size)
{
    return std::make_shared<BufferOutputStream>(size);
}

BufferOutputStream::~BufferOutputStream()
{
    Close();
}

void BufferOutputStream::Close()
{
    if (_buffer)
    {
        _buffer.reset();
        _size = 0;
        _next = 0;
    }
}

bool BufferOutputStream::IsReady() const
{
    return _buffer != nullptr;
}

size_t BufferOutputStream::Write(const char* buffer, size_t size)
{
    TWIO_ASSERT(IsReady());

    const size_t newSize = _next + size;
    _EnsureBufferSize(newSize);

    memcpy(_buffer.get() + _next, buffer, size);
    _next += size;

    return size;
}

size_t BufferOutputStream::Write(const char* buffer)
{
    TWIO_ASSERT(IsReady());

    const size_t size = strlen(buffer);

    return Write(buffer, size);
}

size_t BufferOutputStream::Write(char ch)
{
    TWIO_ASSERT(IsReady());

    _EnsureBufferSize(_next + 1);
    _buffer[_next++] = ch;

    return 1;
}

size_t BufferOutputStream::Write(const char* format, va_list argv)
{
    static char buffer[DEFAULT_BLOCK_SIZE];
    const size_t count = vsprintf(buffer, format, argv);

    if (count > DEFAULT_BLOCK_SIZE)
    {
        TWIO_ASSERT(false && "Buffer overflow!");
    }

    return Write(buffer, count);
}

RedirectRequestPtr BufferOutputStream::Yield()
{
    TWIO_ASSERT(IsReady());

    // Append a null-termination.
    Write('\0');

    return RedirectRequest::New(RedirectProtocol::SRP_BUFFER, std::move(_buffer));
}

void BufferOutputStream::_EnsureBufferSize(size_t size)
{
    if (size > _size)
    {
        const size_t newSize = TWIO_ALIGN(size, DEFAULT_BLOCK_SIZE);
        auto newBuffer = std::make_unique<char[]>(newSize);

        memcpy(newBuffer.get(), _buffer.get(), _size);

        _buffer = std::move(newBuffer);
        _size = newSize;
    }
}


TWIO_END
