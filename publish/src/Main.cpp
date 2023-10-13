/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic.h>
#include "Compiler.h"
#include "Argument.h"

using namespace tomic;

const char* input = "testfile.txt";
const char* output = "output.txt";
const char* ext = ".txt";
bool enableCompleteAst = false;
bool enableLog = false;
bool useStandardError = true;

bool ParseArgs(int argc, char* argv[]);
void Configure();
int main(int argc, char* argv[])
{
#ifndef INTERNAL
    if (argc != 1)
    {
        return -1;
    }
#endif

    if (!ParseArgs(argc, argv))
    {
        return 1;
    }

    Configure();

    auto reader = twio::Reader::New(twio::FileInputStream::New(input));
    auto writer = twio::Writer::New(twio::FileOutputStream::New(output));

    Compile(reader, writer);

    return 0;
}

void Configure()
{
    auto container = mioc::SingletonContainer::GetContainer();

    auto config = tomic::Config::New();
    config->SetEnableCompleteAst(enableCompleteAst)
            ->SetEnableLog(enableLog)
            ->SetUseStandardError(useStandardError)
            ->SetOutputExt(ext);
    container->AddSingleton<IConfig>(config);

    RegisterComponents();
}

bool ParseArgs(int argc, char* argv[])
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while ((opt = getopt(argc, argv, "o:e:clr")))
    {
        if (opterr != 0)
        {
            fprintf(stderr, "Argument error: %s\n", optmsg);
            err = true;
            resetopt();
            break;
        }
        switch (opt)
        {
        case 'o':
            output = optarg;
            break;
        case 'e':
            ext = optarg;
            break;
        case 'c':
            enableCompleteAst = true;
            break;
        case 'l':
            enableLog = true;
            break;
        case 'r':
            useStandardError = false;
            break;
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
            {
                input = optarg;
            }
            else if (arg_cnt == 2)
            {
                err = true;
                fprintf(stderr, "Too many arguments\n");
            }
            break;
        case '?':
            err = true;
            fprintf(stderr, "Unknown parameter \"-%c\"\n", optopt);
            break;
        default:
            break;
        }
    }

    if (err)
    {
        fprintf(stderr, "Usage: %s input [-o output]\n", argv[0]);
        return false;
    }

    return true;
}
