/**
 *       @file  eastl_dandy_config.h
 *      @brief
 *
 *    @details
 *
 *    EASTLの設定ファイルです。このファイルは下記ヘッダファ
 *    イルからincludeされ、EASTLのデフォルト設定値を置き換え
 *    ます。
 *
 *    dandy_external/EASTL/include/EASTL/internal/config.h
 *
 *     @author  MaskedW
 *
 *   @internal
 *     Created  2018/10/13
 * ===================================================================
 */

/*
 * License: MIT license
 * Copyright (c) <2018> <MaskedW [maskedw00@gmail.com]>
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

#ifndef eastl_dandy_config_h_
#define eastl_dandy_config_h_

#include <picox/core/xcore.h>

#ifndef EASTL_DEBUG
    #define EASTL_DEBUG 0
#endif

#ifndef EASTL_DEV_DEBUG
    #define EASTL_DEV_DEBUG 0
#endif

#ifndef EASTL_DEBUGPARAMS_LEVEL
    #if EASTL_DEBUG
        #define EASTL_DEBUGPARAMS_LEVEL 2
    #else
        #define EASTL_DEBUGPARAMS_LEVEL 0
    #endif
#endif

#ifndef EASTL_NAME_ENABLED
    #define EASTL_NAME_ENABLED EASTL_DEBUG
#endif

#define EASTL_DEV_ASSERT_ENABLED EASTL_DEV_DEBUG

#define EASTL_ASSERTION_FAILURE_DEFINED

#define EASTL_ASSERT(expr) X_ASSERT(expr)

#define EASTL_DEV_ASSERT(expr) X_ASSERT(expr)

#define EASTL_FAIL_MSG(message) X_ABORT(message)

#define EASTL_DEBUG_BREAK()     X_ABORT("DEBUG_BREAK")

#define EASTL_MAX_STACK_USAGE 512


#endif /* end of include guard: eastl_dandy_config_h_ */
