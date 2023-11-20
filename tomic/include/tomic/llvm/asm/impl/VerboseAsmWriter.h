/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_VERBOSE_ASM_WRITER_H_
#define _TOMIC_LLVM_VERBOSE_ASM_WRITER_H_

#include <memory>
#include <tomic/llvm/asm/IAsmWriter.h>

TOMIC_LLVM_BEGIN

class VerboseAsmWriter;
using VerboseAsmWriterPtr = std::shared_ptr<VerboseAsmWriter>;


class VerboseAsmWriter : public IAsmWriter
{
public:
    VerboseAsmWriter(twio::IWriterPtr writer);
    ~VerboseAsmWriter() override = default;

    static VerboseAsmWriterPtr New(twio::IWriterPtr writer);

    void Push(char ch) override;
    void Push(const char* format, ...) override;

    void PushNext(char ch) override;
    void PushNext(const char* format, ...) override;

    void PushSpace() override;
    void PushSpaces(int repeat) override;
    void PushNewLine() override;
    void PushNewLines(int repeat) override;

    void PushComment(const char* format, ...) override;
    void CommentBegin() override;
    void CommentEnd() override;

private:
    twio::IWriterPtr _writer;
};


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_VERBOSE_ASM_WRITER_H_
