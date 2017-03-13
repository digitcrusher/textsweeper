/*
 * life/life.cpp
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
#include <src/utils.h>
#include <src/module.h>
#include <src/terminal.h>
#include "board.h"
#include "life.h"

int curx, cury;
board brd;
bool running=1;
bool playing=1;
module options;
void stop() {
    exit(0);
}
void restart() {
    playing = 0;
}
void set() {
    //Prepare terminal for user input
    int wcurx=stdterm->ocurx, wcury=stdterm->ocury;
    stdterm->ocurx = curx;
    stdterm->ocury = cury;
    stdterm->flags = TERMINAL_MOVE_OCUR | TERMINAL_CURSOR;
    //Get input
    tile* to = getTile(&brd, curx, cury);
    to->alive = !to->alive;
    //Go back to normal
    curx = stdterm->ocurx;
    cury = stdterm->ocury;
    stdterm->ocurx = wcurx;
    stdterm->ocury = wcury;
    stdterm->flags = TERMINAL_DEFAULT_FLAGS;
}
void step() {
}
void loop() {
    if(!kbhit(stdterm)) {
        step();
    }
}
void randomize() {
    generateBoard(&brd);
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
void life_init() {
    options = *createModule("options", NULL);
    addRtn(*createRoutine("addOpt", (void (*)())addOpt), getMod("main", modbrd));
    addRtn(*createRoutine("getBoard", (void (*)())getBoard), getMod("main", modbrd));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Quit", (void (*)())stop));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Restart", (void (*)())restart));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Set", (void (*)())set));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Step", (void (*)())step));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Step in a loop", (void (*)())loop));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Randomize", (void (*)())randomize));
    ((void (*)(routine))getRtn("addOpt", getMod("main", modbrd))->func)(*createRoutine("Settings", (void (*)())settings));
}
int life_run(int argc, char** argv) {
    swrite(stdterm, "Conway's Game of Life C++ Edition 1.0\n");
    while(running) {
        {
            //Get input
            choose:
            unsigned int x, y;
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
            swrite(stdterm, "Initializing board...\n");
            createBoard(&brd, x, y, {0});
        }
        playing = 1;
        while(playing) {
            while(1) {
                //Print TUI
                stdterm->flags = (TERMINAL_DEFAULT_FLAGS)-(TERMINAL_N_UPDATE*(TERMINAL_DEFAULT_FLAGS) & TERMINAL_N_UPDATE);
                flush(stdterm, TERMINAL_OUTPUT);
                printBoard(&brd);
                for(int i=0; i<options.size; i++) {
                    if(*getRtn(i, &options)->name != '\0') {
                        swritef(stdterm, "%d-%s\n", i, getRtn(i, &options)->name);
                    }
                }
                swrite(stdterm, "Choose an option.\n");
                //Get input
                char* str = sread(stdterm);
                int opt;
                //Parse the input
                if(stoi(str, &opt)) {
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
        flush(stdterm, TERMINAL_OUTPUT);
        printBoard(&brd);
        swrite(stdterm, "Press any key to continue...\n");
        cread(stdterm);
        deleteBoard(&brd);
    }
    return 0;
}
