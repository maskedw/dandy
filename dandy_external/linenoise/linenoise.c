/* linenoise.c -- guerrilla line editing library against the idea that a
 * line editing lib needs to be 20,000 lines of C code.
 *
 * You can find the latest source code at:
 *
 *   http://github.com/antirez/linenoise
 *
 * Does a number of crazy assumptions that happen to be true in 99.9999% of
 * the 2010 UNIX computers around.
 *
 * ------------------------------------------------------------------------
 *
 * Copyright (c) 2010-2016, Salvatore Sanfilippo <antirez at gmail dot com>
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
 *
 * ------------------------------------------------------------------------
 *
 * References:
 * - http://invisible-island.net/xterm/ctlseqs/ctlseqs.html
 * - http://www.3waylabs.com/nw/WWW/products/wizcon/vt220.html
 *
 * Todo list:
 * - Filter bogus Ctrl+<char> combinations.
 * - Win32 support
 *
 * Bloat:
 * - History search like Ctrl+r in readline?
 *
 * List of escape sequences used by this program, we do everything just
 * with three sequences. In order to be so cheap we may have some
 * flickering effect with some slow terminal, but the lesser sequences
 * the more compatible.
 *
 * EL (Erase Line)
 *    Sequence: ESC [ n K
 *    Effect: if n is 0 or missing, clear from cursor to end of line
 *    Effect: if n is 1, clear from beginning of line to cursor
 *    Effect: if n is 2, clear entire line
 *
 * CUF (CUrsor Forward)
 *    Sequence: ESC [ n C
 *    Effect: moves cursor forward n chars
 *
 * CUB (CUrsor Backward)
 *    Sequence: ESC [ n D
 *    Effect: moves cursor backward n chars
 *
 * The following is used to get the terminal width if getting
 * the width with the TIOCGWINSZ ioctl fails
 *
 * DSR (Device Status Report)
 *    Sequence: ESC [ 6 n
 *    Effect: reports the current cusor position as ESC [ n ; m R
 *            where n is the row and m is the column
 *
 * When multi line mode is enabled, we also use an additional escape
 * sequence. However multi line editing is disabled by default.
 *
 * CUU (Cursor Up)
 *    Sequence: ESC [ n A
 *    Effect: moves cursor up of n chars.
 *
 * CUD (Cursor Down)
 *    Sequence: ESC [ n B
 *    Effect: moves cursor down of n chars.
 *
 * When linenoiseClearScreen() is called, two additional escape sequences
 * are used in order to clear the screen and position the cursor at home
 * position.
 *
 * CUP (Cursor position)
 *    Sequence: ESC [ H
 *    Effect: moves the cursor to upper left corner
 *
 * ED (Erase display)
 *    Sequence: ESC [ 2 J
 *    Effect: clear the whole screen
 *
 */

#include <picox/core/xcore.h>
#ifndef __MBED__
    #include <termios.h>
#endif
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "linenoise.h"

struct winsize
{
    int ws_col;
};

enum KEY_ACTION{
	KEY_NULL = 0,	    /* NULL */
	CTRL_A = 1,         /* Ctrl+a */
	CTRL_B = 2,         /* Ctrl-b */
	CTRL_C = 3,         /* Ctrl-c */
	CTRL_D = 4,         /* Ctrl-d */
	CTRL_E = 5,         /* Ctrl-e */
	CTRL_F = 6,         /* Ctrl-f */
	CTRL_H = 8,         /* Ctrl-h */
	TAB = 9,            /* Tab */
	CTRL_K = 11,        /* Ctrl+k */
	CTRL_L = 12,        /* Ctrl+l */
	KEY_LF = 10,
	KEY_CR  = 13,
	CTRL_N = 14,        /* Ctrl-n */
	CTRL_P = 16,        /* Ctrl-p */
	CTRL_T = 20,        /* Ctrl-t */
	CTRL_U = 21,        /* Ctrl+u */
	CTRL_W = 23,        /* Ctrl+w */
	ESC = 27,           /* Escape */
	BACKSPACE =  127    /* Backspace */
};

static void refreshLine(struct linenoiseState *self);

