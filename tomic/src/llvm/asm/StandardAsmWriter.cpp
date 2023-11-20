/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/asm/impl/StandardAsmWriter.h>

#include <cstdarg>

TOMIC_LLVM_BEGIN

StandardAsmWriter::StandardAsmWriter(twio::IWriterPtr writer)
    : _writer(writer), _isCommenting(false)
{
    TOMIC_ASSERT(writer);
}


StandardAsmWriterPtr StandardAsmWriter::New(twio::IWriterPtr writer)
{
    return std::make_shared<StandardAsmWriter>(writer);
}


void StandardAsmWriter::Push(char ch)
{
    if (!isCommenting)
    {
        return;
    }

    _writer->Write(ch);
}


void StandardAsmWriter::Push(const char* format, ...)
{
    if (!isCommenting)
    {
        return;
    }

    va_list args;

    va_start(args, format);
    _writer->WriteVFormat(format, args);
    va_end(args);
}


void StandardAsmWriter::PushNext(char ch)
{
    if (!isCommenting)
    {
        return;
    }

    PushSpace();
    Push(ch);
}


void StandardAsmWriter::PushNext(const char* format, ...)
{
    if (!isCommenting)
    {
        return;
    }

    va_list args;

    va_start(args, format);
    PushSpace();
    _writer->WriteVFormat(format, args);
    va_end(args);
}


void StandardAsmWriter::PushSpace()
{
    if (!isCommenting)
    {
        return;
    }

    _writer->Write(' ');
}


void StandardAsmWriter::PushSpaces(int repeat)
{
    if (!isCommenting)
    {
        return;
    }

    for (int i = 0; i < repeat; i++)
    {
        _writer->Write(' ');
    }
}


void StandardAsmWriter::PushNewLine()
{
    if (!isCommenting)
    {
        return;
    }

    _writer->Write('\n');
}


void StandardAsmWriter::PushNewLines(int repeat)
{
    if (!isCommenting)
    {
        return;
    }

    for (int i = 0; i < repeat; i++)
    {
        _writer->Write('\n');
    }
}


void StandardAsmWriter::PushComment(const char* format, ...)
{
    return;
}


void StandardAsmWriter::CommentBegin()
{
    _isCommenting = true;
}


void StandardAsmWriter::CommentEnd()
{
    _isCommenting = false;
}


TOMIC_LLVM_END
