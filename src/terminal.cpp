/*
 * terminal.cpp
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
#include <string.h>
#include <iostream>
#include "terminal.h"

terminal* stdterm = createTerminal(DEFAULT_BUFF_WIDTH, DEFAULT_BUFF_HEIGHT, DEFAULT_FLAGS, NULL);

terminal* createTerminal(int w, int h, int flags, void (*close)(terminal*)) {
    terminal* term = (terminal*)malloc(sizeof(terminal));
    term->fontw = DEFAULT_FONT_WIDTH;
    term->fonth = DEFAULT_FONT_HEIGHT;
    term->offsetx = DEFAULT_OFFSETX;
    term->offsety = DEFAULT_OFFSETY;
    term->margintop = DEFAULT_MARGINTOP;
    term->marginleft = DEFAULT_MARGINLEFT;
    term->close = close;
    term->buffw = w;
    term->buffh = h;
    term->ibuff = (char*)malloc(sizeof(char)*term->buffw*term->buffh);
    term->obuff = (char*)malloc(sizeof(char)*term->buffw*term->buffh);
    term->icurx = 0;
    term->icury = 0;
    term->ocurx = 0;
    term->ocury = 0;
    term->flags = flags;

    term->d = XOpenDisplay(NULL);
    if(term->d == NULL) {
        return NULL;
    }
    term->s = DefaultScreen(term->d);
    term->w = XCreateSimpleWindow(term->d, RootWindow(term->d, term->s), 0, 0
                                ,TERMINAL_GET_TEXTAREA_WIDTH(term)
                                ,TERMINAL_GET_TEXTAREA_HEIGHT(term)
                                ,1, WhitePixel(term->d, term->s), BlackPixel(term->d, term->s));
    XSetStandardProperties(term->d, term->w, "Terminal", "Terminal", None, NULL, 0, NULL);
    XSelectInput(term->d, term->w, StructureNotifyMask | ExposureMask | KeyPressMask);
    term->gc = XCreateGC(term->d, term->w, 0, 0);
    XSetBackground(term->d, term->gc, BlackPixel(term->d, term->s));
    XSetForeground(term->d, term->gc, WhitePixel(term->d, term->s));
    XClearWindow(term->d, term->w);
    XMapWindow(term->d, term->w);
    XWindowAttributes wa;
    XGetWindowAttributes(term->d, term->w, &wa);
    term->p = XCreatePixmap(term->d, term->w
                            ,TERMINAL_GET_TEXTAREA_WIDTH(term)
                            ,TERMINAL_GET_TEXTAREA_HEIGHT(term)
                            ,wa.depth);
    XEvent e;
    while(e.type != MapNotify) {
        XNextEvent(term->d, &e);
    }
    return term;
}
void deleteTerminal(terminal* term) {
    free(term->ibuff);
    free(term->obuff);
    XFreePixmap(term->d, term->p);
    XFreeGC(term->d, term->gc);
    XDestroyWindow(term->d, term->w);
    XCloseDisplay(term->d);
    free(term);
}
void redrawTerminal(terminal* term) {
    XSetForeground(term->d, term->gc, BlackPixel(term->d, term->s));
    XFillRectangle(term->d, term->p, term->gc, 0, 0
                  ,TERMINAL_GET_TEXTAREA_WIDTH(term)
                  ,TERMINAL_GET_TEXTAREA_HEIGHT(term));
    XSetForeground(term->d, term->gc, WhitePixel(term->d, term->s));
    for(int x=0; x<term->buffw; x++) {
        for(int y=0; y<term->buffh; y++) {
            char* c = (char*)malloc(sizeof(char)*2);
            *c = *(term->obuff+x+term->buffw*y);
            *(c+1) = '\0';
            XDrawString(term->d, term->p, term->gc
                       ,term->offsetx+(x+1)*term->marginleft+x*term->fontw
                       ,term->offsety+(y+1)*term->margintop+y*term->fonth, c, strlen(c));
        }
    }
    XCopyArea(term->d, term->p, term->w, term->gc, 0, 0
             ,TERMINAL_GET_TEXTAREA_WIDTH(term)
             ,TERMINAL_GET_TEXTAREA_HEIGHT(term), 0, 0);
}
void updateTerminal(terminal* term) {
    XEvent e;
    while(XPending(term->d)) {
        XNextEvent(term->d, &e);
        switch(e.type) {
            case Expose:
                redrawTerminal(term);
                break;
            case KeyPress:
                char buff[16];
                KeySym ks;
                XLookupString(&e.xkey, buff, 16, &ks, NULL);

                break;
            case DestroyNotify:
                if(term->close != NULL) {
                    term->close(term);
                }
                break;
        }
    }
    redrawTerminal(term);
}
void swrite(terminal* term, const char* str) {
    for(unsigned int i=0; i<strlen(str); i++) {
        cwrite(term, str[i]);
    }
}
char* sread(terminal* term);
void cwrite(terminal* term, char chr) {
    switch(chr) {
        case '\n':
            term->ocurx = 0;
            ++term->ocury;
            break;
        default:
            *(term->obuff+term->ocurx+term->ocury*term->buffw) = chr;
            ++term->ocurx;
            break;
    }
    if(term->ocurx == term->buffw) {
        term->ocurx = 0;
        ++term->ocury;
    }
    if(term->ocury == term->buffh) {
        term->ocury = 0;
    }
}
char cread(terminal* term);
void flush(terminal* term, bool buff) {
    if(buff) {
        memset(term->ibuff, '\0', term->buffw+term->buffw*term->buffh);
        term->icurx = 0;
        term->icury = 0;
    }else {
        memset(term->obuff, '\0', term->buffw+term->buffw*term->buffh);
        term->ocurx = 0;
        term->ocury = 0;
    }
}
