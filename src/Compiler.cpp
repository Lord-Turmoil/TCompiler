/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "Compiler.h"
#include <tomic.h>

using namespace tomic;

static twio::IWriterPtr BuildWriter(const char* filename);

void RegisterComponents()
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto config = container->Resolve<Config>();

    // Logger
    auto loggerWriter = BuildWriter(config->LogOutput());
    if (loggerWriter)
    {
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
    const char* astFilename = config->AstOutput();
    if (tomic::StringUtil::EndsWith(astFilename, ".xml"))
    {
        container->AddTransient<IAstPrinter, XmlAstPrinter, ISyntaxMapper, ITokenMapper>();
    }
    else if (tomic::StringUtil::EndsWith(astFilename, ".json"))
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
static void GenerateIR(mioc::ServiceContainerPtr container, SyntaxTreePtr tree, SymbolTablePtr table);

static void OutputSyntaxTree(mioc::ServiceContainerPtr container, SyntaxTreePtr tree);
static int LogError(mioc::ServiceContainerPtr container);

void Compile()
{
    auto container = mioc::SingletonContainer::GetContainer();
    auto config = container->Resolve<Config>();

    // Preprocess.
    auto srcReader = twio::AdvancedReader::New(twio::FileInputStream::New(config->Input()));
    auto writer = BuildWriter(nullptr);
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
    auto table = SemanticParse(tree);

    // Output syntax tree.
    OutputSyntaxTree(container, tree);

    // Log errors.
    int errorCount = LogError(container);
    if (errorCount > 0)
    {
        logger->LogFormat(LogLevel::FATAL, "Compilation completed with %d errors", errorCount);
        return;
    }

    // Generate IR.
    GenerateIR(container, tree, table);
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

static void GenerateIR(mioc::ServiceContainerPtr container, SyntaxTreePtr tree, SymbolTablePtr table)
{
    auto config = container->Resolve<Config>();

    // Generate IR.
    auto writer = BuildWriter(config->IrOutput());
    writer->Write("IR Not Implemented\n");
}

static void OutputSyntaxTree(mioc::ServiceContainerPtr container, SyntaxTreePtr tree)
{
    auto config = container->Resolve<Config>();

    // Output syntax tree.
    auto dstWriter = BuildWriter(config->AstOutput());
    if (dstWriter)
    {
        auto printer = container->Resolve<IAstPrinter>();
        printer->Print(tree, dstWriter);
    }
}

static int LogError(mioc::ServiceContainerPtr container)
{
    auto config = container->Resolve<Config>();

    auto errorLogger = mioc::SingletonContainer::GetContainer()->Resolve<IErrorLogger>();
    auto errorWriter = BuildWriter(config->ErrorOutput());
    if (errorWriter)
    {
        errorLogger->Dumps(errorWriter);
    }

    return errorLogger->Count();
}

static twio::IWriterPtr BuildWriter(const char* filename)
{
    if (!filename)
    {
        return twio::Writer::New(twio::BufferOutputStream::New());
    }

    if (tomic::StringUtil::Equals(filename, "null"))
    {
        return nullptr;
    }

    if (tomic::StringUtil::Equals(filename, "stdout"))
    {
        return twio::Writer::New(twio::FileOutputStream::New(stdout, false));
    }

    if (tomic::StringUtil::Equals(filename, "stderr"))
    {
        return twio::Writer::New(twio::FileOutputStream::New(stderr, false));
    }

    return twio::Writer::New(twio::FileOutputStream::New(filename));
}
