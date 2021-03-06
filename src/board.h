/*
 * board.h
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
#ifndef BOARD_H
#define BOARD_H
#include <karolslib/src/utils/math.h>

struct tile {
    int sum;
    bool show, showsum, mine, wrong, flag;
};
struct board {
    V size;
    int flags, mines;
    tile* map;
    tile* null;
    tile nulltemp;
};

extern V tocheck[];

tile* getTile(board* brd, int x, int y);
void createBoard(board* brd, int w, int h, tile null);
void destroyBoard(board* brd);
void resetBoard(board* brd, tile with);
void printBoard(board* brd);
void solveBoard(board* brd);
void generateBoard(board* brd, int mines);
void putMines(board* brd, int mines);
void calculateSums(board* brd);
void showAll(board* brd);
bool flag(board* brd, int x, int y);
bool show(board* brd, int x, int y, bool recursion);

#endif
