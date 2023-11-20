/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/llvm/asm/impl/VerboseAsmWriter.h>

#include <cstdarg>

TOMIC_LLVM_BEGIN

VerboseAsmWriter::VerboseAsmWriter(twio::IWriterPtr writer)
    : _writer(writer)
{
    TOMIC_ASSERT(writer);
}


VerboseAsmWriterPtr VerboseAsmWriter::New(twio::IWriterPtr writer)
{
    return std::make_shared<VerboseAsmWriter>(writer);
}


void VerboseAsmWriter::Push(char ch)
{
    _writer->Write(ch);
}


void VerboseAsmWriter::Push(const char* format, ...)
{
    va_list args;

    va_start(args, format);
    _writer->WriteVFormat(format, args);
    va_end(args);
}


void VerboseAsmWriter::PushNext(char ch)
{
    PushSpace();
    Push(ch);
}


void VerboseAsmWriter::PushNext(const char* format, ...)
{
    va_list args;

    va_start(args, format);
    PushSpace();
    _writer->WriteVFormat(format, args);
    va_end(args);
}


void VerboseAsmWriter::PushSpace()
{
    _writer->Write(' ');
}


void VerboseAsmWriter::PushSpaces(int repeat)
{
    for (int i = 0; i < repeat; i++)
    {
        _writer->Write(' ');
    }
}


void VerboseAsmWriter::PushNewLine()
{
    _writer->Write('\n');
}


void VerboseAsmWriter::PushNewLines(int repeat)
{
    for (int i = 0; i < repeat; i++)
    {
        _writer->Write('\n');
    }
}


void VerboseAsmWriter::PushComment(const char* format, ...)
{
    va_list args;

    CommentBegin();

    va_start(args, format);
    _writer->WriteVFormat(format, args);
    va_end(args);

    CommentEnd();
}


void VerboseAsmWriter::CommentBegin()
{
    _writer->Write(';');
    PushSpace();
}


void VerboseAsmWriter::CommentEnd()
{
    PushNewLine();
}


TOMIC_LLVM_END
