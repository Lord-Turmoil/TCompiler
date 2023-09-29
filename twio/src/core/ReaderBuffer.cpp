// Copyright (C) 2018 - 2023 Tony's Studio. All rights reserved.

#include <twio/core/ReaderBuffer.h>

TWIO_BEGIN


ReaderBuffer::ReaderBuffer()
    : _front(0), _back(0), _current(0)
{
}

ReaderBuffer::~ReaderBuffer() = default;

bool ReaderBuffer::_IsEmpty() const
{
    return _front == _current;
}

bool ReaderBuffer::_HasNext() const
{
    return _current < _back;
}

void ReaderBuffer::_Push(char ch)
{
    _buffer[_SCALE_TO_READER_BUFFER_SIZE(_back)] = ch;
    _current = ++_back;

    // Avoid overflow.
    if (_back - _front > READER_BUFFER_SIZE)
    {
        _front = _back - READER_BUFFER_SIZE;
    }
}

void ReaderBuffer::_Push(const char* buffer, size_t size)
{
    for (auto i = 0; i < size; i++)
    {
        _Push(*(buffer++));
    }
}

// Actually, pop shouldn't return anything, as its return value will be
// the same as a immediate _Get() call.
int ReaderBuffer::_Pop()
{
    if (_IsEmpty())
    {
        return -1;
    }

    _current--;
    return _buffer[_SCALE_TO_READER_BUFFER_SIZE(_current)];
}

int ReaderBuffer::_Get()
{
    if (!_HasNext())
    {
        return -1;
    }

    int ret = _buffer[_SCALE_TO_READER_BUFFER_SIZE(_current)];
    _current++;
    return ret;
}

TWIO_END
