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

const char INPUT_FILE[] = "testfile.txt";
const char OUTPUT_FILE[] = "output.txt";

int main()
{
    RegisterComponents();

    auto container = mioc::SingletonContainer::GetContainer();

    auto reader = twio::Reader::New(twio::FileInputStream::New(INPUT_FILE));
    auto writer = twio::Writer::New(twio::FileOutputStream::New(OUTPUT_FILE));

    auto preprocessor = container->Resolve<IPreprocessor>();

    preprocessor->SetReader(reader)->SetWriter(writer)->Process();
    preprocessor.reset();   // release memory

    return 0;
}
