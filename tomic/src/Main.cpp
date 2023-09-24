/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <mioc.h>
#include <twio.h>
#include <tomic.h>
#include <Startup.h>

using namespace tomic;

int main()
{
    RegisterComponents();

    auto container = mioc::SingletonContainer::GetContainer();

    auto preprocessor = container->Resolve<IPreprocessor>();
    auto reader = twio::Reader::New(twio::FileInputStream::New("in.c"));
    auto writer = twio::Writer::New(twio::FileOutputStream::New("out.c"));
    preprocessor->SetReader(reader)->SetWriter(writer)->Process();
    preprocessor.reset();

    return 0;
}
