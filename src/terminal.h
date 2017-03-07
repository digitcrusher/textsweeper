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

#define TERMINAL_GET_TEXTAREA_WIDTH(a) ((a->buffw+1)*a->marginleft+a->buffw*a->fontw)
#define TERMINAL_GET_TEXTAREA_HEIGHT(a) ((a->buffh+1)*a->margintop+a->buffh*a->fonth)

#define TERMINAL_OUTPUT 0
#define TERMINAL_INPUT 1
#define TERMINAL_ECHO 1
#define TERMINAL_WRITECLEAR 2
#define DEFAULT_BUFF_WIDTH 80
#define DEFAULT_BUFF_HEIGHT 25
#define DEFAULT_FONT_WIDTH 5
#define DEFAULT_FONT_HEIGHT 11
#define DEFAULT_OFFSETX 0
#define DEFAULT_OFFSETY 9
#define DEFAULT_MARGINLEFT 1
#define DEFAULT_MARGINTOP 1
#define DEFAULT_FLAGS TERMINAL_ECHO

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

terminal* createTerminal(int w, int h, int flags, void (*destroy)(terminal*));
void deleteTerminal(terminal* term);
void redrawTerminal(terminal* term);
void updateTerminal(terminal* term); //Calls close when received a destroy message
void swrite(terminal* term, const char* str);
char* sread(terminal* term); //Blocks until enter AKA '\n' is pressed.
void cwrite(terminal* term, char chr);
char cread(terminal* term); //Blocks until key is pressed.
void flush(terminal* term, bool buff);

extern terminal* stdterm; //Global terminal

#endif
