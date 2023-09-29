/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include "../mioc/include/mioc.h"
#include "../twio/include/twio.h"
#include "../tomic/include/tomic.h"
#include "../include/Startup.h"
#include <cstring>

using namespace tomic;

const char LOCAL_INPUT_FILE[] = "in.c";
const char LOCAL_OUTPUT_FILE[] = "out.txt";
const char INPUT_FILE[] = "testfile.txt";
const char OUTPUT_FILE[] = "output.txt";

int main(int argc, char* argv[])
{
    const char* input;
    const char* output;
    if (argc >= 2 && strcmp(argv[1], "-local") == 0)
    {
        input = LOCAL_INPUT_FILE;
        output = LOCAL_OUTPUT_FILE;
    }
    else
    {
        input = INPUT_FILE;
        output = OUTPUT_FILE;
    }

    RegisterComponents();

    auto reader = twio::Reader::New(twio::FileInputStream::New(input));
    auto writer = twio::Writer::New(twio::FileOutputStream::New(output));

    Compile(reader, writer);

    return 0;
}
