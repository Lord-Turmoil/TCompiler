// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_ADVANCED_READER_H_
#define _TWIO_ADVANCED_READER_H_

#include <twio/core/IReader.h>
#include <twio/core/ReaderBuffer.h>
#include <twio/stream/IStream.h>
#include <memory>
#include <stack>

TWIO_BEGIN


class AdvancedReader final : public IAdvancedReader, public ReaderBuffer
{
public:
    explicit AdvancedReader(IInputStreamPtr stream);
    ~AdvancedReader() override;

    static std::shared_ptr<AdvancedReader> New(const IInputStreamPtr& stream);

    size_t Read(char* buffer, size_t size) override;
    int Read() override;

    int Rewind() override;

    int Line() const override;
    int Char() const override;

    IInputStreamPtr Stream() const override;

    void Close() override;

private:
    void _MoveForward(char ch);
    void _MoveBackward(char ch);

    IInputStreamPtr _stream;

    std::stack<int> _lastChar;

    int _lineNo;
    int _charNo;
};

using AdvancedReaderPtr = std::shared_ptr<AdvancedReader>;


TWIO_END

#endif // _TWIO_ADVANCED_READER_H_
