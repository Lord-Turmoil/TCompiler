/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic.h>
#include "Compiler.h"
#include "Argument.h"

using namespace tomic;

static const char* input = "testfile.txt";
static const char* output = "output.txt";
static const char* error = "error.txt";
static const char* ext = ".txt";
static bool silent = false;
static bool enableCompleteAst = false;
static bool enableLog = false;
static bool enableVerboseError = false;

bool ParseArgs(int argc, char* argv[]);
void Configure();

/*
 * Usage: ToMiCompiler [input] [-o output] [-e error] [-x ext] [-s] [-c] [-l] [-v]
 *  input: input file path
 *  output: output file path
 *  error: error file path, "stderr" for standard error stream, and "null" for no error output
 *  ext: output file extension (for syntax tree shape)
 *  -s: Silent mode, no output (error output is not affected)
 *  -c: Enable complete syntax tree (for debug)
 *  -l: Enable log (for debug, output to stdout)
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

    if (!ParseArgs(argc, argv))
    {
        return 1;
    }

    Configure();
    RegisterComponents();
    Compile();

    return 0;
}

void Configure()
{
    auto container = mioc::SingletonContainer::GetContainer();

    auto config = tomic::Config::New();
    config->SetInput(input)
            ->SetOutput(output)
            ->SetError(error)
            ->SetExt(ext)
            ->SetSilent(silent)
            ->SetEnableCompleteAst(enableCompleteAst)
            ->SetEnableLog(enableLog)
            ->SetEnableVerboseError(enableVerboseError);
    container->AddSingleton<IConfig>(config);
}

bool ParseArgs(int argc, char* argv[])
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while ((opt = getopt(argc, argv, "o:e:x:sclv")))
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
            error = optarg;
            break;
        case 'x':
            ext = optarg;
            break;
        case 'c':
            enableCompleteAst = true;
            break;
        case 'l':
            enableLog = true;
            break;
        case 'v':
            enableVerboseError = true;
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
