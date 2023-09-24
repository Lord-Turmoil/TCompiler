// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_BUFFER_OUTPUT_STREAM_H_
#define _TWIO_BUFFER_OUTPUT_STREAM_H_

#include <twio/Macros.h>
#include <twio/stream/IStream.h>
#include <memory>

TWIO_BEGIN


class BufferOutputStream final : public IOutputStream
{
public:
    explicit BufferOutputStream(size_t size = DEFAULT_BLOCK_SIZE);

    // Copy is prohibited.
    BufferOutputStream(const BufferOutputStream&) = delete;
    BufferOutputStream(BufferOutputStream&&) = delete;
    BufferOutputStream& operator=(const BufferOutputStream&) = delete;
    BufferOutputStream& operator=(BufferOutputStream&&) = delete;

    ~BufferOutputStream() override;

    static std::shared_ptr<BufferOutputStream> New(size_t size = DEFAULT_BLOCK_SIZE);

public:
    void Close() override;
    bool IsReady() const override;

    size_t Write(const char* buffer, size_t size) override;
    size_t Write(const char* buffer) override;
    size_t Write(char ch) override;

    RedirectRequestPtr Yield() override;

private:
    void _EnsureBufferSize(size_t size);

private:
    static const size_t DEFAULT_BLOCK_SIZE;

    std::unique_ptr<char[]> _buffer;
    size_t _size;
    size_t _next;
};

using BufferOutputStreamPtr = std::shared_ptr<BufferOutputStream>;


TWIO_END

#endif // _TWIO_BUFFER_OUTPUT_STREAM_H_
