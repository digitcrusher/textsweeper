/*
 * board.cpp
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
#include <karolslib/src/terminal.h>
#include "board.h"

V tocheck[] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

tile* getTile(board* brd, int x, int y) {
    *brd->null = brd->nulltemp;
    if(sqrInSqr({0, 0, brd->size.x-1, brd->size.y-1}, {(float)x, (float)y, (float)x, (float)y})) {
        return brd->map+x+y*(int)brd->size.x;
    }
    return brd->null;
}
void createBoard(board* brd, int w, int h, tile null) {
    brd->size.x = w;
    brd->size.y = h;
    brd->map = (tile*)malloc(sizeof(tile)*w*h);
    brd->null = (tile*)malloc(sizeof(tile));
    brd->nulltemp = null;
    *brd->null = brd->nulltemp;
}
void deleteBoard(board* brd) {
    free(brd->map);
    free(brd->null);
}
void resetBoard(board* brd, tile with) {
    for(int x=0; x<brd->size.x; x++) {
        for(int y=0; y<brd->size.y; y++) {
            tile* tile = getTile(brd, x, y);
            *tile = with;
        }
    }
}
void generateBoard(board* brd, int mines) {
    brd->mines = mines;
    brd->flags = brd->mines;
    resetBoard(brd, *brd->null);
    putMines(brd, brd->mines);
    calculateSums(brd);
}
void printBoard(board* brd) {
    stdterm->ocurx = 0;
    stdterm->ocury = 0;
    for(int y=0; y<brd->size.y; y++) {
        for(int x=0; x<brd->size.x; x++) {
            tile* tile = getTile(brd, x, y);
            if(tile->flagged) {
                if(tile->wrong) {
                    cwrite(stdterm, 'R');
                }else {
                    cwrite(stdterm, 'P');
                }
            }else if(!tile->show) {
                cwrite(stdterm, '#');
            }else {
                if(tile->wrong) {
                    if(tile->mine) {
                        cwrite(stdterm, 'X');
                    }else if(tile->showsum) {
                        swritef(stdterm, "%d", tile->sum);
                    }else {
                        cwrite(stdterm, '.');
                    }
                }else {
                    if(tile->mine) {
                        cwrite(stdterm, '*');
                    }else if(tile->showsum) {
                        swritef(stdterm, "%d", tile->sum);
                    }else {
                        cwrite(stdterm, '.');
                    }
                }
            }
        }
        cwrite(stdterm, '\n');
    }
}
void putMines(board* brd, int mines) {
    for(int i=0; i<mines; i++) {
        randomize:
        srand(rand()*time(0));
        int x = (int)rand()%(int)brd->size.x;
        srand(rand()*time(0));
        int y = (int)rand()%(int)brd->size.y;
        tile* tile = getTile(brd, x, y);
        if(tile->mine) {
            goto randomize;
        }
        tile->mine = 1;
    }
}
void calculateSums(board* brd) {
    for(int x=0; x<brd->size.x; x++) {
        for(int y=0; y<brd->size.y; y++) {
            tile* on = getTile(brd, x, y);
            int sum=0;
            for(int i=0; i<(int)(sizeof(tocheck)/sizeof(tocheck[0])); i++) {
                V pos = {x+tocheck[i].x, y+tocheck[i].y};
                tile* to = getTile(brd, pos.x, pos.y);
                if(to->mine) {
                    sum += 1;
                }
            }
            if(sum>0) {
                on->showsum = 1;
            }else {
                on->showsum = 0;
            }
            on->sum = sum;
        }
    }
}
void showAll(board* brd) {
    for(int x=0; x<brd->size.x; x++) {
        for(int y=0; y<brd->size.y; y++) {
            tile* to = getTile(brd, x, y);
            if(to->flagged && !to->mine) {
                to->wrong = 1;
            }
            if(to->show && to->mine) {
                to->wrong = 1;
            }
            to->show = 1;
        }
    }
}
bool show(board* brd, int x, int y, bool recursion) {
    tile* on = getTile(brd, x, y);
    if(on->flagged) {
        return 0;
    }
    on->show = 1;
    if(on->mine) {
        on->wrong = 1;
        return 1;
    }
    if(!recursion && !on->showsum) {
        for(int i=0; i<(int)(sizeof(tocheck)/sizeof(tocheck[0])); i++) {
            V pos = {x+tocheck[i].x, y+tocheck[i].y};
            tile* to = getTile(brd, pos.x, pos.y);
            if(!to->show && sqrInSqr({0, 0, brd->size.x-1, brd->size.y-1}, {pos.x, pos.y, pos.x, pos.y})) {
                if(show(brd, pos.x, pos.y, to->showsum)) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
