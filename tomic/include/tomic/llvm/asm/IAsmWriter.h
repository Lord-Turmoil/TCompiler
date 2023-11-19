/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#ifndef _TOMIC_LLVM_ASM_WRITER_H_
#define _TOMIC_LLVM_ASM_WRITER_H_

#include <memory>
#include <tomic/llvm/Llvm.h>

TOMIC_LLVM_BEGIN

/*
 * A wrapper for standard writer to write LLVM IR ASM with
 * more convenience.
 */
class IAsmWriter
{
public:
    virtual ~IAsmWriter() = default;

    virtual void Push(char ch) = 0;
    virtual void Push(const char* format, ...) = 0;

    virtual void PushNext(char ch) = 0;
    virtual void PushNext(const char* format, ...) = 0;

    virtual void PushSpace() = 0;
    virtual void PushSpaces(int repeat) = 0;
    virtual void PushNewLine() = 0;
    virtual void PushNewLines(int repeat) = 0;

    // Push a line comment, will insert a new line at the end.
    virtual void PushComment(const char* format, ...) = 0;
    // Push a ';' and a space.
    virtual void CommentBegin() = 0;
    // Push a new line to terminate the comment.
    virtual void CommentEnd() = 0;
};


using IAsmWriterPtr = std::shared_ptr<IAsmWriter>;


TOMIC_LLVM_END

#endif // _TOMIC_LLVM_ASM_WRITER_H_
