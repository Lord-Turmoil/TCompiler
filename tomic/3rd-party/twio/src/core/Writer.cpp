// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#include <twio/core/Writer.h>
#include <cstdarg>

TWIO_BEGIN

Writer::Writer(IOutputStreamPtr stream) : _stream(std::move(stream))
{
    TWIO_ASSERT(_stream != nullptr);
}


Writer::~Writer() = default;


std::shared_ptr<Writer> Writer::New(const IOutputStreamPtr& stream)
{
    return std::make_shared<Writer>(stream);
}


size_t Writer::Write(const char* buffer, size_t size)
{
    return _stream->Write(buffer, size);
}


size_t Writer::Write(const char* buffer)
{
    return _stream->Write(buffer);
}


size_t Writer::Write(char ch)
{
    return _stream->Write(ch);
}


size_t Writer::WriteFormat(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    size_t result = WriteVFormat(format, args);
    va_end(args);

    return result;
}

size_t Writer::WriteVFormat(const char* format, va_list args)
{
    return _stream->Write(format, args);
}

IOutputStreamPtr Writer::Stream() const
{
    return _stream;
}


void Writer::Close()
{
    if (_stream)
    {
        _stream->Close();
    }
}


TWIO_END
