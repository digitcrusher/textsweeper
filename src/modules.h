/*
modules.h
Textsweeper Source Code Available on Github
Author: Karol "digitcrusher" Łacina 2017
*/
#pragma once

struct operation {
    char* name;
    void (*func)(void);
};
struct module {
    char* name;
    void* data;
    int opssize;
    operation* ops;
};
struct modboard {
    int size;
    module* array;
};

extern modboard* modbrd;

operation* createOperation(const char* name, void (*func)());
void deleteOperation(operation* op);
void addOp(operation op, module* mod);
operation* getOp(int x, module* mod);
operation* getOp(const char* x, module* mod);
module* createModule(const char* name, void* data);
void deleteModule(module* mod);
void addMod(module mod, modboard* brd);
module* getMod(int x, modboard* brd);
module* getMod(const char* x, modboard* brd);
modboard* createModBoard();
void deleteModBoard(modboard* brd);
void resetModBoard(modboard* brd);
