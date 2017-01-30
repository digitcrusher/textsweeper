/*
main.cpp
Textsweeper Source Code Available on Github
Author: Karol "digitcrusher" Łacina 2017
*/
#include <iostream>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include "utils.h"
#include "board.h"
#include "modules.h"

using namespace std;

Display* d;
Window w;
GC gc;
int s;
bool loop=1;
char* buff="12345678901234567890123456789012345678901234567890\nqwertyuiopasdfghjklzxcvbnm\nQWERTYUIOPASDFGHJKLZXCVBNM\n1234567890\nabcdefghijklmnopqrstuvwxyz\nABCDEFGHIJKLMNOPRSTUVWXYZ";
int x=0,y=0;
int fx=5,fy=11;
int offsetx=0,offsety=9;
int margintop=1,marginleft=1;
bool curcycle=1;
void redraw() {
    for(unsigned int i=0;i<strlen(buff);i++) {
        char* c=(char*)malloc(sizeof(char)*2);
        memcpy(c,buff+i,1);
        *(c+1)='\0';
        switch(buff[i]) {
            case '\n':
                x=0;
                y++;
                break;
            default:
                XDrawString(d, w, gc, offsetx+(x+1)*marginleft+x*fx, offsety+(y+1)*margintop+y*fy, c, strlen(c));
                x++;
                break;
        }
    }
    if(curcycle) {
        XFillRectangle(d, w, gc, (x+1)*marginleft+x*fx, (y+1)*margintop+y*fy, fx, fy);
    }
    curcycle=!curcycle;
}
void x11f() {
    d = XOpenDisplay(NULL);
    if(d==NULL) {
        cerr<<"Cannot open display\n";
        exit(1);
    }
    s = DefaultScreen(d);
    w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, (50+1)*marginleft+50*fx, (20+1)*margintop+50*fy, 1, WhitePixel(d, s), BlackPixel(d, s));
    XSetStandardProperties(d, w, "Console", "Console", None, NULL, 0, NULL);
    XSelectInput(d, w, ExposureMask | ButtonPressMask | KeyPressMask);
    gc = XCreateGC(d, w, 0, 0);
    XSetBackground(d, gc, BlackPixel(d, s));
    XSetForeground(d, gc, WhitePixel(d, s));
    XClearWindow(d, w);
    XMapWindow(d, w);
    
    XEvent e;
    while(loop) {
        XNextEvent(d, &e);
        switch(e.type) {
            case Expose:
                redraw();
                break;
            case KeyPress:
                loop=0;
                break;
        }
    }
    XFreeGC(d, gc);
    XDestroyWindow(d, w);
    XCloseDisplay(d);
    exit(0);
}

int mines;
int flags;
board brd;
bool running=1;
bool playing=1;
bool win=0;
module options;
bool addOptf(operation opt) {
    addOp(opt, &options);
    return 0;
}
void flagf() {
    int x,y;
    cout<<"Column ";
    cin>>x;
    cout<<"Row ";
    cin>>y;
    tile* to = getTile(x,y,&brd);
    if(!(flags<=0)) {
        if(!to->show) {
           to->flagged=!to->flagged;
            if(to->flagged) {
                --flags;
            }else {
                ++flags;
            }
        }
    }else {
        cout<<"You don't have enough flags!\n";
    }
}
void showf() {
    int x,y;
    cout<<"Column ";
    cin>>x;
    cout<<"Row ";
    cin>>y;
    if(show(&brd,x,y,0)) {
        playing=0;
        win=0;
    }
}
void secretf() {
    cout<<"Wow, you found a secret. Yoo hoo.\n";
}
board* getBoardf() {
    return &brd;
}
int* getMinesf() {
    return &mines;
}
int* getFlagsf() {
    return &flags;
}
int mainf(int argc, char** argv) {
    options = *createModule("options", NULL);
    addOp(*createOperation("getFlags", (void (*)())getFlagsf), getMod("main", modbrd));
    addOp(*createOperation("getMines", (void (*)())getMinesf), getMod("main", modbrd));
    addOp(*createOperation("getBoard", (void (*)())getBoardf), getMod("main", modbrd));
    addOp(*createOperation("addOpt", (void (*)())addOptf), getMod("main", modbrd));
    ((bool (*)(operation))getOp("addOpt", getMod("main", modbrd))->func)(*createOperation("", (void (*)())secretf));
    ((bool (*)(operation))getOp("addOpt", getMod("main", modbrd))->func)(*createOperation("Flag", (void (*)())flagf));
    ((bool (*)(operation))getOp("addOpt", getMod("main", modbrd))->func)(*createOperation("Show", (void (*)())showf));
    cout<<"TextSweeper C++ Edition 1.0 by digitcrusher\n";
    cout<<"Initializing board...\n";
    createBoard(&brd,9,9,{0,0,0,0,0,0});
    while(running) {
        {
        int x=0,y=0;
        cout<<"Enter size of the board.\n";
        cout<<"Columns ";
        cin>>x;
        cout<<"Rows ";
        cin>>y;
        if(x<=0) {
            brd.size.x=9;
        }else {
            brd.size.x=x;
        }
        if(y<=0) {
            brd.size.y=9;
        }else {
            brd.size.y=y;
        }
        }
        mines=brd.size.x*brd.size.y/8;
        flags=mines;
        cout<<"Generating board...\n";
        generateBoard(&brd,mines);
        playing=1;
        while(playing) {
            printBoard(&brd);
            cout<<"You have "<<flags<<" flags.\n";
            while(1) {
                int opt;
                for(int i=0;i<options.opssize;i++) {
                    if(*getOp(i, &options)->name!='\0') {
                        cout<<i<<"-"<<getOp(i, &options)->name<<'\n';
                    }
                }
                cout<<"Choose an option ";
                cin>>opt;
                operation* op;
                if(!(op=getOp(opt, &options))){
                    cout<<"Please enter a correct option.\n";
                    continue;
                }
                op->func();
                break;
            }
            bool clear=1;
            int flagged=0;
            for(int x=0;x<brd.size.x;x++) {
                for(int y=0;y<brd.size.y;y++) {
                    tile* tile = getTile(x,y,&brd);
                    if(!tile->mine && !tile->show) {
                        clear=0;
                    }
                    if(tile->mine && tile->flagged) {
                        flagged++;
                    }else if(tile->mine && !tile->flagged) {
                        flagged=0;
                    }
                }
            }
            if(flagged == mines || clear) {
                playing=0;
                win=1;
            }
        }
        showAll(&brd);
        printBoard(&brd);
        cout<<"Game Over\n";
        if(win) {
            cout<<"You win! Yay!\n";
        }else {
            cout<<"You lose! Hahaha!\n";
        }
        cout<<"Press any key to continue...\n";
        getch();
    }
    deleteBoard(&brd);
    return 0;
}
int main(int argc, char** argv) {
    addMod(*createModule("main", NULL), modbrd);
    addOp(*createOperation("main", (void (*)())mainf), getMod("main", modbrd));
    //getOp("main", getMod("main", modbrd))->func=x11f;
    for(int i=0;i<modbrd->size;i++) {
        operation* op;
        if((op=getOp("init", getMod(i, modbrd)))) {
            op->func();
        }
    }
    return ((int (*)(int, char**))getOp("main", getMod("main", modbrd))->func)(argc, argv);
}
