/*
board.h
Textsweeper Source Code Available on Github
Author: Karol "digitcrusher" Łacina 2017
*/
#pragma once
#include "math.h"
struct tile {
    int sum;
    bool show,showsum,mine,wrong,flagged;
};
struct board {
    V size;
    tile* map;
    tile* null;
};

extern V tocheck[];

tile* getTile(int x, int y, board* brd);
void createBoard(board* brd, int w, int h, tile null);
void deleteBoard(board* brd);
void resetBoard(board* brd, tile with);
void printBoard(board* brd);
void generateBoard(board* brd, int mines);
void putMines(board* brd, int mines);
void calculateSums(board* brd);
void showAll(board* brd);
bool show(board* brd, int x, int y, bool recursion);
