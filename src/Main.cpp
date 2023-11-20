/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

#include <tomic.h>
#include "Arg.h"

using namespace tomic;

static bool showHelp = false;
static void ShowHelp();
static bool ParseArgs(int argc, char* argv[], ConfigPtr config);
static bool HandleLongOpt(const char* opt, const char* arg, ConfigPtr config);

/*
 * Usage: ToMiCompiler <input> [-o output]
 *           --target=syntactic
 *           --enable-logger[=filename]
 *           --enable-error[=filename] --verbose-error
 *           --emit-ast[=filename] --complete-ast
 *
 *   --target, -t:         specify the target type
 *   --enable-logger, -l:  enable logger
 *   --enable-error, -e:   enable error
 *   --verbose-error, -v:  verbose error
 *   --emit-ast, -a:       emit ast
 *   --complete-ast, -c:   complete ast
 *   --help, -h:           show help
 */
int main(int argc, char* argv[])
{
#ifdef INTERNAL
    printf("Tony's Mini Compiler (ToMiC) [Version %s]\n", TOMIC_VERSION);
    printf("(C) Tony's Studio 2018 - 2023. All rights reserved.\n");
    printf("For BUAA 2023 Compiler Technology\n\n");
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

    if (showHelp)
    {
        ShowHelp();
        return 0;
    }

#ifndef INTERNAL
    // Override default config...
#endif

    ToMiCompiler().Configure(config)->Compile();

    return 0;
}

static void ShowHelp()
{
    static const char HELP[] = R"(
Usage: ToMiCompiler <input> [-o output]
          --target=(syntactic | semantic | ir | asm)
          --enable-logger[=filename]
          --enable-error[=filename] --verbose-error
          --emit-ast[=filename] --complete-ast
          --emit-llvm[=filename]

  --target, -t:         specify the target type
  --enable-logger, -l:  enable logger
  --enable-error, -e:   enable error
  --verbose-error, -v:  verbose error
  --emit-ast, -a:       emit ast
  --complete-ast, -c:   complete ast
  --emit-llvm, -i:      emit llvm ir
  --help, -h:           show help
    )";
    printf("%s\n", HELP);
}

static bool ParseArgs(int argc, char* argv[], ConfigPtr config)
{
    int opt;
    int arg_cnt = 0;
    bool err = false;
    while ((opt = getopt(argc, argv, "o:t:l:e:va:ci:h")))
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
        case 't':
            HandleLongOpt("target", optarg, config);
            break;
        case 'l':
            HandleLongOpt("enable-logger", optarg, config);
            break;
        case 'e':
            HandleLongOpt("enable-error", optarg, config);
            break;
        case 'v':
            HandleLongOpt("verbose-error", optarg, config);
            break;
        case 'a':
            HandleLongOpt("emit-ast", optarg, config);
            break;
        case 'c':
            HandleLongOpt("complete-ast", optarg, config);
            break;
        case 'i':
            HandleLongOpt("emit-llvm", optarg, config);
            break;
        case 'h':
            HandleLongOpt("help", optarg, config);
            break;
        case '@':
            if (!HandleLongOpt(longopt, optarg, config))
            {
                err = true;
            }
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
        return false;
    }

    return true;
}

static bool HandleLongOpt(const char* opt, const char* arg, ConfigPtr config)
{
    using namespace tomic::StringUtil;

    if (Equals(opt, "target"))
    {
        if (IsNullOrEmpty(arg))
        {
            fprintf(stderr, "Target type cannot be empty\n");
            return false;
        }
        if (Equals(arg, "preprocess"))
        {
            config->Target = Config::TargetType::Preprocess;
        }
        else if (Equals(arg, "syntactic"))
        {
            config->Target = Config::TargetType::Syntactic;
        }
        else if (Equals(arg, "semantic"))
        {
            config->Target = Config::TargetType::Semantic;
        }
        else if (Equals(arg, "ir"))
        {
            config->Target = Config::TargetType::IR;
        }
        else if (Equals(arg, "asm"))
        {
            config->Target = Config::TargetType::ASM;
        }
        else
        {
            fprintf(stderr, "Unknown target \"%s\"\n", arg);
            return false;
        }
    }
    else if (Equals(opt, "enable-logger"))
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
    else if (Equals(opt, "emit-llvm"))
    {
        config->EmitLlvm = true;
        config->LlvmOutput = IsNullOrEmpty(arg) ? "llvm.ll" : arg;
    }
    else if (Equals(opt, "help"))
    {
        showHelp = true;
    }
    else
    {
        fprintf(stderr, "Unknown parameter \"--%s\"\n", opt);
        return false;
    }

    return true;
}
