/**
 *       @file  DShell.cpp
 *      @brief
 *
 *    @details
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2019/05/07
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2019> <MaskedW [maskedw00@gmail.com]>
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include <dandy/shell/DShell.hpp>
#include <linenoise/linenoise.h>
#include <picox/misc/xargparser.h>


class DShellLinenoiseCallbackHelper
{
public:
    static void CompletionCallback(linenoiseState* l, const char* buf, linenoiseCompletions* lc)
    {
        DShell* shell = static_cast<DShell*>(l->userPtr);
        const size_t bufLen = ::strlen(buf);

        if (buf[0] == 'c')
            linenoiseAddCompletion(l, lc, "clear");
        else if (buf[0] == 'e')
            linenoiseAddCompletion(l, lc, "exit");

        D_CONST_FOREACH(DShell::DShellCommandMap::const_iterator, it, shell->m_commands)
        {
            if (::strncmp(buf, it->first.c_str(), bufLen) == 0)
                linenoiseAddCompletion(l, lc, it->first.c_str());
        }
    }

    static char* HintsCallback(linenoiseState* l, const char* buf, int* color, int* bold)
    {
        X_UNUSED(l);
        X_UNUSED(buf);
        X_UNUSED(color);
        X_UNUSED(bold);

        return NULL;
    }

    static void FreeHintsCallback(linenoiseState* l, void* ptr)
    {
        X_UNUSED(l);
        X_UNUSED(ptr);
    }

    static ssize_t ReadCallback(linenoiseState* l, void* dst, size_t n)
    {
        DShell* shell = static_cast<DShell*>(l->userPtr);
        return shell->m_stdIn->read(dst, n);
    }

    static ssize_t WriteCallback(linenoiseState* l, const void* src, size_t n)
    {
        DShell* shell = static_cast<DShell*>(l->userPtr);
        return shell->m_stdOut->write(src, n);
    }
};


int DShellCommand_hello(const DShellCommandContext* ctx)
{
    printf("Hello World\n");
    for (int i = 0; i < ctx->argc; i++)
    {
        printf("arg[%d]: %s\n", i, ctx->argv[i]);
    }
    return 0;
}

DShell::DShell()
    : m_stdOut(nullptr)
    , m_stdIn(nullptr)
    , m_stdErr(nullptr)
    , m_maxHistory(20)
    , m_maxLine(256)
    , m_maxArgc(16)
{
}


void DShell::install(const char* name, DShellCommand command)
{
    m_commands.insert(std::make_pair(name, command));
}


void DShell::start(const char* prompt, DStream* stdOut, DStream* stdIn, DStream* stdErr)
{
    X_ASSERT(stdOut);

    m_stdOut = stdOut;
    m_stdIn = stdIn ? stdIn : stdOut;
    m_stdErr = stdErr ? stdErr : stdOut;

    this->install("hello", callback(DShellCommand_hello));

    linenoiseState l;
    linenoiseInit(&l);
    l.userPtr = this;
    l.write = DShellLinenoiseCallbackHelper::WriteCallback;
    l.read = DShellLinenoiseCallbackHelper::ReadCallback;
    l.max_line = m_maxLine;
    linenoiseSetMultiLine(&l, 0);
    linenoiseSetCompletionCallback(&l, DShellLinenoiseCallbackHelper::CompletionCallback);
    linenoiseSetHintsCallback(&l, DShellLinenoiseCallbackHelper::HintsCallback);
    linenoiseSetFreeHintsCallback(&l, DShellLinenoiseCallbackHelper::FreeHintsCallback);
    linenoiseHistorySetMaxLen(&l, m_maxHistory);

    char *line = nullptr;
    int argc;
    char** argv = D_NEW(char*[m_maxArgc + 1]);
    X_ASSERT(argv);

    for (;;)
    {
        linenoiseFree(&l, line);
        line = nullptr;
        line = linenoise(&l, prompt);
        if (!line)
            break;

        line = x_strrstrip(line, nullptr);
        if (line[0] == '\0')
            continue;

        linenoiseHistoryAdd(&l, line);

        const XArgParserErr err = xargparser_to_argv(line, &argc, argv, m_maxArgc);
        if (err != X_ARG_PARSER_ERR_NONE)
        {
            m_stdErr->printf("dsh: command line parsing failed\n");
            continue;
        }

        argv[argc] = nullptr;
        const char* commandName = argv[0];

        if (::strcmp(commandName, "clear") == 0)
        {
            linenoiseClearScreen(&l);
        }
        else if (::strcmp(commandName, "exit") == 0)
        {
            m_stdErr->printf("dsh: bye\n");
            break;
        }
        else
        {
            DShellCommandMap::const_iterator item = m_commands.find(commandName);
            if (item == m_commands.end())
            {
                m_stdErr->printf("dsh: command not found '%s'\n", commandName);
                continue;
            }

            DShellCommandContext ctx;
            ctx.argc = argc;
            ctx.argv = argv;
            ctx.stdOut = m_stdOut;
            ctx.stdIn = m_stdIn;
            ctx.stdErr = m_stdErr;
            const DShellCommand command = item->second;
            command.call(&ctx);
        }
    }

    D_DELETE_ARRAY(argv);
    linenoiseFreeHistory(&l);
}

void DShell::setMaxHistory(int maxHistory)
{
    m_maxHistory = maxHistory;
}

void DShell::setMaxLine(size_t maxLine)
{
    m_maxLine = maxLine;
}

void DShell::setMaxArgc(size_t maxArgc)
{
    m_maxArgc = maxArgc;
}
