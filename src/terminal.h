/*
 * terminal.h
 * Textsweeper Source Code
 * Available on Github
 *
 * Copyright (C) 2017 Karol "digitcrusher" Łacina
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef TERMINAL_H
#define TERMINAL_H
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <src/utils.h>

#define TERMINAL_GET_IBUFF_CHAR(t, x, y) ( *((t)->ibuff+(x)+(y)*(t)->buffw) )
#define TERMINAL_GET_OBUFF_CHAR(t, x, y) ( *((t)->obuff+(x)+(y)*(t)->buffw) )
#define TERMINAL_GET_CURR_IBUFF_CHAR(t) ( TERMINAL_GET_IBUFF_CHAR((t), (t)->icurx, (t)->icury) )
#define TERMINAL_GET_CURR_OBUFF_CHAR(t) ( TERMINAL_GET_OBUFF_CHAR((t), (t)->ocurx, (t)->ocury) )
#define TERMINAL_GET_BUFF_BYTES(t) ( (t)->buffw*(t)->buffh )
#define TERMINAL_GET_BUFF_BITS(t) ( sizeof(char)*TERMINAL_GET_BUFF_BYTES((t)) )
#define TERMINAL_GET_CHAR_X_COORD(t, x) ( (t)->offsetx+((x)+1)*(t)->marginleft+(x)*(t)->fontw )
#define TERMINAL_GET_CHAR_Y_COORD(t, y) ( (t)->offsety+((y)+1)*(t)->margintop+(y)*(t)->fonth )
#define TERMINAL_GET_TEXTAREA_WIDTH(t) ( ((t)->buffw+1)*(t)->marginleft+(t)->buffw*(t)->fontw )
#define TERMINAL_GET_TEXTAREA_HEIGHT(t) ( ((t)->buffh+1)*(t)->margintop+(t)->buffh*(t)->fonth )

//Buffers
#define TERMINAL_OUTPUT 0
#define TERMINAL_INPUT 1
//Flags
#define TERMINAL_IECHO 1
#define TERMINAL_OECHO 2
#define TERMINAL_N_UPDATE 4
//Default values
#define TERMINAL_DEFAULT_BUFF_WIDTH 80
#define TERMINAL_DEFAULT_BUFF_HEIGHT 25
#define TERMINAL_DEFAULT_FONT_WIDTH 5
#define TERMINAL_DEFAULT_FONT_HEIGHT 11
#define TERMINAL_DEFAULT_OFFSETX 0
#define TERMINAL_DEFAULT_OFFSETY 9
#define TERMINAL_DEFAULT_MARGINLEFT 1
#define TERMINAL_DEFAULT_MARGINTOP 1
#define TERMINAL_DEFAULT_FLAGS TERMINAL_IECHO | TERMINAL_N_UPDATE

struct terminal {
    Display* d;
    Window w;
    GC gc;
    Pixmap p;
    int s;
    int fontw, fonth;
    int offsetx, offsety;
    int margintop, marginleft;
    void (*close)(terminal*);

    int buffw, buffh;
    char* ibuff;
    char* obuff;
    int icurx, icury;
    int ocurx, ocury;
    int flags;
};

terminal* createTerminal(int w, int h, int flags, void (*close)(terminal*));
void deleteTerminal(terminal* term);
void redrawTerminal(terminal* term);
void updateTerminal(terminal* term); //Calls close when received a destroy message
void swritef(terminal* term, char* fmt, ...); //Simillar to printf
void swrite(terminal* term, const char* str);
char* sread(terminal* term); //Blocks until enter AKA '\n' is pressed.
void cwrite(terminal* term, char chr);
char cread(terminal* term); //Blocks until key is pressed.
void swritef(terminal* term, char* str, ...);
void flush(terminal* term, bool buff);
bool kbhit(terminal* term);

extern terminal* stdterm; //Global terminal

#endif
