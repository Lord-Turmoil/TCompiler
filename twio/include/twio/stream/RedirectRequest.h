// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_REDIRECT_REQUEST_H_
#define _TWIO_REDIRECT_REQUEST_H_

#include <twio/Macros.h>
#include <memory>

TWIO_BEGIN


enum class RedirectProtocol
{
    SRP_NONE = 0,
    SRP_BUFFER,
    SRP_FILE
};

struct RedirectRequest
{
    RedirectProtocol protocol;
    std::unique_ptr<char[]> buffer;

    RedirectRequest(RedirectProtocol protocol, std::unique_ptr<char[]> buffer)
        : protocol(protocol), buffer(std::move(buffer))
    {
    }

    static std::shared_ptr<RedirectRequest> New(RedirectProtocol protocol, std::unique_ptr<char[]> buffer)
    {
        return std::make_shared<RedirectRequest>(protocol, std::move(buffer));
    }

    RedirectRequest(RedirectRequest& other) = delete;

    RedirectRequest(RedirectRequest&& other) noexcept
    {
        protocol = other.protocol;
        buffer = std::move(other.buffer);
    }

    RedirectRequest& operator=(RedirectRequest& other) = delete;

    RedirectRequest& operator=(RedirectRequest&& other) noexcept
    {
        // Avoid self assignment!
        if (this != &other)
        {
            protocol = other.protocol;
            // Old data will be freed automatically.
            buffer = std::move(other.buffer);
        }
        return *this;
    }

    ~RedirectRequest() = default;
};

using RedirectRequestPtr = std::shared_ptr<RedirectRequest>;

TWIO_END

#endif // _TWIO_REDIRECT_REQUEST_H_
