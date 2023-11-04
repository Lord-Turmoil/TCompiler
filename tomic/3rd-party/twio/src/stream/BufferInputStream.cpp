// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#include <twio/stream/BufferInputStream.h>
#include <twio/stream/RedirectRequest.h>
#include <cstring>
#include <utility>
#include <cstdio>   // EOF

TWIO_BEGIN


BufferInputStream::BufferInputStream(const char* buffer, size_t size)
    : _size(size), _next(0)
{
    TWIO_ASSERT(buffer);

    _buffer = std::make_unique<char[]>(TWIO_ALIGN_8(_size));

    memcpy(_buffer.get(), buffer, size);
}


BufferInputStream::BufferInputStream(const char* buffer) : _next(0)
{
    _size = strlen(buffer);
    _buffer = std::make_unique<char[]>(TWIO_ALIGN_8(_size));

    memcpy(_buffer.get(), buffer, _size);
}


BufferInputStream::BufferInputStream() : _size(0), _next(0)
{
}


BufferInputStream::BufferInputStream(RedirectRequestPtr request)
{
    Accept(std::move(request));
}


BufferInputStream::BufferInputStream(BufferInputStream&& other) noexcept
{
    _buffer = std::move(other._buffer);
    _size = other._size;
    _next = other._next;
}


BufferInputStream& BufferInputStream::operator=(BufferInputStream&& other) noexcept
{
    // Avoid self assignment!
    if (this != &other)
    {
        // Old data will be freed automatically.
        _buffer = std::move(other._buffer);
        _size = other._size;
        _next = other._next;
    }
    return *this;
}


std::shared_ptr<BufferInputStream> BufferInputStream::New(const char* buffer, size_t size)
{
    return std::make_shared<BufferInputStream>(buffer, size);
}


std::shared_ptr<BufferInputStream> BufferInputStream::New(const char* buffer)
{
    return std::make_shared<BufferInputStream>(buffer);
}


std::shared_ptr<BufferInputStream> BufferInputStream::New(RedirectRequestPtr request)
{
    return std::make_shared<BufferInputStream>(std::move(request));
}


BufferInputStream::~BufferInputStream()
{
    Close();
}


void BufferInputStream::Close()
{
    // Set buffer to nullptr.
    _buffer.reset();
}


bool BufferInputStream::IsReady() const
{
    return _buffer != nullptr;
}


bool BufferInputStream::HasNext() const
{
    return IsReady() && (_next < _size);
}


size_t BufferInputStream::Read(char* buffer, size_t size)
{
    TWIO_ASSERT(IsReady());

    // adjust size
    size = std::min(size, _size - _next);
    if (size == 0)
    {
        return 0;
    }

    memcpy(buffer, _buffer.get() + _next, size);
    buffer[size] = '\0';

    return size;
}


size_t BufferInputStream::Read(char* buffer)
{
    TWIO_ASSERT(IsReady());

    // buffer empty
    if (_next >= _size)
    {
        *buffer = '\0';
        return 0;
    }

    *buffer = _buffer[_next++];

    return 1;
}


int BufferInputStream::Read()
{
    TWIO_ASSERT(IsReady());

    // buffer empty
    if (_next >= _size)
    {
        return EOF;
    }

    return _buffer[_next++];
}


void BufferInputStream::Accept(RedirectRequestPtr request)
{
    TWIO_ASSERT(request);

    // Currently only support buffer request.
    if (request->protocol != RedirectProtocol::SRP_BUFFER)
    {
        TWIO_PANIC("BufferInputStream only accept buffer request.");
    }

    // old value will be freed automatically.
    _buffer = std::move(request->buffer);
    _size = strlen(_buffer.get());
    _next = 0;
}


TWIO_END
