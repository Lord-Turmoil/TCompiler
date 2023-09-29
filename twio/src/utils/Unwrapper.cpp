// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#define _CRT_SECURE_NO_WARNINGS

#include <twio/utils/Unwrapper.h>
#include <cstring>

TWIO_BEGIN


void UnwrapStream(const IOutputStreamPtr& stream, char* buffer)
{
    TWIO_ASSERT(stream);

    const auto request = stream->Yield();
    if (request->protocol != RedirectProtocol::SRP_BUFFER)
    {
        TWIO_PANIC("UnwrapStream only accept buffer request.");
    }

    // TODO: for compiler compatibility, use strcpy instead of strcpy_s.
    strcpy(buffer, request->buffer.get());

    // Release old buffer.
    request->protocol = RedirectProtocol::SRP_NONE;
    request->buffer.reset();
}


TWIO_END
