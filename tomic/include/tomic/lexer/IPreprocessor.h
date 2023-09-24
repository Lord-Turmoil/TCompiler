/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_IPREPROCESSOR_H_
#define _TOMIC_IPREPROCESSOR_H_

#include <tomic/Defines.h>
#include <twio.h>

#include <memory>

TOMIC_BEGIN

class IPreprocessor
{
public:
    virtual ~IPreprocessor() = default;

    virtual void Process() = 0;

    virtual IPreprocessor* SetReader(twio::IReaderPtr&& reader) = 0;
    virtual IPreprocessor* SetWriter(twio::IWriterPtr&& writer) = 0;

    virtual twio::IReaderPtr GetReader() const = 0;
    virtual twio::IWriterPtr GetWriter() const = 0;
};

using IPreprocessorPtr = std::shared_ptr<IPreprocessor>;

TOMIC_END

#endif // _TOMIC_IPREPROCESSOR_H_
