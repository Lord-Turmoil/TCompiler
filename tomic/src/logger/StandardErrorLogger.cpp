/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../../include/tomic/logger/impl/StandardErrorLogger.h"
#include <string>

TOMIC_BEGIN

// Using PIMPL idiom to hide the implementation details
class StandardErrorLogger::StandardErrorEntry
{
public:
    StandardErrorEntry(int line, int column, const char* type, const char* msg)
            : _line(line), _column(column)
    {
        if (type != nullptr)
        {
            _type = type;
        }
        if (msg != nullptr)
        {
            _msg = msg;
        }
    }

    int _line;
    int _column;
    std::string _type;
    std::string _msg;
};

void StandardErrorLogger::Log(int line, int column, const char* type, const char* msg)
{
    _entries.emplace_back(new StandardErrorEntry(line, column, type, msg));
}

void StandardErrorLogger::Dumps(twio::IWriterPtr writer)
{
    for (auto& entry: _entries)
    {
        writer->WriteFormat("%d %s\n", entry->_line, entry->_type.c_str());
    }
}

TOMIC_END
