/*
 * console.cpp
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
#include <stdlib.h>
#include "console.h"

console* createConsole(int w, int h) {
    console* con = (console*)malloc(sizeof(console));
    con->buffw = w;
    con->buffh = h;
    con->ibuff = (char*)malloc(sizeof(char)*con->buffw*con->buffh);
    con->obuff = (char*)malloc(sizeof(char)*con->buffw*con->buffh);
    con->fontw = DEFAULT_FONT_WIDTH;
    con->fonth = DEFAULT_FONT_HEIGHT;
    con->offsetx = DEFAULT_OFFSETX;
    con->offsety = DEFAULT_OFFSETY;
    con->margintop = DEFAULT_MARGINTOP;
    con->marginleft = DEFAULT_MARGINLEFT;
    con->icurx = 0;
    con->icury = 0;
    con->ocurx = 0;
    con->ocury = 0;
    con->flags = DEFAULT_FLAGS;

    con->d = XOpenDisplay(NULL);
    if(con->d == NULL) {
        return NULL;
    }
    con->s = DefaultScreen(con->d);
    con->w = XCreateSimpleWindow(con->d, RootWindow(con->d, con->s), 0, 0
                                ,(con->buffw+1)*con->marginleft+con->buffw*con->fontw
                                ,(con->buffh+1)*con->margintop+con->buffh*con->fonth
                                ,1, WhitePixel(con->d, con->s), BlackPixel(con->d, con->s));
    XSetStandardProperties(con->d, con->w, "Console", "Console", None, NULL, 0, NULL);
    XSelectInput(con->d, con->w, StructureNotifyMask | ExposureMask | KeyPressMask);
    con->gc = XCreateGC(con->d, con->w, 0, 0);
    XSetBackground(con->d, con->gc, BlackPixel(con->d, con->s));
    XSetForeground(con->d, con->gc, WhitePixel(con->d, con->s));
    XClearWindow(con->d, con->w);
    XMapWindow(con->d, con->w);
    XEvent e;
    while(1) {
        XNextEvent(con->d, &e);
        if(e.type == MapNotify) {
            break;
        }
    }
    return con;
}
void deleteConsole(console* con) {
    free(con->ibuff);
    free(con->obuff);
    XFreeGC(con->d, con->gc);
    XDestroyWindow(con->d, con->w);
    XCloseDisplay(con->d);
}
void redrawConsole(console* con) {
    for(int x=0; x < con->buffw; x++) {
        for(int y=0; y < con->buffh; y++) {
            char* c = (char*)malloc(sizeof(char)*2);
            *c = *(con->obuff+x+con->buffw*y);
            *(c+1) = '\0';
            XDrawString(con->d, con->w, con->gc, con->offsetx+(x+1)*con->marginleft+x*con->fontw
                       ,con->offsety+(y+1)*con->margintop+y*con->fonth, c, strlen(c));
        }
    }
}
void updateConsole(console* con) {
    XEvent e;
    while(1) {
        if(XPending(con->d)) {
            XNextEvent(con->d, &e);
            switch(e.type) {
                case Expose:
                    redrawConsole(con);
                    break;
                case KeyPress:
                    break;
            }
        }else {
            sleep(500);
            XEvent e = {Expose};
            XSendEvent(con->d, con->w, 0, ExposureMask, &e);
        }
    }
}
