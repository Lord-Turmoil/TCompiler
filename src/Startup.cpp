/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../mioc/include/mioc.h"
#include "../tomic/include/tomic.h"

using namespace tomic;

void RegisterComponents()
{
    mioc::SingletonContainer::GetContainer()
            ->AddTransient<IPreprocessor, DefaultPreprocessor>()
            ->AddSingleton<ITokenMapper, DefaultTokenMapper>()
            ->AddTransient<ILexicalAnalyser, DefaultLexicalAnalyser>()
            ->AddTransient<ILexicalParser, DefaultLexicalParser, ILexicalAnalyser>();
}

void Compile(twio::IReaderPtr srcReader, twio::IWriterPtr dstWriter)
{
    auto container = mioc::SingletonContainer::GetContainer();

    auto preprocessor = container->Resolve<IPreprocessor>();
    auto writer = twio::Writer::New(twio::BufferOutputStream::New());
    preprocessor->SetReader(srcReader)->SetWriter(writer)->Process();

    auto lexicalParser = container->Resolve<ILexicalParser>();
    auto reader = twio::AdvancedReader::New(twio::BufferInputStream::New(writer->Stream()->Yield()));
    lexicalParser->SetReader(reader);

    TokenPtr token;
    auto mapper = container->Resolve<ITokenMapper>();
    while ((token = lexicalParser->Next()) != nullptr)
    {
        dstWriter->Write(mapper->Description(token->type));
        dstWriter->Write(" ");
        dstWriter->Write(token->lexeme.c_str());
        dstWriter->Write("\n");
    }
}