/* Debugging macro. */
#if 0
FILE *lndebug_fp = NULL;
#define lndebug(...) \
    do { \
        if (lndebug_fp == NULL) { \
            lndebug_fp = fopen("/tmp/lndebug.txt","a"); \
            fprintf(lndebug_fp, \
            "[%d %d %d] p: %d, rows: %d, rpos: %d, max: %d, oldmax: %d\n", \
            (int)self->len,(int)self->pos,(int)self->oldpos,plen,rows,rpos, \
            (int)self->maxrows,old_rows); \
        } \
        fprintf(lndebug_fp, ", " __VA_ARGS__); \
        fflush(lndebug_fp); \
    } while (0)
#else
#define lndebug(fmt, ...)
#endif

/* ======================= Low level terminal handling ====================== */

/* Set if to use or not the multi line mode. */
void linenoiseSetMultiLine(linenoiseState* self, int ml) {
    self->mlmode = ml;
}

// /* Return true if the terminal name is in the list of terminals we know are
//  * not able to understand basic escape sequences. */
// static int isUnsupportedTerm(void) {
//     char *term = getenv("TERM");
//     int j;
//
//     if (term == NULL) return 0;
//     for (j = 0; unsupported_term[j]; j++)
//         if (!strcasecmp(term,unsupported_term[j])) return 1;
//     return 0;
// }

/* Use the ESC [6n escape sequence to query the horizontal cursor position
 * and return it. On error -1 is returned, on success the position of the
 * cursor. */
// static int getCursorPosition(linenoiseState* self, int ifd, int ofd) {
static int getCursorPosition(linenoiseState* self) {
    char buf[32];
    int cols, rows;
    unsigned int i = 0;

    /* Report cursor location */
    if (self->write(self, "\x1b[6n", 4) != 4) return -1;

    /* Read the response: ESC [ rows ; cols R */
    while (i < sizeof(buf)-1) {
        if (self->read(self, buf+i,1) != 1) break;
        if (buf[i] == 'R') break;
        i++;
    }
    buf[i] = '\0';

    /* Parse it. */
    if (buf[0] != ESC || buf[1] != '[') return -1;
    if (sscanf(buf+2,"%d;%d",&rows,&cols) != 2) return -1;
    return cols;
}

/* Try to get the number of columns in the current terminal, or assume 80
 * if it fails. */
