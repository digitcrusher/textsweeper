/*
 * main.cpp
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
#include <iostream>
#include <string.h>
#include "utils.h"
#include "board.h"
#include "module.h"
#include "terminal.h"

using namespace std;

int x11f(int, char**) {
    flush(stdterm, TERMINAL_OUTPUT);
    swrite(stdterm, "Hello, World!");
    updateTerminal(stdterm);
    flush(stdterm, TERMINAL_OUTPUT);
    swrite(stdterm, "Haha!");
    while(1) {
        updateTerminal(stdterm);
    }
    return 0;
}

int mines;
int flags;
board brd;
bool running=1;
bool playing=1;
bool win=0;
module options;
bool addOptf(routine opt) {
    addRtn(opt, &options);
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
    addRtn(*createRoutine("getFlags", (void (*)())getFlagsf), getMod("main", modbrd));
    addRtn(*createRoutine("getMines", (void (*)())getMinesf), getMod("main", modbrd));
    addRtn(*createRoutine("getBoard", (void (*)())getBoardf), getMod("main", modbrd));
    addRtn(*createRoutine("addOpt", (void (*)())addOptf), getMod("main", modbrd));
    ((bool (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("", (void (*)())secretf));
    ((bool (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Flag", (void (*)())flagf));
    ((bool (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Show", (void (*)())showf));
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
                for(int i=0;i<options.size;i++) {
                    if(*getRtn(i, &options)->name!='\0') {
                        cout<<i<<"-"<<getRtn(i, &options)->name<<'\n';
                    }
                }
                cout<<"Choose an option ";
                cin>>opt;
                routine* rtn;
                if(!(rtn=getRtn(opt, &options))){
                    cout<<"Please enter a correct option.\n";
                    continue;
                }
                rtn->func();
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
    //Inform the user that we use GPLv3
    cout<<"textsweeper Copyright (C) 2017 Karol \"digitcrusher\" Łacina"<<'\n';
    cout<<"This program comes with ABSOLUTELY NO WARRANTY."<<'\n';
    cout<<"This is free software, and you are welcome to redistribute it"<<'\n';
    cout<<"under certain conditions."<<'\n';
    //Create module main and add it to the global modboard modbrd.
    addMod(*createModule("main", NULL), modbrd);
    //Create routine main pointing to casted mainf and add it to module main from modbrd.
    addRtn(*createRoutine("main", (void (*)())mainf), getMod("main", modbrd));
    //Get routine main from main and change it so it pointing to function x11f.
    getRtn("main", getMod("main", modbrd))->func = (void (*)())x11f;
    //Iterate over the modules from modbrd.
    for(int i=0; i<modbrd->size; i++) {
        routine* rtn;
        //Rtn has init? If yes call it.
        if((rtn = getRtn("init", getMod(i, modbrd)))) {
            rtn->func();
        }
    }
    /*
     * This is a big pile of casting and calling the main function, here i explain it:
     * getRtn("main", getMod("main", modbrd))->func) Get function pointer from main which is from main.
     * ((int (*)(int, char**)) Cast it to a function which returns int and accepts int and char** as arguments/
     * (argc, argv) Call it with argc and argv
     */
    return ((int (*)(int, char**))getRtn("main", getMod("main", modbrd))->func)(argc, argv);
}
