/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/logger/error/impl/DefaultErrorLogger.h"

TOMIC_BEGIN

class DefaultErrorLogger::DefaultErrorEntry
{
public:
    DefaultErrorEntry(int line, int column, ErrorType type, const char* msg)
            : _line(line), _column(column), _type(type), _msg(msg)
    {
    }

    int _line;
    int _column;
    ErrorType _type;
    const char* _msg;
};

DefaultErrorLogger::DefaultErrorLogger(IErrorMapperPtr mapper)
        : _mapper(mapper)
{
}

void DefaultErrorLogger::Log(int line, int column, ErrorType type, const char* msg)
{
    _entries.push_back(std::make_unique<DefaultErrorEntry>(line, column, type, msg));
}

void DefaultErrorLogger::Dumps(twio::IWriterPtr writer)
{
    for (auto& entry: _entries)
    {
        writer->WriteFormat("Line %d, Column %d: %s\n",
                            entry->_line,
                            entry->_column,
                            _mapper->Description(entry->_type));
        writer->WriteFormat("    %s\n", entry->_msg);
    }
}

TOMIC_END
