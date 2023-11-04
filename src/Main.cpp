/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic.h>
#include "Argument.h"

using namespace tomic;

static bool ParseArgs(int argc, char* argv[], ConfigPtr config);
static void HandleLongOpt(const char* opt, const char* arg, ConfigPtr config);

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

    ToMiCompiler().Configure(config)->Compile();

    return 0;
}


static bool ParseArgs(int argc, char* argv[], ConfigPtr config)
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while ((opt = getopt(argc, argv, "o:")))
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
            config->Output = optarg;
            break;
        case '@':
            HandleLongOpt(longopt, optarg, config);
            break;
        case '!':
            arg_cnt++;
            if (arg_cnt == 1)
            {
                config->Input = optarg;
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

static void HandleLongOpt(const char* opt, const char* arg, ConfigPtr config)
{
    using namespace tomic::StringUtil;

    if (Equals(opt, "enable-logger"))
    {
        config->EnableLog = true;
        config->LogOutput = IsNullOrEmpty(arg) ? "stdout" : arg;
    }
    else if (Equals(opt, "enable-error"))
    {
        config->EnableError = true;
        config->ErrorOutput = IsNullOrEmpty(arg) ? "stderr" : arg;
    }
    else if (Equals(opt, "verbose-error"))
    {
        config->EnableVerboseError = true;
    }
    else if (Equals(opt, "complete-ast"))
    {
        config->EnableCompleteAst = true;
    }
    else if (Equals(opt, "emit-ast"))
    {
        config->EmitAst = true;
        config->AstOutput = IsNullOrEmpty(arg) ? "ast.xml" : arg;
    }
    else
    {
        fprintf(stderr, "Unknown parameter \"--%s\"\n", opt);
    }
}
