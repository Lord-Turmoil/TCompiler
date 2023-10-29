/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic.h>
#include "Compiler.h"
#include "Argument.h"

using namespace tomic;

static bool ParseArgs(int argc, char* argv[], ConfigPtr config);

/*
 * Usage: ToMiCompiler [input] [-a filename] [-i filename] [-e filename] [-l filename] [-c] [-v]
 *  input: input file path
 *  -a: output file path for syntax tree
 *  -i: output file path for intermediate code
 *  -e: error file path
 *  -l: log file path
 *  -c: Enable complete syntax tree (for debug)
 *  -v: Use verbose error (for debug)
 */
int main(int argc, char* argv[])
{
#ifdef INTERNAL
    printf("ToMiCompiler v1.0\n");
    printf("For BUAA 2023 Compiler Technology\n");
    printf("Copyright (C) Tony's Studio 2018 - 2023\n");
    printf("________________________________________\n");
#endif

#ifndef INTERNAL
    if (argc != 1)
    {
        return -1;
    }
#endif

    auto config = Config::New();

    if (!ParseArgs(argc, argv, config))
    {
        return 1;
    }

    mioc::SingletonContainer::GetContainer()->AddSingleton<Config>(config);

    RegisterComponents();
    Compile();

    return 0;
}

static bool ParseArgs(int argc, char* argv[], ConfigPtr config)
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while ((opt = getopt(argc, argv, "a:i:e:l:cv")))
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
        case 'a':
            config->SetAstOutput(optarg);
            break;
        case 'i':
            config->SetIrOutput(optarg);
            break;
        case 'e':
            config->SetErrorOutput(optarg);
            break;
        case 'l':
            config->SetLogOutput(optarg);
            break;
        case 'c':
            config->SetEnableCompleteAst(true);
            break;
        case 'v':
            config->SetEnableVerboseError(true);
            break;
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
            {
                config->SetInput(optarg);
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
