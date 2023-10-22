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
    if (config->EnableVerboseError())
    {
        container->AddSingleton<IErrorMapper, VerboseErrorMapper>()
                ->AddSingleton<IErrorLogger, VerboseErrorLogger, IErrorMapper>();
    }
    else
    {
        container->AddSingleton<IErrorMapper, StandardErrorMapper>()
                ->AddSingleton<IErrorLogger, StandardErrorLogger, IErrorMapper>();
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
    container->AddTransient<ISyntacticParser, ResilientSyntacticParser, ILexicalParser, ISyntaxMapper, ITokenMapper, IErrorLogger, ILogger>();

    // Semantic
    container->AddTransient<ISemanticAnalyzer, DefaultSemanticAnalyzer, IErrorLogger, ILogger>();
    container->AddTransient<ISemanticParser, DefaultSemanticParser, ISemanticAnalyzer, ILogger>();

    // Ast printer
    if (tomic::StringUtil::Equals(config->Ext(), ".xml"))
    {
        container->AddTransient<IAstPrinter, XmlAstPrinter, ISyntaxMapper, ITokenMapper>();
    }
    else if (tomic::StringUtil::Equals(config->Ext(), ".json"))
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
static SyntaxTreePtr SyntacticParse(twio::IAdvancedReaderPtr srcReader);
static SymbolTablePtr SemanticParse(SyntaxTreePtr tree);

void Compile()
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto config = container->Resolve<IConfig>();
    auto srcReader = twio::AdvancedReader::New(twio::FileInputStream::New(config->Input()));

    // Preprocess.
    auto writer = twio::Writer::New(twio::BufferOutputStream::New());
    Preprocess(srcReader, writer);

    auto reader = twio::AdvancedReader::New(twio::BufferInputStream::New(writer->Stream()->Yield()));

    // Lexical parse only.
    // LexicalParse(reader, dstWriter);

    auto logger = container->Resolve<ILogger>();

    // Syntactic parse only.
    auto tree = SyntacticParse(reader);

    if (tree == nullptr)
    {
        logger->LogFormat(LogLevel::FATAL, "Syntactic parse failed, compilation aborted");
        return;
    }

    // Semantic parse only.
    // SemanticParse(tree);

    if (!config->Silent())
    {
        auto dstWriter = twio::Writer::New(twio::FileOutputStream::New(config->Output()));
        auto printer = container->Resolve<IAstPrinter>();
        printer->Print(tree, dstWriter);
    }

    auto errorLogger = mioc::SingletonContainer::GetContainer()->Resolve<IErrorLogger>();
    twio::WriterPtr errorWriter;
    if (tomic::StringUtil::Equals(config->Error(), "stderr"))
    {
        errorWriter = twio::Writer::New(twio::FileOutputStream::New(stderr, false));
    }
    else if (tomic::StringUtil::Equals(config->Error(), "null"))
    {
        errorWriter = nullptr;
    }
    else
    {
        errorWriter = twio::Writer::New(twio::FileOutputStream::New(config->Error()));
    }

    if (errorWriter)
    {
        errorLogger->Dumps(errorWriter);
    }
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

static SyntaxTreePtr SyntacticParse(twio::IAdvancedReaderPtr srcReader)
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto syntacticParser = container->Resolve<ISyntacticParser>();

    syntacticParser->SetReader(srcReader);

    auto logger = container->Resolve<ILogger>();
    auto tree = syntacticParser->Parse();
    if (!tree)
    {
        logger->LogFormat(LogLevel::FATAL, "Syntactic parse failed.");
        return nullptr;
    }
    if (logger->Count(LogLevel::ERROR) > 0)
    {
        logger->LogFormat(LogLevel::FATAL, "Syntactic parse completed with errors.");
    }

    return tree;
}

static SymbolTablePtr SemanticParse(SyntaxTreePtr tree)
{
    auto container = mioc::SingletonContainer::GetContainer();

    auto parser = container->Resolve<ISemanticParser>();
    return parser->Parse(tree);
}
