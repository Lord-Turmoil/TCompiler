/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic/Config.h>
#include <tomic/lexer/ILexicalAnalyzer.h>
#include <tomic/lexer/ILexicalParser.h>
#include <tomic/lexer/impl/DefaultLexicalAnalyzer.h>
#include <tomic/lexer/impl/DefaultLexicalParser.h>
#include <tomic/lexer/impl/DefaultPreprocessor.h>
#include <tomic/lexer/impl/HeaderPreprocessor.h>
#include <tomic/lexer/impl/token/DefaultTokenMapper.h>
#include <tomic/lexer/IPreprocessor.h>
#include <tomic/lexer/token/ITokenMapper.h>
#include <tomic/llvm/asm/IAsmGenerator.h>
#include <tomic/llvm/asm/IAsmPrinter.h>
#include <tomic/llvm/asm/impl/StandardAsmGenerator.h>
#include <tomic/llvm/asm/impl/VerboseAsmPrinter.h>
#include <tomic/llvm/asm/impl/StandardAsmPrinter.h>
#include <tomic/logger/debug/impl/DefaultLogger.h>
#include <tomic/logger/debug/impl/DumbLogger.h>
#include <tomic/logger/error/IErrorLogger.h>
#include <tomic/logger/error/IErrorMapper.h>
#include <tomic/logger/error/impl/StandardErrorLogger.h>
#include <tomic/logger/error/impl/StandardErrorMapper.h>
#include <tomic/logger/error/impl/VerboseErrorLogger.h>
#include <tomic/logger/error/impl/VerboseErrorMapper.h>
#include <tomic/parser/ast/mapper/CompleteSyntaxMapper.h>
#include <tomic/parser/ast/mapper/ISyntaxMapper.h>
#include <tomic/parser/ast/mapper/ReducedSyntaxMapper.h>
#include <tomic/parser/ast/printer/IAstPrinter.h>
#include <tomic/parser/ast/printer/JsonAstPrinter.h>
#include <tomic/parser/ast/printer/StandardAstPrinter.h>
#include <tomic/parser/ast/printer/XmlAstPrinter.h>
#include <tomic/parser/impl/DefaultSemanticAnalyzer.h>
#include <tomic/parser/impl/DefaultSemanticParser.h>
#include <tomic/parser/impl/ResilientSyntacticParser.h>
#include <tomic/parser/ISemanticAnalyzer.h>
#include <tomic/parser/ISemanticParser.h>
#include <tomic/parser/ISyntacticParser.h>
#include <tomic/Shared.h>
#include <tomic/ToMiCompiler.h>
#include <tomic/utils/StringUtil.h>

TOMIC_BEGIN

static twio::IWriterPtr BuildWriter(const char* filename);
static void OutputSyntaxTree(const char* filename, IAstPrinterPtr printer, SyntaxTreePtr tree);
static void OutputLlvmAsm(const char* filename, llvm::IAsmPrinterPtr printer, llvm::ModuleSmartPtr module);


class ToMiCompilerImpl
{
    friend class ToMiCompiler;

public:
    ToMiCompilerImpl() :
        _config(nullptr),
        _container(mioc::ServiceContainer::New())
    {
    }


    ToMiCompilerImpl* Configure(ConfigPtr config)
    {
        _config = config;
        return this;
    }


    ToMiCompilerImpl* Configure(std::function<void(mioc::ServiceContainerPtr)> config)
    {
        config(_container);
        return this;
    }


    // Compilation processes.
    void Compile();

private:
    bool _Preprocess(twio::IWriterPtr* outWriter);
    bool _SyntacticParse(twio::IAdvancedReaderPtr reader, SyntaxTreePtr* outAst);
    bool _SemanticParse(SyntaxTreePtr ast, SymbolTablePtr* outTable);

    bool _GenerateLlvmAsm(SyntaxTreePtr ast, SymbolTablePtr table, llvm::ModuleSmartPtr* outModule);

