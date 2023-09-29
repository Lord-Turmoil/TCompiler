// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#ifndef _TWIO_READER_H_
#define _TWIO_READER_H_

#include <twio/core/IReader.h>
#include <twio/core/ReaderBuffer.h>
#include <twio/stream/IStream.h>

#include <memory>

TWIO_BEGIN

class Reader final : public IReader, public ReaderBuffer
{
public:
    explicit Reader(IInputStreamPtr stream);

    ~Reader() override;

    static std::shared_ptr<Reader> New(const IInputStreamPtr& stream);

    size_t Read(char* buffer, size_t size) override;
    int Read() override;

    int Rewind() override;

    IInputStreamPtr Stream() const override;

    void Close() override;

private:
    IInputStreamPtr _stream;
};

using ReaderPtr = std::shared_ptr<Reader>;


TWIO_END

#endif // _TWIO_READER_H_
