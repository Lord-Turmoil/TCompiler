// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_BUFFER_INPUT_STREAM_H_
#define _TWIO_BUFFER_INPUT_STREAM_H_

#include <twio/Common.h>
#include <twio/stream/IStream.h>
#include <memory>

TWIO_BEGIN


class BufferInputStream final : public IInputStream
{
public:
    // Initialize via a given buffer with a fixed size.
    // To ensure the buffer is valid and immutable, a copy of the original
    // buffer is made.
    explicit BufferInputStream(const char* buffer, size_t size);

    // Initialize via a given buffer, which must be a null-terminated string.
    explicit BufferInputStream(const char* buffer);

    // Initialize via a redirect request, which must be a buffer request for now.
    explicit BufferInputStream(RedirectRequestPtr request);

    BufferInputStream();

    // Copy is prohibited.
    BufferInputStream(const BufferInputStream&) = delete;
    BufferInputStream(BufferInputStream&& other) noexcept;
    BufferInputStream& operator=(const BufferInputStream&) = delete;
    BufferInputStream& operator=(BufferInputStream&& other) noexcept;

    ~BufferInputStream() override;

    static std::shared_ptr<BufferInputStream> New(const char* buffer, size_t size);
    static std::shared_ptr<BufferInputStream> New(const char* buffer);
    static std::shared_ptr<BufferInputStream> New(RedirectRequestPtr request);

public:
    // Close the buffer input means free the memory.
    void Close() override;

    // Check if the buffer is ready for reading.
    bool IsReady() const override;

    size_t Read(char* buffer, size_t size) override;
    size_t Read(char* buffer) override;
    int Read() override;

    void Accept(RedirectRequestPtr request) override;

private:
    std::unique_ptr<char[]> _buffer;
    size_t _size;
    size_t _next;
};

using BufferInputStreamPtr = std::shared_ptr<BufferInputStream>;


TWIO_END

#endif // _TWIO_BUFFER_INPUT_STREAM_H_