    void _LogError();

    ConfigPtr _config;
    mioc::ServiceContainerPtr _container;
};


ToMiCompiler::ToMiCompiler()
{
    _impl = new ToMiCompilerImpl();
}


ToMiCompiler::~ToMiCompiler()
{
    if (_impl)
    {
        delete _impl;
    }
}


ToMiCompiler* ToMiCompiler::Configure(ConfigPtr config)
{
    _impl->Configure(config);
    return this;
}


void ToMiCompiler::Compile()
{
    if (_impl->_config == nullptr)
    {
        _impl->_config = Config::New();
    }

    // Configure all components.
    auto config = _impl->_config;
    // Logger
    _impl->Configure([=](mioc::ServiceContainerPtr container) {
        if (config->EnableLog)
        {
            if (config->LogOutput.empty())
            {
                config->LogOutput = "stdout";
            }
            auto loggerWriter = BuildWriter(config->LogOutput.c_str());
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
        }
        else
        {
            container->AddSingleton<ILogger, DumbLogger>();
        }
    });
    // Error logger
    _impl->Configure([=](mioc::ServiceContainerPtr container) {
        if (config->EnableVerboseError)
        {
            container->AddSingleton<IErrorMapper, VerboseErrorMapper>()
                ->AddSingleton<IErrorLogger, VerboseErrorLogger, IErrorMapper>();
        }
        else
        {
            container->AddSingleton<IErrorMapper, StandardErrorMapper>()
                ->AddSingleton<IErrorLogger, StandardErrorLogger, IErrorMapper>();
        }
    });
    // Lexical
    _impl->Configure([=](mioc::ServiceContainerPtr container) {
        container->AddSingleton<ITokenMapper, DefaultTokenMapper>()
            ->AddTransient<IPreprocessor, HeaderPreprocessor>()
            //->AddTransient<IPreprocessor, DefaultPreprocessor>()
            ->AddTransient<ILexicalAnalyzer, DefaultLexicalAnalyzer, ITokenMapper>()
            ->AddTransient<ILexicalParser, DefaultLexicalParser, ILexicalAnalyzer, IErrorLogger, ILogger>();
    });
    // Ast printer (Might be used by Syntactic and Semantic.
    _impl->Configure([=](mioc::ServiceContainerPtr container) {
        if (config->EmitAst)
        {
            if (config->AstOutput.empty())
            {
                config->AstOutput = "ast.xml";
            }
            const char* astFilename = config->AstOutput.c_str();
            if (StringUtil::EndsWith(astFilename, ".xml"))
            {
                container->AddTransient<IAstPrinter, XmlAstPrinter, ISyntaxMapper, ITokenMapper>();
            }
            else if (StringUtil::EndsWith(astFilename, ".json"))
            {
                container->AddTransient<IAstPrinter, JsonAstPrinter, ISyntaxMapper, ITokenMapper>();
            }
            else
            {
                container->AddTransient<IAstPrinter, StandardAstPrinter, ISyntaxMapper, ITokenMapper>();
            }
        }
    });
    // Syntactic
    _impl->Configure([=](mioc::ServiceContainerPtr container) {
        if (config->EnableCompleteAst)
        {
            container->AddSingleton<ISyntaxMapper, CompleteSyntaxMapper>();
        }
        else
        {
            container->AddSingleton<ISyntaxMapper, ReducedSyntaxMapper>();
        }
        container->AddTransient<ISyntacticParser, ResilientSyntacticParser, ILexicalParser, ISyntaxMapper, ITokenMapper,
            IErrorLogger, ILogger>();
    });
    // Semantic
    _impl->Configure([=](mioc::ServiceContainerPtr container) {
        container->AddTransient<ISemanticAnalyzer, DefaultSemanticAnalyzer, IErrorLogger, ILogger>();
        container->AddTransient<ISemanticParser, DefaultSemanticParser, ISemanticAnalyzer, ILogger>();
    });
    // LLVM
    _impl->Configure([=](mioc::ServiceContainerPtr container) {
        container->AddTransient<llvm::IAsmGenerator, llvm::StandardAsmGenerator>();
        if (config->EnableVerboseLlvm)
        {
            container->AddTransient<llvm::IAsmPrinter, llvm::VerboseAsmPrinter>();
        }
        else
        {
            container->AddTransient<llvm::IAsmPrinter, llvm::StandardAsmPrinter>();
        }
    });

    _impl->Compile();
}


