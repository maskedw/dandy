/**
 *       @file  DShell.hpp
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


#ifndef dandy_DShell_hpp_
#define dandy_DShell_hpp_


#include <dandy/core/stream/DStream.hpp>
#include <map>


struct DShellCommandContext
{
    int         argc;
    char**      argv;
    DStream*    stdOut;
    DStream*    stdErr;
    DStream*    stdIn;
};

typedef Callback<int (const DShellCommandContext*)> DShellCommand;

class DShell
{
public:
    DShell();
    void install(const char* name, DShellCommand command);
    void installBuiltinCommands();
    void start(const char* prompt, DStream* stdOut, DStream* stdIn = nullptr, DStream* stdErr = nullptr);
    void setMaxHistory(int maxHistory);
    void setMaxLine(size_t maxLine);
    void setMaxArgc(size_t maxArgc);

private:
    typedef std::map<std::string, DShellCommand> DShellCommandMap;

    DShellCommandMap m_commands;
    DStream* m_stdOut;
    DStream* m_stdIn;
    DStream* m_stdErr;
    int m_maxHistory;
    size_t m_maxLine;
    size_t m_maxArgc;

    friend class DShellLinenoiseCallbackHelper;

};

#endif /* end of include guard: dandy_DShell_hpp_ */
