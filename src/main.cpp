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
#include <karolslib/src/utils/utils.h>
#include <karolslib/src/utils/module.h>
#include <karolslib/src/window/terminal.h>
#include "board.h"

int curx, cury;
board brd;
bool running=1;
bool playing=1;
bool win=0;
KL_module options;
void help() {
    KL_swrite(KL_stdterm, "    How to play\n");
    KL_swrite(KL_stdterm, "Use arrow keys to navigate the cursor.\n");
    KL_swrite(KL_stdterm, "The highlighted tile is where the cursor is.\n");
    KL_swrite(KL_stdterm, "Press 1 to flag the highlighted tile.\n");
    KL_swrite(KL_stdterm, "Press 2 to show the highlighted tile.\n");
    KL_swrite(KL_stdterm, "Every game you get to choose the number of mines\n");
    KL_swrite(KL_stdterm, "and the size of the board.\n");
    KL_swrite(KL_stdterm, "Press any key to continue...\n");
    KL_cread(KL_stdterm);
}
void flag() {
    if(flag(&brd, curx, cury)) {
        KL_swrite(KL_stdterm, "You don't have enough flags!\n");
        KL_cread(KL_stdterm);
    }
}
void show() {
    show(&brd, curx, cury, 0);
}
void solve() {
    solveBoard(&brd);
}
void restart() {
    playing = 0;
    win = 0;
}
void settings() {
    choose:
    char* str;
    KL_swrite(KL_stdterm, "Settings\n");
    KL_swrite(KL_stdterm, "Upper margin ");
    str = KL_sread(KL_stdterm);
    if(stoi(str, &KL_stdterm->margintop)) {
        goto choose;
    }
    KL_swrite(KL_stdterm, "Right margin ");
    str = KL_sread(KL_stdterm);
    if(stoi(str, &KL_stdterm->marginright)) {
        goto choose;
    }
    KL_swrite(KL_stdterm, "Bottom margin ");
    str = KL_sread(KL_stdterm);
    if(stoi(str, &KL_stdterm->marginbottom)) {
        goto choose;
    }
    KL_swrite(KL_stdterm, "Left margin ");
    str = KL_sread(KL_stdterm);
    if(stoi(str, &KL_stdterm->marginleft)) {
        goto choose;
    }
}
void stop() {
    KL_deinit();
    exit(0);
}
void termclose(KL_terminal* term) {
    stop();
}
void addOpt(KL_routine opt) {
    KL_addRtn(opt, &options);
}
board* getBoard() {
    return &brd;
}
int main(int argc, char** argv) {
    KL_init();
    KL_stdterm->close = termclose;
    options = *KL_createModule("options", NULL);
    KL_addRtn(*KL_createRoutine("addOpt", (void (*)())addOpt), KL_getMod("main", KL_stdmodbrd));
    KL_addRtn(*KL_createRoutine("getBoard", (void (*)())getBoard), KL_getMod("main", KL_stdmodbrd));
    addOpt(*KL_createRoutine("Help", (void (*)())help));
    addOpt(*KL_createRoutine("Flag", (void (*)())flag));
    addOpt(*KL_createRoutine("Show", (void (*)())show));
    addOpt(*KL_createRoutine("Solve", (void (*)())solve));
    addOpt(*KL_createRoutine("Restart", (void (*)())restart));
    addOpt(*KL_createRoutine("Settings", (void (*)())settings));
    addOpt(*KL_createRoutine("Quit", (void (*)())stop));
    KL_swrite(KL_stdterm, "textsweeper 1.1 Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n");
    KL_swrite(KL_stdterm, "This program comes with ABSOLUTELY NO WARRANTY.\n");
    KL_swrite(KL_stdterm, "This is free software, and you are welcome to redistribute it\n");
    KL_swrite(KL_stdterm, "under certain conditions.\n");
    help();
    while(running) {
        {
            //Get input
            choose:
            unsigned int x, y, mines;
            char* str;
            KL_swrite(KL_stdterm, "Enter the size of the board:\n");
            KL_swrite(KL_stdterm, "Column ");
            str = KL_sread(KL_stdterm);
            if(stoui(str, &x)) {
                goto choose;
            }
            KL_swrite(KL_stdterm, "Row ");
            str = KL_sread(KL_stdterm);
            if(stoui(str, &y)) {
                goto choose;
            }
            KL_swrite(KL_stdterm, "Mines ");
            str = KL_sread(KL_stdterm);
            bool error = stoui(str, &mines);
            if(error || mines > x*y) {
                goto choose;
            }
            KL_swrite(KL_stdterm, "Initializing board...\n");
            createBoard(&brd, x, y, {0, 0, 0, 0, 0, 0});
            KL_swrite(KL_stdterm, "Generating board...\n");
            generateBoard(&brd, mines);
            brd.nulltemp = {0, 1, 1, 0, 0, 0};
        }
        int starttime=time(0);
        playing = 1;
        while(playing) {
            //Check if won
            bool clear=1;
            int flag=0;
            bool fail=0;
            for(int x=0; x<brd.size.x; x++) {
                for(int y=0; y<brd.size.y; y++) {
                    tile* tile = getTile(&brd, x, y);
                    if(tile->mine && tile->show) {
                        fail = 1;
                    }
                    if(!tile->mine && !tile->show) {
                        clear = 0;
                    }
                    if(tile->mine && tile->flag) {
                        flag++;
                    }else if(tile->mine && !tile->flag) {
                        flag = 0;
                    }
                }
            }
            if(fail) {
                playing = 0;
                win = 0;
                break;
            }
            if(flag == brd.mines || clear) {
                playing = 0;
                win = 1;
                break;
            }
            //Input loop
            while(1) {
                //Print TUI
                KL_flush(KL_stdterm, TERMINAL_OUTPUT);
                printBoard(&brd);
                KL_swritef(KL_stdterm, "You have %d flags.\n", brd.flags);
                for(int i=0; i<options.size; i++) {
                    if(*KL_getRtn(i, &options)->name != '\0') {
                        KL_swritef(KL_stdterm, "%d-%s\n", i, KL_getRtn(i, &options)->name);
                    }
                }
                KL_swrite(KL_stdterm, "Choose an option.\n");
                //Get input
                int wcurx=KL_stdterm->ocurx, wcury=KL_stdterm->ocury;
                KL_gotoxy(KL_stdterm, curx, cury);
                KL_stdterm->flags = TERMINAL_MOVE_OCUR | TERMINAL_CURSOR;
                char c = KL_cread(KL_stdterm);
                int opt = c-'0';
                curx = KL_stdterm->ocurx;
                cury = KL_stdterm->ocury;
                KL_gotoxy(KL_stdterm, wcurx, wcury);
                KL_stdterm->flags = TERMINAL_DEFAULT_FLAGS;
                //Parse the input
                if(c < '0' || c > '9') {
                    KL_swrite(KL_stdterm, "Please enter a number.\n");
                    KL_cread(KL_stdterm);
                    continue;
                }
                KL_routine* rtn;
                if(!(rtn = KL_getRtn(opt, &options))){
                    KL_swrite(KL_stdterm, "Please enter a correct option.\n");
                    KL_cread(KL_stdterm);
                    continue;
                }
                if(rtn->func) {
                    rtn->func();
                }else {
                    continue;
                }
                break;
            }
        }
        //Show statistics
        showAll(&brd);
        KL_flush(KL_stdterm, TERMINAL_OUTPUT);
        printBoard(&brd);
        KL_swrite(KL_stdterm, "Game Over\n");
        if(win) {
            KL_swrite(KL_stdterm, "You win! Yay!\n");
        }else {
            KL_swrite(KL_stdterm, "You lose! Hahaha!\n");
        }
        KL_swritef(KL_stdterm, "Time: %ds\n", time(0)-starttime);
        KL_swrite(KL_stdterm, "Press any key to continue...\n");
        KL_cread(KL_stdterm);
        destroyBoard(&brd);
    }
    stop();
    return 0;
}
