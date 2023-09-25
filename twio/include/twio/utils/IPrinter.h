// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_IPRINTER_H_
#define _TWIO_IPRINTER_H_

#include <memory>

TWIO_BEGIN


class IPrinter
{
public:
    virtual ~IPrinter() = 0;

    virtual void Print() = 0;

    virtual bool IsReady() const = 0;
};

inline IPrinter::~IPrinter() = default;

using IPrinterPtr = std::shared_ptr<IPrinter>;

TWIO_END

#endif // _TWIO_IPRINTER_H_
