// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#define _CRT_SECURE_NO_WARNINGS

#include <twio/utils/Unwrapper.h>
#include <cstring>

TWIO_BEGIN


std::unique_ptr<char[]> UnwrapStream(const IOutputStreamPtr& stream)
{
    TWIO_ASSERT(stream);

    const auto request = stream->Yield();
    if (request->protocol != RedirectProtocol::SRP_BUFFER)
    {
        TWIO_PANIC("UnwrapStream only accept buffer request.");
    }

    // Release old buffer.
    request->protocol = RedirectProtocol::SRP_NONE;

    return std::move(request->buffer);
}


TWIO_END
