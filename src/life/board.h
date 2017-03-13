/*
 * life/board.h
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
#ifndef BOARD_H
#define BOARD_H
#include <src/math.h>

struct tile {
    bool alive;
};
struct board {
    V size;
    tile* map;
    tile* null;
    tile nulltemp;
};

tile* getTile(board* brd, int x, int y);
void createBoard(board* brd, int w, int h, tile null);
void deleteBoard(board* brd);
void resetBoard(board* brd, tile with);
void printBoard(board* brd);
void generateBoard(board* brd);

#endif
