/*
 * console.h
 * Textsweeper Source Code Available on Github
 * Author: Karol "digitcrusher" Łacina 2017
 */
#ifndef CONSOLE_H
#define CONSOLE_H

#define CONSOLE_ECHO 1
#define DEFAULT_BUFF_WIDTH 80
#define DEFAULT_BUFF_HEIGHT 25
#define DEFAULT_FONT_WIDTH 5
#define DEFAULT_FONT_HEIGHT 11
#define DEFAULT_OFFSETX 0
#define DEFAULT_OFFSETY 9
#define DEFAULT_MARGINLEFT 1
#define DEFAULT_MARGINTOP 1
#define DEFAULT_FLAGS CONSOLE_ECHO

struct console {
    Display* d;
    Window w;
    GC gc;
    int s;
    int fontw, fonth;
    int offsetx, offsety;
    int margintop, marginleft;

    int buffw, buffh;
    char* ibuff;
    char* obuff;
    int icurx, icury;
    int ocurx, ocury;
    int flags;
};

console* createConsole(int w, int h);
void deleteConsole(console* con);
void redrawConsole(console* con);
void updateConsole(console* con);
#endif
