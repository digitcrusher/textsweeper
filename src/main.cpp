/*
 * main.cpp
 * textsweeper Source Code
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
#include <time.h>
#include <karolslib/src/karolslib.h>
#include <karolslib/src/utils.h>
#include <karolslib/src/module.h>
#include <karolslib/src/terminal.h>
#include "board.h"

int curx, cury;
board brd;
bool running=1;
bool playing=1;
bool win=0;
module options;
void stop() {
    exit(0);
}
void flag() {
    tile* to = getTile(&brd, curx, cury);
    if(!(brd.flags <= 0) || to->flagged) {
        if(!to->show) {
           to->flagged = !to->flagged;
            if(to->flagged) {
                --brd.flags;
            }else {
                ++brd.flags;
            }
        }
    }else {
        swrite(stdterm, "You don't have enough flags!\n");
        cread(stdterm);
    }
}
void show() {
    if(show(&brd, curx, cury, 0)) {
        playing = 0;
        win = 0;
    }
}
void restart() {
    playing = 0;
    win = 0;
}
void settings() {
    choose:
    char* str;
    swrite(stdterm, "Settings\n");
    swrite(stdterm, "Upper margin ");
    str = sread(stdterm);
    if(stoi(str, &stdterm->margintop)) {
        goto choose;
    }
    swrite(stdterm, "Right margin ");
    str = sread(stdterm);
    if(stoi(str, &stdterm->marginright)) {
        goto choose;
    }
    swrite(stdterm, "Bottom margin ");
    str = sread(stdterm);
    if(stoi(str, &stdterm->marginbottom)) {
        goto choose;
    }
    swrite(stdterm, "Left margin ");
    str = sread(stdterm);
    if(stoi(str, &stdterm->marginleft)) {
        goto choose;
    }
}
void addOpt(routine opt) {
    addRtn(opt, &options);
}
board* getBoard() {
    return &brd;
}
int main(int argc, char** argv) {
    karolslib_init();
    options = *createModule("options", NULL);
    addRtn(*createRoutine("addOpt", (void (*)())addOpt), getMod("main", modbrd));
    addRtn(*createRoutine("getBoard", (void (*)())getBoard), getMod("main", modbrd));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Quit", (void (*)())stop));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Flag", (void (*)())flag));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Show", (void (*)())show));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Restart", (void (*)())restart));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Settings", (void (*)())settings));
    swrite(stdterm, "textsweeper 1.0 Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n");
    swrite(stdterm, "This program comes with ABSOLUTELY NO WARRANTY.\n");
    swrite(stdterm, "This is free software, and you are welcome to redistribute it\n");
    swrite(stdterm, "under certain conditions.\n");
    while(running) {
        {
            //Get input
            choose:
            unsigned int x, y, mines;
            char* str;
            swrite(stdterm, "Enter the size of the board:\n");
            swrite(stdterm, "Column ");
            str = sread(stdterm);
            if(stoui(str, &x)) {
                goto choose;
            }
            swrite(stdterm, "Row ");
            str = sread(stdterm);
            if(stoui(str, &y)) {
                goto choose;
            }
            swrite(stdterm, "Mines ");
            str = sread(stdterm);
            bool error = stoui(str, &mines);
            if(error || mines > x*y) {
                goto choose;
            }
            swrite(stdterm, "Initializing board...\n");
            createBoard(&brd, x, y, {0, 0, 1, 0, 0, 0});
            swrite(stdterm, "Generating board...\n");
            generateBoard(&brd, mines);
        }
        int starttime=time(0);
        playing = 1;
        while(playing) {
            //Check if won
            bool clear=1;
            int flagged=0;
            for(int x=0; x<brd.size.x; x++) {
                for(int y=0; y<brd.size.y; y++) {
                    tile* tile = getTile(&brd, x, y);
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
            if(flagged == brd.mines || clear) {
                playing = 0;
                win = 1;
                break;
            }
            //Input loop
            while(1) {
                //Print TUI
                stdterm->flags = TERMINAL_DEFAULT_FLAGS-(TERMINAL_N_UPDATE*TERMINAL_DEFAULT_FLAGS & TERMINAL_N_UPDATE);
                flush(stdterm, TERMINAL_OUTPUT);
                printBoard(&brd);
                swritef(stdterm, "You have %d flags.\n", brd.flags);
                for(int i=0; i<options.size; i++) {
                    if(*getRtn(i, &options)->name != '\0') {
                        swritef(stdterm, "%d-%s\n", i, getRtn(i, &options)->name);
                    }
                }
                swrite(stdterm, "Choose an option.\n");
                //Get input
                int wcurx=stdterm->ocurx, wcury=stdterm->ocury;
                stdterm->ocurx = curx;
                stdterm->ocury = cury;
                stdterm->flags = TERMINAL_MOVE_OCUR | TERMINAL_CURSOR;
                char c = cread(stdterm);
                int opt = c-'0';
                curx = stdterm->ocurx;
                cury = stdterm->ocury;
                stdterm->ocurx = wcurx;
                stdterm->ocury = wcury;
                stdterm->flags = TERMINAL_DEFAULT_FLAGS;
                //Parse the input
                if(c < '0' || c > '9') {
                    swrite(stdterm, "Please enter a number.\n");
                    cread(stdterm);
                    continue;
                }
                routine* rtn;
                if(!(rtn = getRtn(opt, &options))){
                    swrite(stdterm, "Please enter a correct option.\n");
                    cread(stdterm);
                    continue;
                }
                rtn->func();
                break;
            }
        }
        //Show statistics
        showAll(&brd);
        flush(stdterm, TERMINAL_OUTPUT);
        printBoard(&brd);
        swrite(stdterm, "Game Over\n");
        if(win) {
            swrite(stdterm, "You win! Yay!\n");
        }else {
            swrite(stdterm, "You lose! Hahaha!\n");
        }
        swritef(stdterm, "Time: %ds\n", time(0)-starttime);
        swrite(stdterm, "Press any key to continue...\n");
        cread(stdterm);
        deleteBoard(&brd);
    }
    return 0;
}
