# ToMiCompiler

A shabby compiler for BUAA 2023 Autumn Compiler Technology course.

> "Shabby" means that it is not a complete compiler, but a toy compiler that can only compile a small subset of C language.

_"Past the point of no return, the final threshold."_

---

[![Build](https://github.com/Lord-Turmoil/ToMiCompiler/actions/workflows/cmake-multi-platform.yml/badge.svg?branch=main)](https://github.com/Lord-Turmoil/ToMiCompiler/actions/workflows/cmake-multi-platform.yml)

## DISCLAIMER 

DO NOT COPY IT DIRECTLY! Study purposes is welcomed, but don't copy any parts to your projects.

---

## Overview

### Basic Structure

The primary idea of this compiler is using Inverse of Control to manage all processes in compilation. To achieve this, I used [MinIoC](https://github.com/Lord-Turmoil/MinIoC) as my IoC container.

The structure of this compiler confoms the classic 5 parts: Lexical Parsing, Syntactical Parsing, Semantic Parsing, Intermediate Code Generation (LLVM) and Target Code Generation (MIPS).

Lexical parser will read the source code, and return all tokens in it. It is the simplest, so I merged it with syntax parser. Here, all errors with tokens will be discovered and reported.

The syntax parser will call lexical parser, and generate a basic AST. Here, all syntax errors will be reported.

> Although I use "AST", this thing is in fact not strictly an Abstract Syntax Tree. Instead, it is a top-down derivation tree.

The semantic parser will then use this AST to do attribute analysis, and construct the symbol table. At the same time, it will add more information to the AST, making it "complete". All errors will be reported here, and the compilation process will end up here if any error exists.

Then, the intermediate code generator will use the "complete" AST to generate LLVM. After generation, it will output a LLVM `Module`.

> The generator is called `IAsmGenerator`, following the name in LLVM source.

At last, the MIPS generator will take the LLVM `Module`, and output MIPS assemply code.

### Details

The compiler is highly configurable, you can use command-line arguments to choose what to be injected as implementation. For example, the `IAstPrinter` can allow you to output XML format AST when debugging locally.

For AST, I mimicked [TinyXML2](https://github.com/leethomason/tinyxml2), thus my AST is in fact a XML document. It empowers the AST with incredible flexibility, and makes it easier for semantic parsing, which uses Attribute Translation Grammar (ATG).

For the intermediate code, I chose LLVM, and refered to a relatively old version of it, see [LLVM](https://github.com/llvm-mirror/llvm). The two main concepts in LLVM are [Value](https://llvm.org/doxygen/classllvm_1_1Value.html) and [Type](https://llvm.org/doxygen/classllvm_1_1Type.html), for which I implemented my own version.

## Epilogue

Due to some iresistable factors, this project was forced to halt. However, it was not ended, as I re-implemented it with Java. You can find the final project here: [ToMiCompiler4J](https://github.com/Lord-Turmoil/ToMiCompiler4J).
