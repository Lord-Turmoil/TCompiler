/*******************************************************************************
 * Copyright (C) Tony's Studio 2018 - 2023. All rights reserved.
 *
 *   For BUAA 2023 Compiler Technology
 */

/******************************************************************************
 ***                        T O N Y ' S  S T U D I O                        ***
 ******************************************************************************
 *                   Project Name : PassBashPro                               *
 *                                                                            *
 *                      File Name : Argument.cpp                              *
 *                                                                            *
 *                     Programmer : Tony Skywalker                            *
 *                                                                            *
 *                     Start Date : April 9, 2023                             *
 *                                                                            *
 *                    Last Update :                                           *
 *                                                                            *
 * -------------------------------------------------------------------------- *
 * Over View:                                                                 *
 *   For basic argument parsing.                                              *
 * -------------------------------------------------------------------------- *
 * Build Environment:                                                         *
 *   Windows 11 Pro                                                           *
 *   Visual Studio 2022 Community                                             *
 ******************************************************************************/

#include "Arg.h"

#include <cstdio>
#include <cstring>

static char _opt_buffer[128];
static char _long_opt_buffer[128];

char* optarg;
const char* optmsg;
int opterr;
int optopt;
char* longopt;

static int optind;

void resetopt()
{
    optind = 0;
    opterr = 0;
    optarg = nullptr;
    optmsg = nullptr;
}

// parse short option. e.g. l from -l
static int _parse_opt(const char* arg);

// parse long option. e.g. enable-log from --enable-log
// Such option do not support arguments, so optarg will
// be the option itself, and optopt will be '@'.
static const char* _parse_long_opt(const char* arg);

// parse arg in opt. e.g. pthread from -lpthread
// Must be checked by _parseopt first.
static char* _parse_arg(char* arg);

static void _init_opt();

int getopt(int argc, char* argv[], const char* pattern)
{
    _init_opt();

    if (optind >= argc)
    {
        resetopt();
        return 0;
    }

    int opt = _parse_opt(argv[optind]);
    if (!opt) // not an option
    {
        // check if it is a long option
        auto long_opt = _parse_long_opt(argv[optind]);
        if (long_opt)
        {
            opt = optopt = '@';
            longopt = _long_opt_buffer;
            while (*long_opt && (*long_opt != '='))
            {
                *longopt++ = *long_opt++;
            }
            *longopt = '\0';
            longopt = _long_opt_buffer;
            if (*long_opt)
            {
                optarg = const_cast<char*>(long_opt + 1);
            }
            else
            {
                optarg = const_cast<char*>(long_opt);
            }
        }
        else // not an option
        {
            opt = optopt = '!';
            optarg = argv[optind];
        }
    }
    else // is an option
    {
        optopt = opt;
        auto pos = strchr(pattern, opt);
        if (!pos) // not valid option
        {
            opt = '?';
            opterr = ERRNO_INVALID_OPTION;
            SPRINTF(_opt_buffer, "Illegal argument %c", opt);
        }
        else
        {
            if (pos[1] == ':') // has argument
            {
                optarg = _parse_arg(argv[optind]);
                if (!optarg) // argument not compact
                {
                    if (((optind < argc - 1) && _parse_opt(argv[optind + 1])) ||
                        (optind == argc - 1))
                    {
                        opterr = ERRNO_MISSING_ARGUMENT;
                        SPRINTF(_opt_buffer, "Missing argument for parameter %c", opt);
                        optmsg = _opt_buffer;
                    }
                    else
                    {
                        optarg = argv[++optind];
                    }
                }
            }
        }
    }

    optmsg = _opt_buffer;

    return opt;
}

static int _parse_opt(const char* arg)
{
    if (arg[0] == '-' && arg[1] != '-' && arg[1] != '\0')
    {
        return arg[1];
    }
    return 0;
}

static const char* _parse_long_opt(const char* arg)
{
    if (arg[0] == '-' && arg[1] == '-' && arg[2] != '\0')
    {
        return &arg[2];
    }

    return nullptr;
}

// parse arg in opt. e.g. pthread from -lpthread
// Must checked by _parseopt first.
static char* _parse_arg(char* arg)
{
    if (arg[2] != '\0')
    {
        return &arg[2];
    }
    return nullptr;
}

static void _init_opt()
{
    optarg = nullptr;
    optmsg = nullptr;
    opterr = 0;
    optopt = '?';
    optind++;
}
