/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/logger/error/impl/VerboseErrorLogger.h>
#include <string>
#include <algorithm>

TOMIC_BEGIN

class CompareDefaultErrorEntry
{
public:
    bool operator()(const VerboseErrorEntry& lhs, const VerboseErrorEntry& rhs)
    {
        if (lhs._line != rhs._line)
        {
            return lhs._line < rhs._line;
        }
        else if (lhs._column != rhs._column)
        {
            return lhs._column < rhs._column;
        }
        else
        {
            return lhs._type < rhs._type;
        }
    }
};

bool DefaultErrorEntryPred(const VerboseErrorEntry& lhs, const VerboseErrorEntry& rhs)
{
    if (lhs._line == rhs._line && lhs._column == rhs._column && lhs._type == rhs._type)
    {
        return lhs._msg == rhs._msg;
    }

    return false;
}

VerboseErrorLogger::VerboseErrorLogger(IErrorMapperPtr mapper)
        : _mapper(mapper)
{
}

void VerboseErrorLogger::LogFormat(int line, int column, ErrorType type, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    LogVFormat(line, column, type, format, args);
    va_end(args);
}

void VerboseErrorLogger::LogVFormat(int line, int column, ErrorType type, const char* format, va_list args)
{
    static char buffer[1024];

    if (format)
    {
        vsprintf(buffer, format, args);
    }
    else
    {
        buffer[0] = '\0';
    }

    _entries.emplace_back(VerboseErrorEntry(line, column, type, buffer));
}

void VerboseErrorLogger::Dumps(twio::IWriterPtr writer)
{
    // It may be sorted many times, but it doesn't matter. :)
    std::sort(_entries.begin(), _entries.end(), CompareDefaultErrorEntry());
    auto last = std::unique(_entries.begin(), _entries.end(), DefaultErrorEntryPred);

    for (auto entry = _entries.begin(); entry != last; entry++)
    {
        writer->WriteFormat("Line %d, Column %d: %s\n",
                            entry->_line,
                            entry->_column,
                            _mapper->Description(entry->_type));
        writer->WriteFormat("    %s\n", entry->_msg.c_str());
    }
}

int VerboseErrorLogger::Count()
{
    return _entries.size();
}

TOMIC_END
