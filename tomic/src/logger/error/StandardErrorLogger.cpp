/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/logger/error/impl/StandardErrorLogger.h"
#include <string>

TOMIC_BEGIN

// Using PIMPL idiom to hide the implementation details
class StandardErrorLogger::StandardErrorEntry
{
public:
    StandardErrorEntry(int line, ErrorType type)
            : _line(line), _type(type)
    {
    }

    int _line;
    ErrorType _type;
};

StandardErrorLogger::StandardErrorLogger(IErrorMapperPtr mapper)
        : _mapper(mapper)
{
    TOMIC_ASSERT(_mapper);
}

void StandardErrorLogger::Log(int line, int column, ErrorType type, const char* msg)
{
    _entries.emplace_back(new StandardErrorEntry(line, type));
}

void StandardErrorLogger::Dumps(twio::IWriterPtr writer)
{
    for (auto& entry: _entries)
    {
        writer->WriteFormat("%d %s\n", entry->_line, _mapper->Description(entry->_type));
    }
}

TOMIC_END