/*
 * ================================ Implementation ================================
 */
void ToMiCompilerImpl::Compile()
{
    auto logger = _container->Resolve<ILogger>();

    // Preprocess
    twio::IWriterPtr output;
    if (!_Preprocess(&output))
    {
        _LogError();
        return;
    }

    // Syntax parse
    SyntaxTreePtr ast;
    auto syntaxReader = twio::AdvancedReader::New(twio::BufferInputStream::New(output->Stream()->Yield()));
    if (!_SyntacticParse(syntaxReader, &ast))
    {
        _LogError();
        return;
    }

    // Semantic parse
    SymbolTablePtr table;
    if (!_SemanticParse(ast, &table))
    {
        _LogError();
        return;
    }

    // Generate LLVM IR
    llvm::ModuleSmartPtr module;
    if (!_GenerateLlvmAsm(ast, table, &module))
    {
        _LogError();
        return;
    }


    // Final error output.
    _LogError();
}


bool ToMiCompilerImpl::_Preprocess(twio::IWriterPtr* outWriter)
{
    if (_config->Target < Config::TargetType::Preprocess)
    {
        return false;
    }

    auto logger = _container->Resolve<ILogger>();
    logger->LogFormat(LogLevel::DEBUG, "Preprocessing \"%s\"...", _config->Input.c_str());

    // ===============
    auto srcReader = twio::AdvancedReader::New(twio::FileInputStream::New(_config->Input.c_str()));
    auto srcWriter = BuildWriter(nullptr);
    _container->Resolve<IPreprocessor>()->SetReader(srcReader)->SetWriter(srcWriter)->Process();
    // ===============

    if (_config->Target == Config::TargetType::Preprocess)
    {
        // Output preprocessed file.
        auto tempReader = twio::AdvancedReader::New(twio::BufferInputStream::New(srcWriter->Stream()->Yield()));
        auto tempWriter = BuildWriter(_config->Output.c_str());
        if (tempWriter)
        {
            twio::Printer::New(tempReader, tempWriter)->Print();
        }
        logger->LogFormat(LogLevel::DEBUG, "Preprocessed file \"%s\" generated", _config->Output.c_str());
        return false;
    }

    if (outWriter)
    {
        *outWriter = srcWriter;
    }

    return true;
}


bool ToMiCompilerImpl::_SyntacticParse(twio::IAdvancedReaderPtr reader, SyntaxTreePtr* outAst)
{
    if (_config->Target < Config::TargetType::Syntactic)
    {
        return false;
    }
    auto logger = _container->Resolve<ILogger>();
    logger->LogFormat(LogLevel::DEBUG, "Parsing \"%s\"...", _config->Input.c_str());

    // ===============
    auto ast = _container->Resolve<ISyntacticParser>()->SetReader(reader)->Parse();
    if (!ast)
    {
        logger->LogFormat(LogLevel::FATAL, "Syntactic parse failed, compilation aborted");
        return false;
    }
    // ===============

    if (logger->Count(LogLevel::ERROR) > 0)
    {
        logger->LogFormat(LogLevel::FATAL, "Syntactic parse completed with errors.");
    }

    if (_config->Target == Config::TargetType::Syntactic)
    {
        if (_config->EmitAst)
        {
            OutputSyntaxTree(_config->AstOutput.c_str(), _container->Resolve<IAstPrinter>(), ast);
        }
    }

    if (outAst)
    {
        *outAst = ast;
    }

    return true;
}


