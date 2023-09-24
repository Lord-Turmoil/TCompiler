// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_PRINTER_H_
#define _TWIO_PRINTER_H_

#include <twio/Macros.h>
#include <twio/core/IReader.h>
#include <twio/core/IWriter.h>
#include <twio/utils/IPrinter.h>

#include <memory>

TWIO_BEGIN


class Printer final : public IPrinter
{
public:
    Printer(IReaderPtr reader, IWriterPtr writer);
    ~Printer() override = default;

    Printer(const Printer&) = delete;
    Printer(Printer&& other) noexcept;
    Printer& operator=(const Printer&) = delete;
    Printer& operator=(Printer&& other) noexcept;

    static std::shared_ptr<Printer> New(IReaderPtr reader, IWriterPtr writer);

    void Print() override;

    bool IsReady() const override;

private:
    IReaderPtr _reader;
    IWriterPtr _writer;
};

using PrinterPtr = std::shared_ptr<Printer>;

TWIO_END

#endif // _TWIO_PRINTER_H_
