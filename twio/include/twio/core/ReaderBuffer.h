// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#ifndef _TWIO_READER_BUFFER_H_
#define _TWIO_READER_BUFFER_H_

#include <twio/Macros.h>

TWIO_BEGIN


// Reader will keep last READER_BUFFER_SIZE bytes in memory.
const size_t READER_BUFFER_SIZE = 1024;

#define _SCALE_TO_READER_BUFFER_SIZE(size) ((size) & ((READER_BUFFER_SIZE) - 1))

// Reader buffer is actually a stack.
class ReaderBuffer
{
public:
    ReaderBuffer();
    virtual ~ReaderBuffer() = 0;

protected:
    // Whether buffer is empty or not.
    bool _IsEmpty() const;

    // Whether buffer is rewinded, which means _Pop is called that _current is
    // between _back and _front.
    bool _HasNext() const;

    // Push a character to the end of the buffer. If buffer is full, then _front
    // will be adjusted to keep a maximum `READER_BUFFER_SIZE` characters.
    // WARNING: _current will be rest to _back!
    void _Push(char ch);
    void _Push(const char* buffer, size_t size);

    // Pop will not actually remove element in the buffer, it only moves _current
    // back to get the last input.
    int _Pop();

    // This is used when buffer is in rewinded state. It will get next character
    // based on _current.
    int _Get();

private:
    char _buffer[READER_BUFFER_SIZE];

    // _front <= _current <= _back
    size_t _front;
    size_t _back;
    size_t _current;
};


TWIO_END

#endif // _TWIO_READER_BUFFER_H_