static int getColumns(linenoiseState* self) {
    struct winsize ws;
    ws.ws_col = 100;

#ifdef __MBED__
    if (1) {
#else
    if (ioctl(1, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
#endif
        /* ioctl() failed. Try to query the terminal itself. */
        int start, cols;

        /* Get the initial position so we can restore it later. */
        start = getCursorPosition(self);
        if (start == -1) goto failed;

        /* Go to right margin and get position. */
        if (self->write(self,"\x1b[999C",6) != 6) goto failed;
        cols = getCursorPosition(self);
        if (cols == -1) goto failed;

        /* Restore position. */
        if (cols > start) {
            char seq[32];
            snprintf(seq,32,"\x1b[%dD",cols-start);
            if (self->write(self,seq,strlen(seq)) == -1) {
                /* Can't recover... */
            }
        }
        return cols;
    } else {
        return ws.ws_col;
    }

failed:
    return 80;
}

/* Clear the screen. Used to handle ctrl+l */
void linenoiseClearScreen(linenoiseState* self) {
    if (self->write(self,"\x1b[H\x1b[2J",7) <= 0) {
        /* nothing to do, just to avoid warning. */
    }
}

/* Beep, used for completion when there is nothing to complete or when all
 * the choices were already shown. */
static void linenoiseBeep(void) {
    // fprintf(stderr, "\x7");
    // fflush(stderr);
}

/* ============================== Completion ================================ */

/* Free a list of completion option populated by linenoiseAddCompletion(). */
static void freeCompletions(linenoiseCompletions *lc) {
    size_t i;
    for (i = 0; i < lc->len; i++)
        free(lc->cvec[i]);
    if (lc->cvec != NULL)
        free(lc->cvec);
}

/* This is an helper function for linenoiseEdit() and is called when the
 * user types the <tab> key in order to complete the string currently in the
 * input.
 *
 * The state of the editing is encapsulated into the pointed linenoiseState
 * structure as described in the structure definition. */
static int completeLine(struct linenoiseState *self) {
    linenoiseCompletions lc = { 0, NULL };
    int nread, nwritten;
    char c = 0;

    self->completionCallback(self, self->buf,&lc);
    if (lc.len == 0) {
        linenoiseBeep();
    } else {
        size_t stop = 0, i = 0;

        while(!stop) {
            /* Show completion or original buffer */
            if (i < lc.len) {
                struct linenoiseState saved = *self;

                self->len = self->pos = strlen(lc.cvec[i]);
                self->buf = lc.cvec[i];
                refreshLine(self);
                self->len = saved.len;
                self->pos = saved.pos;
                self->buf = saved.buf;
            } else {
                refreshLine(self);
            }

            nread = self->read(self,&c,1);
            if (nread <= 0) {
                freeCompletions(&lc);
                return -1;
            }

            switch(c) {
                case 9: /* tab */
                    i = (i+1) % (lc.len+1);
                    if (i == lc.len) linenoiseBeep();
                    break;
                case 27: /* escape */
                    /* Re-show original buffer */
                    if (i < lc.len) refreshLine(self);
                    stop = 1;
                    break;
                default:
                    /* Update buffer and return */
                    if (i < lc.len) {
                        nwritten = snprintf(self->buf,self->buflen,"%s",lc.cvec[i]);
                        self->len = self->pos = nwritten;
                    }
                    stop = 1;
                    break;
            }
        }
    }

    freeCompletions(&lc);
    return c; /* Return last read character */
}

/* Register a callback function to be called for tab-completion. */
void linenoiseSetCompletionCallback(linenoiseState* self, linenoiseCompletionCallback *fn) {
    self->completionCallback = fn;
}

/* Register a hits function to be called to show hits to the user at the
 * right of the prompt. */
void linenoiseSetHintsCallback(linenoiseState* self, linenoiseHintsCallback *fn) {
    self->hintsCallback = fn;
}

/* Register a function to free the hints returned by the hints callback
 * registered with linenoiseSetHintsCallback(). */
void linenoiseSetFreeHintsCallback(linenoiseState* self, linenoiseFreeHintsCallback *fn) {
    self->freeHintsCallback = fn;
}

/* This function is used by the callback function registered by the user
 * in order to add completion options given the input string when the
 * user typed <tab>. See the example.c source code for a very easy to
 * understand example. */
void linenoiseAddCompletion(linenoiseState* self, linenoiseCompletions *lc, const char *str) {
    size_t len = strlen(str);
    char *copy, **cvec;

    copy = malloc(len+1);
    if (copy == NULL) return;
    memcpy(copy,str,len+1);
    cvec = realloc(lc->cvec,sizeof(char*)*(lc->len+1));
    if (cvec == NULL) {
        free(copy);
        return;
    }
    lc->cvec = cvec;
    lc->cvec[lc->len++] = copy;
}

/* =========================== Line editing ================================= */

/* We define a very simple "append buffer" structure, that is an heap
 * allocated string where we can append to. This is useful in order to
 * write all the escape sequences in a buffer and flush them to the standard
 * output in a single call, to avoid flickering effects. */
struct abuf {
    char *b;
    int len;
};

static void abInit(struct abuf *ab) {
    ab->b = NULL;
    ab->len = 0;
}

static void abAppend(struct abuf *ab, const char *s, int len) {
    char *new = realloc(ab->b,ab->len+len);

    if (new == NULL) return;
    memcpy(new+ab->len,s,len);
    ab->b = new;
    ab->len += len;
}

static void abFree(struct abuf *ab) {
    free(ab->b);
}

/* Helper of refreshSingleLine() and refreshMultiLine() to show hints
 * to the right of the prompt. */
void refreshShowHints(struct abuf *ab, struct linenoiseState *self, int plen) {
    char seq[64];
    if (self->hintsCallback && plen+self->len < self->cols) {
        int color = -1, bold = 0;
        char *hint = self->hintsCallback(self, self->buf,&color,&bold);
        if (hint) {
            int hintlen = strlen(hint);
            int hintmaxlen = self->cols-(plen+self->len);
            if (hintlen > hintmaxlen) hintlen = hintmaxlen;
            if (bold == 1 && color == -1) color = 37;
            if (color != -1 || bold != 0)
                snprintf(seq,64,"\033[%d;%d;49m",bold,color);
            else
                seq[0] = '\0';
            abAppend(ab,seq,strlen(seq));
            abAppend(ab,hint,hintlen);
            if (color != -1 || bold != 0)
                abAppend(ab,"\033[0m",4);
            /* Call the function to free the hint returned. */
            if (self->freeHintsCallback) self->freeHintsCallback(self, hint);
        }
    }
}

/* Single line low level line refresh.
 *
 * Rewrite the currently edited line accordingly to the buffer content,
 * cursor position, and number of columns of the terminal. */
static void refreshSingleLine(struct linenoiseState *self) {
    char seq[64];
    size_t plen = strlen(self->prompt);
    // int fd = self->ofd;
    char *buf = self->buf;
    size_t len = self->len;
    size_t pos = self->pos;
    struct abuf ab;

    while((plen+pos) >= self->cols) {
        buf++;
        len--;
        pos--;
    }
    while (plen+len > self->cols) {
        len--;
    }

    abInit(&ab);
    /* Cursor to left edge */
    snprintf(seq,64,"\r");
    abAppend(&ab,seq,strlen(seq));
    /* Write the prompt and the current buffer content */
    abAppend(&ab,self->prompt,strlen(self->prompt));
    abAppend(&ab,buf,len);
    /* Show hits if any. */
    refreshShowHints(&ab,self,plen);
    /* Erase to right */
    snprintf(seq,64,"\x1b[0K");
    abAppend(&ab,seq,strlen(seq));
    /* Move cursor to original position. */
    snprintf(seq,64,"\r\x1b[%dC", (int)(pos+plen));
    abAppend(&ab,seq,strlen(seq));
    if (self->write(self,ab.b,ab.len) == -1) {} /* Can't recover from write error. */
    abFree(&ab);
}

/* Multi line low level line refresh.
 *
 * Rewrite the currently edited line accordingly to the buffer content,
 * cursor position, and number of columns of the terminal. */
static void refreshMultiLine(struct linenoiseState *self) {
    char seq[64];
    int plen = strlen(self->prompt);
    int rows = (plen+self->len+self->cols-1)/self->cols; /* rows used by current buf. */
    int rpos = (plen+self->oldpos+self->cols)/self->cols; /* cursor relative row. */
    int rpos2; /* rpos after refresh. */
    int col; /* colum position, zero-based. */
    int old_rows = self->maxrows;
    // int fd = self->ofd, j;
    int j;
    struct abuf ab;

    /* Update maxrows if needed. */
    if (rows > (int)self->maxrows) self->maxrows = rows;

    /* First step: clear all the lines used before. To do so start by
     * going to the last row. */
    abInit(&ab);
    if (old_rows-rpos > 0) {
        lndebug("go down %d", old_rows-rpos);
        snprintf(seq,64,"\x1b[%dB", old_rows-rpos);
        abAppend(&ab,seq,strlen(seq));
    }

    /* Now for every row clear it, go up. */
    for (j = 0; j < old_rows-1; j++) {
        lndebug("clear+up");
        snprintf(seq,64,"\r\x1b[0K\x1b[1A");
        abAppend(&ab,seq,strlen(seq));
    }

    /* Clean the top line. */
    lndebug("clear");
    snprintf(seq,64,"\r\x1b[0K");
    abAppend(&ab,seq,strlen(seq));

    /* Write the prompt and the current buffer content */
    abAppend(&ab,self->prompt,strlen(self->prompt));
    abAppend(&ab,self->buf,self->len);

    /* Show hits if any. */
    refreshShowHints(&ab,self,plen);

    /* If we are at the very end of the screen with our prompt, we need to
     * emit a newline and move the prompt to the first column. */
    if (self->pos &&
        self->pos == self->len &&
        (self->pos+plen) % self->cols == 0)
    {
        lndebug("<newline>");
        abAppend(&ab,"\n",1);
        snprintf(seq,64,"\r");
        abAppend(&ab,seq,strlen(seq));
        rows++;
        if (rows > (int)self->maxrows) self->maxrows = rows;
    }

    /* Move cursor to right position. */
    rpos2 = (plen+self->pos+self->cols)/self->cols; /* current cursor relative row. */
    lndebug("rpos2 %d", rpos2);

    /* Go up till we reach the expected positon. */
    if (rows-rpos2 > 0) {
        lndebug("go-up %d", rows-rpos2);
        snprintf(seq,64,"\x1b[%dA", rows-rpos2);
        abAppend(&ab,seq,strlen(seq));
    }

    /* Set column. */
    col = (plen+(int)self->pos) % (int)self->cols;
    lndebug("set col %d", 1+col);
    if (col)
        snprintf(seq,64,"\r\x1b[%dC", col);
    else
        snprintf(seq,64,"\r");
    abAppend(&ab,seq,strlen(seq));

    lndebug("\n");
    self->oldpos = self->pos;

    if (self->write(self,ab.b,ab.len) == -1) {} /* Can't recover from write error. */
    abFree(&ab);
}

/* Calls the two low level functions refreshSingleLine() or
 * refreshMultiLine() according to the selected mode. */
static void refreshLine(struct linenoiseState *self) {
    if (self->mlmode)
        refreshMultiLine(self);
    else
        refreshSingleLine(self);
}

/* Insert the character 'c' at cursor current position.
 *
 * On error writing to the terminal -1 is returned, otherwise 0. */
int linenoiseEditInsert(struct linenoiseState *self, char c) {
    if (self->len < self->buflen) {
        if (self->len == self->pos) {
            self->buf[self->pos] = c;
            self->pos++;
            self->len++;
            self->buf[self->len] = '\0';
            if ((!self->mlmode && self->plen+self->len < self->cols && !self->hintsCallback)) {
                /* Avoid a full update of the line in the
                 * trivial case. */
                if (self->write(self,&c,1) == -1) return -1;
            } else {
                refreshLine(self);
            }
        } else {
            memmove(self->buf+self->pos+1,self->buf+self->pos,self->len-self->pos);
            self->buf[self->pos] = c;
            self->len++;
            self->pos++;
            self->buf[self->len] = '\0';
            refreshLine(self);
        }
    }
    return 0;
}

/* Move cursor on the left. */
void linenoiseEditMoveLeft(struct linenoiseState *self) {
    if (self->pos > 0) {
        self->pos--;
        refreshLine(self);
    }
}

/* Move cursor on the right. */
void linenoiseEditMoveRight(struct linenoiseState *self) {
    if (self->pos != self->len) {
        self->pos++;
        refreshLine(self);
    }
}

/* Move cursor to the start of the line. */
void linenoiseEditMoveHome(struct linenoiseState *self) {
    if (self->pos != 0) {
        self->pos = 0;
        refreshLine(self);
    }
}

/* Move cursor to the end of the line. */
void linenoiseEditMoveEnd(struct linenoiseState *self) {
    if (self->pos != self->len) {
        self->pos = self->len;
        refreshLine(self);
    }
}

/* Substitute the currently edited line with the next or previous history
 * entry as specified by 'dir'. */
#define LINENOISE_HISTORY_NEXT 0
#define LINENOISE_HISTORY_PREV 1
void linenoiseEditHistoryNext(struct linenoiseState *self, int dir) {
    if (self->history_len > 1) {
        /* Update the current history entry before to
         * overwrite it with the next one. */
        free(self->history[self->history_len - 1 - self->history_index]);
        self->history[self->history_len - 1 - self->history_index] = strdup(self->buf);
        /* Show the new entry */
        self->history_index += (dir == LINENOISE_HISTORY_PREV) ? 1 : -1;
        if (self->history_index < 0) {
            self->history_index = 0;
            return;
        } else if (self->history_index >= self->history_len) {
            self->history_index = self->history_len-1;
            return;
        }
        strncpy(self->buf,self->history[self->history_len - 1 - self->history_index],self->buflen);
        self->buf[self->buflen-1] = '\0';
        self->len = self->pos = strlen(self->buf);
        refreshLine(self);
    }
}

/* Delete the character at the right of the cursor without altering the cursor
 * position. Basically this is what happens with the "Delete" keyboard key. */
void linenoiseEditDelete(struct linenoiseState *self) {
    if (self->len > 0 && self->pos < self->len) {
        memmove(self->buf+self->pos,self->buf+self->pos+1,self->len-self->pos-1);
        self->len--;
        self->buf[self->len] = '\0';
        refreshLine(self);
    }
}

/* Backspace implementation. */
void linenoiseEditBackspace(struct linenoiseState *self) {
    if (self->pos > 0 && self->len > 0) {
        memmove(self->buf+self->pos-1,self->buf+self->pos,self->len-self->pos);
        self->pos--;
        self->len--;
        self->buf[self->len] = '\0';
        refreshLine(self);
    }
}

/* Delete the previosu word, maintaining the cursor at the start of the
 * current word. */
void linenoiseEditDeletePrevWord(struct linenoiseState *self) {
    size_t old_pos = self->pos;
    size_t diff;

    while (self->pos > 0 && self->buf[self->pos-1] == ' ')
        self->pos--;
    while (self->pos > 0 && self->buf[self->pos-1] != ' ')
        self->pos--;
    diff = old_pos - self->pos;
    memmove(self->buf+self->pos,self->buf+old_pos,self->len-old_pos+1);
    self->len -= diff;
    refreshLine(self);
}

/* This function is the core of the line editing capability of linenoise.
 * It expects 'fd' to be already in "raw mode" so that every key pressed
 * will be returned ASAP to read().
 *
 * The resulting string is put into 'buf' when the user type enter, or
 * when ctrl+d is typed.
 *
 * The function returns the length of the current buffer. */
static int linenoiseEdit(linenoiseState* self, char *buf, size_t buflen, const char *prompt)
{
    // struct linenoiseState l;
    /* Populate the linenoise state that we pass to functions implementing
     * specific editing functionalities. */
    self->buf = buf;
    self->buflen = buflen;
    self->prompt = prompt;
    self->plen = strlen(prompt);
    self->oldpos = self->pos = 0;
    self->len = 0;
    self->cols = getColumns(self);
    self->maxrows = 0;
    self->history_index = 0;

    /* Buffer starts empty. */
    self->buf[0] = '\0';
    self->buflen--; /* Make sure there is always space for the nulterm */

    /* The latest history entry is always our current buffer, that
     * initially is just an empty string. */
    linenoiseHistoryAdd(self, "");

    if (self->write(self,prompt,self->plen) == -1) return -1;
    while(1) {
        uint8_t b;
        int c;
        int nread;
        char seq[3];

        nread = self->read(self,&b,1);
        c = b;
        if (nread <= 0) return self->len;

        /* Only autocomplete when the callback is set. It returns < 0 when
         * there was an error reading from fd. Otherwise it will return the
         * character that should be handled next. */
        if (c == 9 && self->completionCallback != NULL) {
            c = completeLine(self);
            // /* Return on errors */
            if (c < 0) return self->len;
            /* Read next character when 0 */
            if (c == 0) continue;
        }

        switch(c) {
        case KEY_CR:
            continue;
        case KEY_LF:    /* enter */
            self->history_len--;
            free(self->history[self->history_len]);
            if (self->mlmode) linenoiseEditMoveEnd(self);
            if (self->hintsCallback) {
                /* Force a refresh without hints to leave the previous
                 * line as the user typed it after a newline. */
                linenoiseHintsCallback *hc = self->hintsCallback;
                self->hintsCallback = NULL;
                refreshLine(self);
                self->hintsCallback = hc;
            }
            return (int)self->len;
        case CTRL_C:     /* ctrl-c */
            errno = EAGAIN;
            return -1;
        case BACKSPACE:   /* backspace */
        case 8:     /* ctrl-h */
            linenoiseEditBackspace(self);
            break;
        case CTRL_D:     /* ctrl-d, remove char at right of cursor, or if the
                            line is empty, act as end-of-file. */
            if (self->len > 0) {
                linenoiseEditDelete(self);
            } else {
                self->history_len--;
                free(self->history[self->history_len]);
                return -1;
            }
            break;
        case CTRL_T:    /* ctrl-t, swaps current character with previous. */
            if (self->pos > 0 && self->pos < self->len) {
                int aux = buf[self->pos-1];
                buf[self->pos-1] = buf[self->pos];
                buf[self->pos] = aux;
                if (self->pos != self->len-1) self->pos++;
                refreshLine(self);
            }
            break;
        case CTRL_B:     /* ctrl-b */
            linenoiseEditMoveLeft(self);
            break;
        case CTRL_F:     /* ctrl-f */
            linenoiseEditMoveRight(self);
            break;
        case CTRL_P:    /* ctrl-p */
            linenoiseEditHistoryNext(self, LINENOISE_HISTORY_PREV);
            break;
        case CTRL_N:    /* ctrl-n */
            linenoiseEditHistoryNext(self, LINENOISE_HISTORY_NEXT);
            break;
        case ESC:    /* escape sequence */
            /* Read the next two bytes representing the escape sequence.
             * Use two calls to handle slow terminals returning the two
             * chars at different times. */
            if (self->read(self,seq,1) == -1) break;
            if (self->read(self,seq+1,1) == -1) break;

            /* ESC [ sequences. */
            if (seq[0] == '[') {
                if (seq[1] >= '0' && seq[1] <= '9') {
                    /* Extended escape, read additional byte. */
                    if (self->read(self,seq+2,1) == -1) break;
                    if (seq[2] == '~') {
                        switch(seq[1]) {
                        case '3': /* Delete key. */
                            linenoiseEditDelete(self);
                            break;
                        }
                    }
                } else {
                    switch(seq[1]) {
                    case 'A': /* Up */
                        linenoiseEditHistoryNext(self, LINENOISE_HISTORY_PREV);
                        break;
                    case 'B': /* Down */
                        linenoiseEditHistoryNext(self, LINENOISE_HISTORY_NEXT);
                        break;
                    case 'C': /* Right */
                        linenoiseEditMoveRight(self);
                        break;
                    case 'D': /* Left */
                        linenoiseEditMoveLeft(self);
                        break;
                    case 'H': /* Home */
                        linenoiseEditMoveHome(self);
                        break;
                    case 'F': /* End*/
                        linenoiseEditMoveEnd(self);
                        break;
                    }
                }
            }

            /* ESC O sequences. */
            else if (seq[0] == 'O') {
                switch(seq[1]) {
                case 'H': /* Home */
                    linenoiseEditMoveHome(self);
                    break;
                case 'F': /* End*/
                    linenoiseEditMoveEnd(self);
                    break;
                }
            }
            break;
        default:
            if (linenoiseEditInsert(self,c)) return -1;
            break;
        case CTRL_U: /* Ctrl+u, delete the whole line. */
            buf[0] = '\0';
            self->pos = self->len = 0;
            refreshLine(self);
            break;
        case CTRL_K: /* Ctrl+k, delete from current to end of line. */
            buf[self->pos] = '\0';
            self->len = self->pos;
            refreshLine(self);
            break;
        case CTRL_A: /* Ctrl+a, go to the start of the line */
            linenoiseEditMoveHome(self);
            break;
        case CTRL_E: /* ctrl+e, go to the end of the line */
            linenoiseEditMoveEnd(self);
            break;
        case CTRL_L: /* ctrl+l, clear screen */
            linenoiseClearScreen(self);
            refreshLine(self);
            break;
        case CTRL_W: /* ctrl+w, delete previous word */
            linenoiseEditDeletePrevWord(self);
            break;
        }
    }
    return self->len;
}

/* This function calls the line editing function linenoiseEdit() using
 * the STDIN file descriptor set in raw mode. */
static int linenoiseRaw(linenoiseState* self, char *buf, size_t buflen, const char *prompt) {
    int count;

    if (buflen == 0) {
        errno = EINVAL;
        return -1;
    }

    count = linenoiseEdit(self, buf, buflen, prompt);
    self->write(self, "\n", 1);
    return count;
}

void linenoiseInit(linenoiseState* self)
{
    self->write = NULL;
    self->read = NULL;
    self->userPtr = NULL;
    self->mlmode = 0;
    self->history_max_len = 0;
    self->history_len = 0;
    self->history = NULL;
    self->completionCallback = NULL;
    self->hintsCallback = NULL;
    self->freeHintsCallback = NULL;
    self->max_line = 256;
}

/* The high level function that is the main API of the linenoise library.
 * This function checks if the terminal has basic capabilities, just checking
 * for a blacklist of stupid terminals, and later either calls the line
 * editing function or uses dummy fgets() so that you will be able to type
 * something even in the most desperate of the conditions. */
char *linenoise(linenoiseState* self, const char *prompt) {
    char* buf = NULL;
    char* result = NULL;
    int count;

    X_ASSERT(self->write);
    X_ASSERT(self->read);
    buf = malloc(self->max_line);
    X_ASSERT(buf);

    count = linenoiseRaw(self, buf, self->max_line, prompt);

    if (count == -1) {
        free(buf);
        return NULL;
    }

    result = strdup(buf);
    free(buf);

    return result;
}

/* This is just a wrapper the user may want to call in order to make sure
 * the linenoise returned buffer is freed with the same allocator it was
 * created with. Useful when the main program is using an alternative
 * allocator. */
void linenoiseFree(linenoiseState* self, void *ptr) {
    free(ptr);
}

/* ================================ History ================================= */

/* Free the history, but does not reset it. Only used when we have to
 * exit() to avoid memory leaks are reported by valgrind & co. */
void linenoiseFreeHistory(linenoiseState* self) {
    if (self->history) {
        int j;

        for (j = 0; j < self->history_len; j++)
        {
            free(self->history[j]);
            self->history[j] = NULL;
        }
        free(self->history);
        self->history = NULL;
    }
}

/* This is the API call to add a new entry in the linenoise history.
 * It uses a fixed array of char pointers that are shifted (memmoved)
 * when the history max length is reached in order to remove the older
 * entry and make room for the new one, so it is not exactly suitable for huge
 * histories, but will work well for a few hundred of entries.
 *
 * Using a circular buffer is smarter, but a bit more complex to handle. */
int linenoiseHistoryAdd(linenoiseState* self, const char *line) {
    char *linecopy;

    if (self->history_max_len == 0) return 0;

    /* Initialization on first call. */
    if (self->history == NULL) {
        self->history = malloc(sizeof(char*)* self->history_max_len);
        if (self->history == NULL) return 0;
        memset(self->history,0,(sizeof(char*)* self->history_max_len));
    }

    /* Don't add duplicated lines. */
    if (self->history_len && !strcmp(self->history[self->history_len-1], line)) return 0;

    /* Add an heap allocated copy of the line in the history.
     * If we reached the max length, remove the older line. */
    linecopy = strdup(line);
    if (!linecopy) return 0;
    if (self->history_len == self->history_max_len) {
        free(self->history[0]);
        memmove(self->history,self->history+1,sizeof(char*)*(self->history_max_len-1));
        self->history_len--;
    }
    self->history[self->history_len] = linecopy;
    self->history_len++;
    return 1;
}

/* Set the maximum length for the history. This function can be called even
 * if there is already some history, the function will make sure to retain
 * just the latest 'len' elements if the new history length value is smaller
 * than the amount of items already inside the history. */
int linenoiseHistorySetMaxLen(linenoiseState* self, int len) {
    char **new;

    if (len < 1) return 0;
    if (self->history) {
        int tocopy = self->history_len;

        new = malloc(sizeof(char*)*len);
        if (new == NULL) return 0;

        /* If we can't copy everything, free the elements we'll not use. */
        if (len < tocopy) {
            int j;

            for (j = 0; j < tocopy-len; j++) free(self->history[j]);
            tocopy = len;
        }
        memset(new,0,sizeof(char*)*len);
        memcpy(new,self->history+(self->history_len-tocopy), sizeof(char*)*tocopy);
        free(self->history);
        self->history = new;
    }
    self->history_max_len = len;
    if (self->history_len > self->history_max_len)
        self->history_len = self->history_max_len;
    return 1;
}

/* Save the history in the specified file. On success 0 is returned
 * otherwise -1 is returned. */
int linenoiseHistorySave(linenoiseState* self, const char *filename) {
    // mode_t old_umask = umask(S_IXUSR|S_IRWXG|S_IRWXO);
    FILE *fp;
    int j;

    fp = fopen(filename,"w");
#ifndef __MBED__
    umask(old_umask);
#endif
    if (fp == NULL) return -1;
#ifndef __MBED__
    chmod(filename,S_IRUSR|S_IWUSR);
#endif
    for (j = 0; j < self->history_len; j++)
        fprintf(fp,"%s\n",self->history[j]);
    fclose(fp);
    return 0;
}

/* Load the history from the specified file. If the file does not exist
 * zero is returned and no operation is performed.
 *
 * If the file exists and the operation succeeded 0 is returned, otherwise
 * on error -1 is returned. */
int linenoiseHistoryLoad(linenoiseState* self, const char *filename) {
    FILE *fp = fopen(filename,"r");
    // char buf[LINENOISE_MAX_LINE];
    char* buf = NULL;

    if (fp == NULL) return -1;
    buf = malloc(self->max_line);
    X_ASSERT(buf);

    while (fgets(buf,self->max_line,fp) != NULL) {
        char *p;

        p = strchr(buf,'\r');
        if (!p) p = strchr(buf,'\n');
        if (p) *p = '\0';
        linenoiseHistoryAdd(self, buf);
    }
    free(buf);
    fclose(fp);
    return 0;
}