bool ToMiCompilerImpl::_SemanticParse(SyntaxTreePtr ast, SymbolTablePtr* outTable)
{
    if (_config->Target < Config::TargetType::Semantic)
    {
        return false;
    }
    auto logger = _container->Resolve<ILogger>();
    logger->LogFormat(LogLevel::DEBUG, "Performing semantic analyzing on \"%s\"...", _config->Input.c_str());

    // ===============
    auto table = _container->Resolve<ISemanticParser>()->Parse(ast);
    // ===============

    if (_config->EmitAst)
    {
        OutputSyntaxTree(_config->AstOutput.c_str(), _container->Resolve<IAstPrinter>(), ast);
    }

    if (outTable)
    {
        *outTable = table;
    }

    auto errorLogger = _container->Resolve<IErrorLogger>();
    if (!table || (errorLogger->Count() > 0))
    {
        logger->LogFormat(LogLevel::FATAL, "Semantic analysis failed, compilation aborted");
        return false;
    }

    logger->LogFormat(LogLevel::DEBUG, "Semantic analysis completed");

    return true;
}


bool ToMiCompilerImpl::_GenerateLlvmAsm(SyntaxTreePtr ast, SymbolTablePtr table, llvm::ModuleSmartPtr* outModule)
{
    if (_config->Target < Config::TargetType::IR)
    {
        return false;
    }
    auto logger = _container->Resolve<ILogger>();
    logger->LogFormat(LogLevel::DEBUG, "Generating LLVM IR...");

    auto asmGenerator = _container->Resolve<llvm::IAsmGenerator>();
    auto module = asmGenerator->Generate(ast, table, _config->Input.c_str());

    if (!module)
    {
        logger->LogFormat(LogLevel::FATAL, "Generating LLVM IR failed, compilation aborted");
        return false;
    }

    if (outModule)
    {
        *outModule = module;
    }

    if (_config->EmitLlvm)
    {
        OutputLlvmAsm(_config->LlvmOutput.c_str(), _container->Resolve<llvm::IAsmPrinter>(), module);
    }

    return true;
}


void ToMiCompilerImpl::_LogError()
{
    if (!_config->EnableError)
    {
        return;
    }

    auto logger = _container->Resolve<ILogger>();
    auto errorLogger = _container->Resolve<IErrorLogger>();
    auto errorWriter = BuildWriter(_config->ErrorOutput.c_str());
    if (errorWriter && errorLogger->Count() > 0)
    {
        errorLogger->Dumps(errorWriter);
    }

    if (errorLogger->Count() > 0)
    {
        logger->LogFormat(LogLevel::FATAL,
                          "Compilation completed with %d errors",
                          errorLogger->Count());
    }
}


static twio::IWriterPtr BuildWriter(const char* filename)
{
    if (!filename)
    {
        return twio::Writer::New(twio::BufferOutputStream::New());
    }

    if (StringUtil::Equals(filename, "null"))
    {
        return nullptr;
    }

    if (StringUtil::Equals(filename, "stdout"))
    {
        return twio::Writer::New(twio::FileOutputStream::New(stdout, false));
    }

    if (StringUtil::Equals(filename, "stderr"))
    {
        return twio::Writer::New(twio::FileOutputStream::New(stderr, false));
    }

    return twio::Writer::New(twio::FileOutputStream::New(filename));
}


static void OutputSyntaxTree(const char* filename, IAstPrinterPtr printer, SyntaxTreePtr tree)
{
    TOMIC_ASSERT(printer && "Missing AST Printer");
    // Output syntax tree.
    auto writer = BuildWriter(filename);
    if (writer)
    {
        printer->Print(tree, writer);
    }
}


static void OutputLlvmAsm(const char* filename, llvm::IAsmPrinterPtr printer, llvm::ModuleSmartPtr module)
{
    printer->Print(module.get(), BuildWriter(filename));
}


TOMIC_END
