/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../../include/tomic/logger/error/impl/DefaultErrorLogger.h"
#include <string>
#include <algorithm>

TOMIC_BEGIN

class DefaultErrorLogger::DefaultErrorEntry
{
public:
    DefaultErrorEntry(int line, int column, ErrorType type, const char* msg)
            : _line(line), _column(column), _type(type)
    {
        if (msg)
        {
            _msg = msg;
        }
    }

    int _line;
    int _column;
    ErrorType _type;
    std::string _msg;
};

using DefaultErrorEntryPtr = std::unique_ptr<DefaultErrorLogger::DefaultErrorEntry>;

class CompareDefaultErrorEntry
{
public:
    bool operator()(const DefaultErrorEntryPtr& lhs, const DefaultErrorEntryPtr& rhs)
    {
        if (lhs->_line != rhs->_line)
        {
            return lhs->_line < rhs->_line;
        }
        else if (lhs->_column != rhs->_column)
        {
            return lhs->_column < rhs->_column;
        }
        else
        {
            return lhs->_type < rhs->_type;
        }
    }
};

DefaultErrorLogger::DefaultErrorLogger(IErrorMapperPtr mapper)
        : _mapper(mapper)
{
}

void DefaultErrorLogger::Log(int line, int column, ErrorType type, const char* msg)
{
    _entries.push_back(std::make_unique<DefaultErrorEntry>(line, column, type, msg));
}

void DefaultErrorLogger::LogFormat(int line, int column, ErrorType type, const char* format, ...)
{
    static char buffer[1024];

    if (format)
    {
        va_list args;
        va_start(args, format);
        vsprintf(buffer, format, args);
        va_end(args);
    }
    else
    {
        buffer[0] = '\0';
    }

    _entries.push_back(std::make_unique<DefaultErrorEntry>(line, column, type, buffer));
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
