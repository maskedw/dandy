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


// DShell* self;


int DShellCommand_hello(const DShellCommandContext* ctx)
{
    printf("Hello World\n");
    for (int i = 0; i < ctx->argc; i++)
    {
        printf("arg[%d]: %s\n", i, ctx->argv[i]);
    }
    return 0;
}


// static void completion(const char *buf, linenoiseCompletions *lc) {
//     if (buf[0] == 'h') {
//         linenoiseAddCompletion(lc,"hello");
//         linenoiseAddCompletion(lc,"hello there");
//     }
// }


void DShell::install(const char* name, DShellCommand command)
{
    m_commands.insert(std::make_pair(name, command));
}


void DShell::start()
{
    this->install("hello", callback(DShellCommand_hello));
    linenoiseSetMultiLine(0);
    // self = this;
    // linenoiseSetCompletionCallback(completion);
    char *line;

    while((line = linenoise("hello> ")) != NULL) {
        /* Do something with the string. */
        if (line[0] != '\0' && line[0] != '/') {
            linenoiseHistoryAdd(line); /* Add to the history. */
            // linenoiseHistorySave("history.txt"); /* Save the history on disk. */

            int argc;
            char* argv[16];
            const XArgParserErr err = xargparser_to_argv(line, &argc, argv, 16);
            if (err != X_ARG_PARSER_ERR_NONE)
            {
                X_LOG_WARN(("Shell", "Command parse error: %d", err));
            }
            else
            {
                const char* commandName = argv[0];
                DShellCommandMap::const_iterator item = m_commands.find(commandName);
                if (item == m_commands.end())
                {
                    printf("%s not found\n", commandName);
                }
                else
                {
                    DShellCommandContext ctx;
                    ctx.argc = argc;
                    ctx.argv = argv;
                    const DShellCommand command = item->second;
                    command.call(&ctx);
                }
            }
        } else if (!strncmp(line,"/historylen",11)) {
            /* The "/historylen" command will change the history len. */
            int len = atoi(line+11);
            linenoiseHistorySetMaxLen(len);
        } else if (line[0] == '/') {
            printf("Unreconized command: %s\n", line);
        }
        free(line);
    }
}


void DShell::stop()
{
    X_LOG_WARN(("Shell", "Not implemented yet"));

}
