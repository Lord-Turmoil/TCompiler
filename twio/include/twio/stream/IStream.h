// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_STREAM_H_
#define _TWIO_STREAM_H_

#include <twio/Common.h>
#include <twio/stream/RedirectRequest.h>
#include <memory>
#include <cstdarg>

TWIO_BEGIN


class IStream
{
public:
    virtual ~IStream() = default;

    virtual void Close() = 0;
    virtual bool IsReady() const = 0;
};


class IInputStream : public IStream
{
public:
    ~IInputStream() override = default;

    // Read up to 'size' characters from the stream.
    virtual size_t Read(char* buffer, size_t size) = 0;

    // Read one character from the stream.
    virtual size_t Read(char* buffer) = 0;

    // Read one character from the stream.
    virtual int Read() = 0;

    // Default implementation is not implemented.
    virtual void Accept(RedirectRequestPtr request)
    {
        TWIO_PANIC("Not implemented");
    }
};


class IOutputStream : public IStream
{
public:
    ~IOutputStream() override = default;

    // Write up to 'size' characters from the buffer to the stream.
    virtual size_t Write(const char* buffer, size_t size) = 0;

    // Write a null-terminated string to the stream.
    virtual size_t Write(const char* buffer) = 0;

    // Write one character to the stream.
    virtual size_t Write(char ch) = 0;

    virtual size_t Write(const char* format, va_list argv) = 0;

    // Default implementation is not implemented.
    virtual RedirectRequestPtr Yield()
    {
        TWIO_PANIC("Not implemented");
        return nullptr;
    }
};


using IInputStreamPtr = std::shared_ptr<IInputStream>;
using IOutputStreamPtr = std::shared_ptr<IOutputStream>;

TWIO_END

#endif // _TWIO_STREAM_H_
