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
#include <karolslib/src/window/terminal.h>
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
void destroyBoard(board* brd) {
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
    resetBoard(brd, brd->nulltemp);
    putMines(brd, brd->mines);
    calculateSums(brd);
}
void printBoard(board* brd) {
    int fbefore = KL_stdterm->flags;
    KL_stdterm->flags = TERMINAL_DEFAULT_FLAGS-(TERMINAL_N_UPDATE*TERMINAL_DEFAULT_FLAGS & TERMINAL_N_UPDATE);
    for(int y=0; y<brd->size.y; y++) {
        for(int x=0; x<brd->size.x; x++) {
            tile* tile = getTile(brd, x, y);
            if(tile->flag) {
                if(tile->wrong) {
                    KL_cwrite(KL_stdterm, 'R');
                }else {
                    KL_cwrite(KL_stdterm, 'P');
                }
            }else if(!tile->show) {
                KL_cwrite(KL_stdterm, '#');
            }else {
                if(tile->wrong) {
                    if(tile->mine) {
                        KL_cwrite(KL_stdterm, 'X');
                    }else if(tile->showsum) {
                        KL_swritef(KL_stdterm, "%d", tile->sum);
                    }else {
                        KL_cwrite(KL_stdterm, '.');
                    }
                }else {
                    if(tile->mine) {
                        KL_cwrite(KL_stdterm, '*');
                    }else if(tile->showsum) {
                        KL_swritef(KL_stdterm, "%d", tile->sum);
                    }else {
                        KL_cwrite(KL_stdterm, '.');
                    }
                }
            }
        }
        KL_cwrite(KL_stdterm, '\n');
    }
    KL_stdterm->flags = fbefore;
}
void solveBoard(board* brd) {
    board pbrd;
    createBoard(&pbrd, brd->size.x, brd->size.y, {0, 1, 1, 0, 0, 0});
    resetBoard(&pbrd, pbrd.nulltemp);
    //Calculate possibilities
    for(int x=0; x<pbrd.size.x; x++) {
        for(int y=0; y<pbrd.size.y; y++) {
            for(int i=0; i<(int)(sizeof(tocheck)/sizeof(tocheck[0])); i++) {
                V p1 = {x+tocheck[i].x, y+tocheck[i].y};
                tile* t1 = getTile(brd, p1.x, p1.y);
                if(t1->show) {
                    int sum=t1->sum;
                    for(int j=0; j<(int)(sizeof(tocheck)/sizeof(tocheck[0])); j++) {
                        V p2 = {p1.x+tocheck[j].x, p1.y+tocheck[j].y};
                        tile* t2 = getTile(brd, p2.x, p2.y);
                        sum -= t2->flag;
                    }
                    getTile(&pbrd, x, y)->sum += sum;
                    getTile(&pbrd, x, y)->show = !getTile(brd, x, y)->show;
                }
            }
        }
    }
    //Copy numbers
    tile** tiles=(tile**)malloc(sizeof(tile*)*pbrd.size.x*pbrd.size.y);
    for(int x=0; x<pbrd.size.x; x++) {
        for(int y=0; y<pbrd.size.y; y++) {
            tiles[x+y*(int)pbrd.size.x] = getTile(&pbrd, x, y);
        }
    }
    //Sort numbers
    for(int x=0; x<pbrd.size.x*pbrd.size.y; x++) {
        for(int y=0; y<pbrd.size.x*pbrd.size.y-1; y++) {
            if(tiles[y]->sum > tiles[y+1]->sum) {
                tile* temp = tiles[y+1];
                tiles[y+1] = tiles[y];
                tiles[y] = temp;
            }
        }
    }
    //Flags, show etc.
    if(tiles[0]->sum < 0) {
        int i=0;
        int x=(tiles[i]-pbrd.map)%(int)pbrd.size.x;
        int y=(tiles[i]-pbrd.map)/(int)pbrd.size.x;
        for(; !getTile(brd, x, y)->flag; i++) {
            x = (tiles[i]-pbrd.map)%(int)pbrd.size.x;
            y = (tiles[i]-pbrd.map)/(int)pbrd.size.x;
        }
        if(tiles[i]->sum < 0) {
            flag(brd, x, y);
            goto end;
        }
    }
    if(tiles[(int)pbrd.size.x*(int)pbrd.size.y-1]->sum > 0) {
        int i=pbrd.size.x*pbrd.size.y-1;
        int x=(tiles[i]-pbrd.map)%(int)pbrd.size.x;
        int y=(tiles[i]-pbrd.map)/(int)pbrd.size.x;
        for(; getTile(brd, x, y)->flag || getTile(brd, x, y)->show; i--) {
            x = (tiles[i]-pbrd.map)%(int)pbrd.size.x;
            y = (tiles[i]-pbrd.map)/(int)pbrd.size.x;
        }
        if(tiles[i]->sum > 0) {
            flag(brd, x, y);
            goto end;
        }
    }
    //Didn't find anything so guess a tile
    for(int x=0; x<pbrd.size.x; x++) {
        for(int y=0; y<pbrd.size.y; y++) {
            if(!getTile(brd, x, y)->show) {
                show(brd, x, y, 0);
                goto end;
            }
        }
    }
    end:
    destroyBoard(&pbrd);
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
            if(sum > 0) {
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
            if(to->flag && !to->mine) {
                to->wrong = 1;
            }
            if(to->show && to->mine) {
                to->wrong = 1;
            }
            to->show = 1;
        }
    }
}
bool flag(board* brd, int x, int y) {
    tile* to = getTile(brd, x, y);
    if(!(brd->flags <= 0) || to->flag) {
        if(!to->show) {
           to->flag = !to->flag;
            if(to->flag) {
                --brd->flags;
            }else {
                ++brd->flags;
            }
        }
    }else {
        return 1;
    }
    return 0;
}
bool show(board* brd, int x, int y, bool recursion) {
    tile* on = getTile(brd, x, y);
    if(on->flag) {
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
