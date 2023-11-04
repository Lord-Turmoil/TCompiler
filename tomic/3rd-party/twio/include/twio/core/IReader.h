// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#ifndef _TWIO_IREADER_H_
#define _TWIO_IREADER_H_

#include <twio/Common.h>
#include <twio/stream/IStream.h>
#include <memory>

TWIO_BEGIN


class IReader
{
public:
    virtual ~IReader() = 0;

    virtual bool HasNext() = 0;

    virtual size_t Read(char* buffer, size_t size) = 0;
    // Readline will discard the '\n' character.
    virtual const char* ReadLine(char* buffer) = 0;
    virtual int Read() = 0;

    virtual int Rewind() = 0;

    // Dangerous method, use with caution.
    virtual IInputStreamPtr Stream() const = 0;

    virtual void Close() = 0;
};


inline IReader::~IReader() = default;

using IReaderPtr = std::shared_ptr<IReader>;


class IAdvancedReader : public IReader
{
public:
    ~IAdvancedReader() override = 0;

    virtual int Line() const = 0;
    virtual int Char() const = 0;
};


inline IAdvancedReader::~IAdvancedReader() = default;

using IAdvancedReaderPtr = std::shared_ptr<IAdvancedReader>;


TWIO_END

#endif // _TWIO_IREADER_H_
