/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "Compiler.h"
#include <tomic.h>

using namespace tomic;

void RegisterComponents()
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto config = container->Resolve<IConfig>();

    // Logger
    if (config->EnableLog())
    {
        auto loggerWriter = twio::Writer::New(twio::FileOutputStream::New(stdout, false));
        auto logger = DefaultLogger::New();
        logger->SetWriter(loggerWriter)->SetLogLevel(LogLevel::DEBUG);
        container->AddSingleton<ILogger>(logger);
    }
    else
    {
        container->AddSingleton<ILogger, DumbLogger>();
    }

    // Error logger
    if (config->UseStandardErrorMessage())
    {
        container->AddSingleton<IErrorMapper, StandardErrorMapper>()
                ->AddSingleton<IErrorLogger, StandardErrorLogger, IErrorMapper>();
    }
    else
    {
        container->AddSingleton<IErrorMapper, DefaultErrorMapper>()
                ->AddSingleton<IErrorLogger, DefaultErrorLogger, IErrorMapper>();
    }

    // Lexical
    container->AddSingleton<ITokenMapper, DefaultTokenMapper>()
            ->AddTransient<IPreprocessor, DefaultPreprocessor>()
            ->AddTransient<ILexicalAnalyzer, DefaultLexicalAnalyzer, ITokenMapper>()
            ->AddTransient<ILexicalParser, DefaultLexicalParser, ILexicalAnalyzer, IErrorLogger, ILogger>();

    // Syntactic
    if (config->EnableCompleteAst())
    {
        container->AddSingleton<ISyntaxMapper, CompleteSyntaxMapper>();
    }
    else
    {
        container->AddSingleton<ISyntaxMapper, ReducedSyntaxMapper>();
    }
    // container->AddTransient<ISyntacticParser, DefaultSyntacticParser, ILexicalParser, ISyntaxMapper, ITokenMapper, ILogger>();
    container->AddTransient<ISyntacticParser, ResilientSyntacticParser, ILexicalParser, ISyntaxMapper, ITokenMapper, IErrorLogger, ILogger>();

    // Ast printer
    if (tomic::StringUtil::Equals(config->OutputExt(), ".xml"))
    {
        container->AddTransient<IAstPrinter, XmlAstPrinter, ISyntaxMapper, ITokenMapper>();
    }
    else if (tomic::StringUtil::Equals(config->OutputExt(), ".json"))
    {
        container->AddTransient<IAstPrinter, JsonAstPrinter, ISyntaxMapper, ITokenMapper>();
    }
    else
    {
        container->AddTransient<IAstPrinter, StandardAstPrinter, ISyntaxMapper, ITokenMapper>();
    }
}


/*
 * Compile a source file to a target file.
 */

static void Preprocess(twio::IReaderPtr srcReader, twio::IWriterPtr dstWriter);
static void LexicalParse(twio::IAdvancedReaderPtr srcReader, twio::IWriterPtr dstWriter);
static void SyntacticParse(twio::IAdvancedReaderPtr srcReader, twio::IWriterPtr dstWriter);
static void SemanticParse(twio::IAdvancedReaderPtr srcReader, twio::IWriterPtr dstWriter);

void Compile(twio::IReaderPtr srcReader, twio::IWriterPtr dstWriter)
{
    // Preprocess.
    auto writer = twio::Writer::New(twio::BufferOutputStream::New());
    Preprocess(srcReader, writer);

    auto reader = twio::AdvancedReader::New(twio::BufferInputStream::New(writer->Stream()->Yield()));

    // Lexical parse only.
    // LexicalParse(reader, dstWriter);

    // Syntactic parse only.
    SyntacticParse(reader, dstWriter);

    // Semantic parse only. (Syntactic with error handling.)
    // SemanticParse(reader, dstWriter);

    auto errorLogger = mioc::SingletonContainer::GetContainer()->Resolve<IErrorLogger>();
    auto errorWriter = twio::Writer::New(twio::FileOutputStream::New(stderr, false));
    errorLogger->Dumps(errorWriter);
}

static void Preprocess(twio::IReaderPtr srcReader, twio::IWriterPtr dstWriter)
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto preprocessor = container->Resolve<IPreprocessor>();
    preprocessor->SetReader(srcReader)->SetWriter(dstWriter)->Process();
}

static void LexicalParse(twio::IAdvancedReaderPtr srcReader, twio::IWriterPtr dstWriter)
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto lexicalParser = container->Resolve<ILexicalParser>();
    auto mapper = container->Resolve<ITokenMapper>();
    TokenPtr token;

    lexicalParser->SetReader(srcReader);
    while ((token = lexicalParser->Next()) != nullptr)
    {
        if (token->type == TokenType::TK_TERMINATOR)
        {
            break;
        }
        dstWriter->Write(mapper->Description(token->type));
        dstWriter->Write(" ");
        dstWriter->Write(token->lexeme.c_str());
        dstWriter->Write("\n");
    }
}

static void SyntacticParse(twio::IAdvancedReaderPtr srcReader, twio::IWriterPtr dstWriter)
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto syntacticParser = container->Resolve<ISyntacticParser>();

    syntacticParser->SetReader(srcReader);

    auto logger = container->Resolve<ILogger>();
    auto tree = syntacticParser->Parse();
    if (!tree)
    {
        logger->Log(LogLevel::FATAL, "Syntactic parse failed.");
        return;
    }
    if (logger->Count(LogLevel::ERROR) > 0)
    {
        logger->Log(LogLevel::FATAL, "Syntactic parse completed with errors.");
    }

    container->Resolve<IAstPrinter>()->Print(tree, dstWriter);
}

static void SemanticParse(twio::IAdvancedReaderPtr srcReader, twio::IWriterPtr dstWriter)
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto syntacticParser = container->Resolve<ISyntacticParser>();

    syntacticParser->SetReader(srcReader);

    auto logger = container->Resolve<ILogger>();
    auto tree = syntacticParser->Parse();
    if (!tree)
    {
        logger->Log(LogLevel::FATAL, "Syntactic parse failed.");
    }
}
