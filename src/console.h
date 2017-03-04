/*
 * console.h
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
