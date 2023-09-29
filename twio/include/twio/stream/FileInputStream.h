// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIOC_FILE_INPUT_STREAM_H_
#define _TWIOC_FILE_INPUT_STREAM_H_

#include <twio/stream/IStream.h>
#include <cstdio>
#include <memory>

TWIO_BEGIN


class FileInputStream final : public IInputStream
{
public:
    // Initialize via an open file, by default will take over the
    // close operation, thus the client won't bother close it.
    // BE CAREFUL when you set takeOver to false, you must close it
    // by your self then, and should not be closed before read complete.
    explicit FileInputStream(FILE* fp, bool takeOver = true);

    // Initialize via a file path, will close it automatically.
    explicit FileInputStream(const char* path);

    // Copy is prohibited.
    FileInputStream(const FileInputStream&) = delete;
    FileInputStream(FileInputStream&& other) noexcept;
    FileInputStream& operator=(const FileInputStream&) = delete;
    FileInputStream& operator=(FileInputStream&& other) noexcept;

    // Ensure the file is closed.
    ~FileInputStream() override;

    static std::shared_ptr<FileInputStream> New(FILE* fp, bool takeOver = true);
    static std::shared_ptr<FileInputStream> New(const char* path);

public:
    // Manually close the file.
    void Close() override;

    // Check if the file is ready for reading.
    bool IsReady() const override;

    size_t Read(char* buffer, size_t size) override;
    size_t Read(char* buffer) override;
    int Read() override;

private:
    FILE* _fp;
    bool _takeOver;
};

using FileInputStreamPtr = std::shared_ptr<FileInputStream>;


TWIO_END

#endif // _TWIOC_FILE_INPUT_STREAM_H_
