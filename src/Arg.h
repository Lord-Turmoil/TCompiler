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
 *                      File Name : Argument.h                                *
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

#pragma once

#ifndef _ARG_H_
#define _ARG_H_


/*
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
** Basic argument parsing.
**+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

extern char* optarg;
extern const char* optmsg;
extern int opterr;
extern int optopt;
extern char* longopt;

void resetopt();
int getopt(int argc, char* argv[], const char* pattern);

#define ERRNO_INVALID_OPTION   1
#define ERRNO_MISSING_ARGUMENT 2

#define ERRMSG_TOO_MANY "Too many arguments!"
#define ERRMSG_TOO_FEW  "Too few arguments!"
#define ERRMSG_ILLEGAL  "Arguments illegal!"

#ifdef _WIN32
#define SPRINTF sprintf_s
#else
#define SPRINTF sprintf
#endif

#endif
