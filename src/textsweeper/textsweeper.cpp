/*
 * textsweeper.cpp
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
#include "textsweeper.h"

using namespace std;

int mines;
int flags;
board brd;
bool running=1;
bool playing=1;
bool win=0;
module options;
void addOpt(routine opt) {
    addRtn(opt, &options);
}
void flag() {
    choose:
    int x, y;
    char* str;
    swrite(stdterm, "Column ");
    str = sread(stdterm);
    if(stoi(str, &x)) {
        goto choose;
    }
    swrite(stdterm, "Row ");
    str = sread(stdterm);
    if(stoi(str, &y)) {
        goto choose;
    }
    tile* to = getTile(x, y, &brd);
    if(!(flags <= 0)) {
        if(!to->show) {
           to->flagged = !to->flagged;
            if(to->flagged) {
                --flags;
            }else {
                ++flags;
            }
        }
    }else {
        swrite(stdterm, "You don't have enough flags!\n");
    }
}
void show() {
    choose:
    int x, y;
    char* str;
    swrite(stdterm, "Column ");
    str = sread(stdterm);
    if(stoi(str, &x)) {
        goto choose;
    }
    swrite(stdterm, "Row ");
    str = sread(stdterm);
    if(stoi(str, &y)) {
        goto choose;
    }
    if(show(&brd, x, y, 0)) {
        playing = 0;
        win = 0;
    }
}
void secret() {
    swrite(stdterm, "Wow, you found a secret. Yoo hoo.\n");
}
board* getBoard() {
    return &brd;
}
int* getMines() {
    return &mines;
}
int* getFlags() {
    return &flags;
}
void init() {
    options = *createModule("options", NULL);
    addRtn(*createRoutine("getFlags", (void (*)())getFlags), getMod("main", modbrd));
    addRtn(*createRoutine("getMines", (void (*)())getMines), getMod("main", modbrd));
    addRtn(*createRoutine("getBoard", (void (*)())getBoard), getMod("main", modbrd));
    addRtn(*createRoutine("addOpt", (void (*)())addOpt), getMod("main", modbrd));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("", (void (*)())secret));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Flag", (void (*)())flag));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Show", (void (*)())show));
}
int run(int argc, char** argv) {
    swrite(stdterm, "TextSweeper C++ Edition 1.0\n");
    while(running) {
        {
            choose:
            int x, y;
            char* str;
            swrite(stdterm, "Enter size of the board\n");
            swrite(stdterm, "Column ");
            str = sread(stdterm);
            if(stoi(str, &x)) {
                goto choose;
            }
            swrite(stdterm, "Row ");
            str = sread(stdterm);
            if(stoi(str, &y)) {
                goto choose;
            }
            if(x <= 0) {
                brd.size.x = 9;
            }else {
                brd.size.x = x;
            }
            if(y <= 0) {
                brd.size.y = 9;
            }else {
                brd.size.y = y;
            }
            mines = brd.size.x*brd.size.y/8;
            flags = mines;
            swrite(stdterm, "Initializing board...\n");
            createBoard(&brd, x, y, {0, 0, 0, 0, 0, 0});
            swrite(stdterm, "Generating board...\n");
            generateBoard(&brd, mines);
        }
        playing = 1;
        while(playing) {
            printBoard(&brd);
            swritef(stdterm, "You have %d flags.\n", flags);
            while(1) {
                int opt;
                for(int i=0; i<options.size; i++) {
                    if(*getRtn(i, &options)->name != '\0') {
                        swritef(stdterm, "%d-%s\n", i, getRtn(i, &options)->name);
                    }
                }
                swrite(stdterm, "Choose an option ");
                char* str = sread(stdterm);
                if(stoi(str, &opt)) {
                    swrite(stdterm, "Please enter a number.\n");
                    continue;
                }
                routine* rtn;
                if(!(rtn = getRtn(opt, &options))){
                    swrite(stdterm, "Please enter a correct option.\n");
                    continue;
                }
                rtn->func();
                break;
            }
            bool clear=1;
            int flagged=0;
            for(int x=0; x<brd.size.x; x++) {
                for(int y=0; y<brd.size.y; y++) {
                    tile* tile = getTile(x, y, &brd);
                    if(!tile->mine && !tile->show) {
                        clear = 0;
                    }
                    if(tile->mine && tile->flagged) {
                        flagged++;
                    }else if(tile->mine && !tile->flagged) {
                        flagged = 0;
                    }
                }
            }
            if(flagged == mines || clear) {
                playing = 0;
                win = 1;
            }
        }
        showAll(&brd);
        printBoard(&brd);
        swrite(stdterm, "Game Over\n");
        if(win) {
            swrite(stdterm, "You win! Yay!\n");
        }else {
            swrite(stdterm, "You lose! Hahaha!\n");
        }
        swrite(stdterm, "Press any key to continue...\n");
        cread(stdterm);
        deleteBoard(&brd);
    }
    return 0;
}
