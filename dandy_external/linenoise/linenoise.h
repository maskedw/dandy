/* linenoise.h -- VERSION 1.0
 *
 * Guerrilla line editing library against the idea that a line editing lib
 * needs to be 20,000 lines of C code.
 *
 * See linenoise.c for more information.
 *
 * ------------------------------------------------------------------------
 *
 * Copyright (c) 2010-2014, Salvatore Sanfilippo <antirez at gmail dot com>
 * Copyright (c) 2010-2013, Pieter Noordhuis <pcnoordhuis at gmail dot com>
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *  *  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  *  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __LINENOISE_H
#define __LINENOISE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct linenoiseCompletions {
  size_t len;
  char **cvec;
} linenoiseCompletions;

struct linenoiseState;

typedef ssize_t (*linenoiseWriteFunc)(struct linenoiseState*, const void*, size_t);
typedef ssize_t (*linenoiseReadFunc)(struct linenoiseState*, void*, size_t);
typedef void(linenoiseCompletionCallback)(struct linenoiseState*, const char *, linenoiseCompletions *);
typedef char*(linenoiseHintsCallback)(struct linenoiseState*, const char *, int *color, int *bold);
typedef void(linenoiseFreeHintsCallback)(struct linenoiseState*, void *);


/* The linenoiseState structure represents the state during line editing.
 * We pass this state to functions implementing specific editing
 * functionalities. */
struct linenoiseState {
    linenoiseWriteFunc write;
    linenoiseReadFunc read;
    void* userPtr;
    char *buf;          /* Edited line buffer. */
    size_t buflen;      /* Edited line buffer size. */
    const char *prompt; /* Prompt to display. */
    size_t plen;        /* Prompt length. */
    size_t pos;         /* Current cursor position. */
    size_t oldpos;      /* Previous refresh cursor position. */
    size_t len;         /* Current edited line length. */
    size_t cols;        /* Number of columns in terminal. */
    size_t maxrows;     /* Maximum num of rows used so far (multiline mode) */
    int history_index;  /* The history index we are currently editing. */
    int mlmode;
    int history_max_len;
    int history_len;
    char **history;
    linenoiseCompletionCallback* completionCallback;
    linenoiseHintsCallback* hintsCallback;
    linenoiseFreeHintsCallback* freeHintsCallback;
    size_t max_line;
};
typedef struct linenoiseState linenoiseState;


void linenoiseSetCompletionCallback(linenoiseState* self, linenoiseCompletionCallback *);
void linenoiseSetHintsCallback(linenoiseState* self, linenoiseHintsCallback *);
void linenoiseSetFreeHintsCallback(linenoiseState* self, linenoiseFreeHintsCallback *);
void linenoiseAddCompletion(linenoiseState* self, linenoiseCompletions *, const char *);

void linenoiseInit(linenoiseState* self);
char *linenoise(linenoiseState* self, const char *prompt);
void linenoiseFree(linenoiseState* self, void *ptr);
int linenoiseHistoryAdd(linenoiseState* self, const char *line);
int linenoiseHistorySetMaxLen(linenoiseState* self, int len);
int linenoiseHistorySave(linenoiseState* self, const char *filename);
int linenoiseHistoryLoad(linenoiseState* self, const char *filename);
void linenoiseClearScreen(linenoiseState* self);
void linenoiseSetMultiLine(linenoiseState* self, int ml);
void linenoiseFreeHistory(linenoiseState* self);


#ifdef __cplusplus
}
#endif

#endif /* __LINENOISE_H */
