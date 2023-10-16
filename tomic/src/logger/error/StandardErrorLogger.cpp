/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/logger/error/impl/StandardErrorLogger.h>
#include <algorithm>
#include <string>

TOMIC_BEGIN

class CompareStandardErrorEntry
{
public:
    bool operator()(const StandardErrorEntry& lhs, const StandardErrorEntry& rhs)
    {
        if (lhs._line != rhs._line)
        {
            return lhs._line < rhs._line;
        }
        else
        {
            return lhs._type < rhs._type;
        }
    }
};

bool StandardErrorEntryPred(const StandardErrorEntry& lhs, const StandardErrorEntry& rhs)
{
    return lhs._line == rhs._line && lhs._type == rhs._type;
}

StandardErrorLogger::StandardErrorLogger(IErrorMapperPtr mapper)
        : _mapper(mapper)
{
    TOMIC_ASSERT(_mapper);
}

void StandardErrorLogger::LogFormat(int line, int column, ErrorType type, const char* format, ...)
{
    _entries.emplace_back(line, type);
}

void StandardErrorLogger::LogVFormat(int line, int column, ErrorType type, const char* format, va_list args)
{
    _entries.emplace_back(line, type);
}

void StandardErrorLogger::Dumps(twio::IWriterPtr writer)
{
    std::sort(_entries.begin(), _entries.end(), CompareStandardErrorEntry());
    // filter out duplicate entries
    auto last = std::unique(_entries.begin(), _entries.end(), StandardErrorEntryPred);

    for (auto entry = _entries.begin(); entry != last; entry++)
    {
        if (entry->_type == ErrorType::ERR_UNKNOWN)
        {
            continue;
        }
        writer->WriteFormat("%d %s\n", entry->_line, _mapper->Description(entry->_type));
    }
}

int StandardErrorLogger::Count()
{
    return _entries.size();
}

TOMIC_END
