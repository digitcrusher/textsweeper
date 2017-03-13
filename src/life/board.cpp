/*
 * life/board.cpp
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
#include <src/terminal.h>
#include "board.h"

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
void generateBoard(board* brd) {
    srand(rand()*time(0));
    for(int i=0; i<rand()%(int)(brd->size.x*brd->size.y); i++) {
        randomize:
        srand(rand()*time(0));
        int x = (int)rand()%(int)brd->size.x;
        srand(rand()*time(0));
        int y = (int)rand()%(int)brd->size.y;
        tile* tile = getTile(brd, x, y);
        if(tile->alive) {
            goto randomize;
        }
        tile->alive = 1;
    }
}
void printBoard(board* brd) {
    stdterm->ocurx = 0;
    stdterm->ocury = 0;
    for(int y=0; y<brd->size.y; y++) {
        for(int x=0; x<brd->size.x; x++) {
            tile* tile = getTile(brd, x, y);
            if(tile->alive) {
                cwrite(stdterm, '#');
            }else {
                cwrite(stdterm, ' ');
            }
        }
        cwrite(stdterm, '\n');
    }
}
