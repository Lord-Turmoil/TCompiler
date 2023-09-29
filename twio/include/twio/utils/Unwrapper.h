// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#pragma once

#ifndef _TWIO_UNWRAPPER_H_
#define _TWIO_UNWRAPPER_H_

#include <twio/Common.h>
#include <twio/stream/IStream.h>

TWIO_BEGIN


// A utility class to unwrap stream to raw buffer.
void UnwrapStream(const IOutputStreamPtr& stream, char* buffer);


TWIO_END

#endif // _TWIO_UNWRAPPER_H_
