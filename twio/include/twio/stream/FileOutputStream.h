// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_FILE_OUTPUT_STREAM_H_
#define _TWIO_FILE_OUTPUT_STREAM_H_

#include <twio/Macros.h>
#include <twio/stream/IStream.h>
#include <cstdio>
#include <memory>

TWIO_BEGIN

class FileOutputStream final : public IOutputStream
{
public:
    // Initialize via an open file, by default will take over the
    // close operation, thus the client won't bother close it.
    // BE CAREFUL when you set takeOver to false, you must close it
    // by your self then, and should not be closed before write complete.
    explicit FileOutputStream(FILE* fp, bool takeOver = true);

    // Initialize via a file path, will close it automatically.
    explicit FileOutputStream(const char* path);

    // Copy is prohibited.
    FileOutputStream(const FileOutputStream&) = delete;
    FileOutputStream(FileOutputStream&&) = delete;
    FileOutputStream& operator=(const FileOutputStream&) = delete;
    FileOutputStream& operator=(FileOutputStream&&) = delete;

    // Ensure the file is closed.
    ~FileOutputStream() override;

    static std::shared_ptr<FileOutputStream> New(FILE* fp, bool takeOver = true);
    static std::shared_ptr<FileOutputStream> New(const char* path);

public:
    // Manually close the file.
    void Close() override;

    // Check if the file is ready for reading.
    bool IsReady() const override;

    size_t Write(const char* buffer, size_t size) override;
    size_t Write(const char* buffer) override;
    size_t Write(char ch) override;

private:
    FILE* _fp;
    bool _takeOver;
};

using FileOutputStreamPtr = std::shared_ptr<FileOutputStream>;

TWIO_END

#endif // _TWIO_FILE_OUTPUT_STREAM_H_
