/*
modules.cpp
Textsweeper Source Code Available on Github
Author: Karol "digitcrusher" Łacina 2017
*/
#include <stdlib.h>
#include <string.h>
#include "modules.h"

modboard* modbrd = createModBoard();

operation* createOperation(const char* name, void (*func)()) {
    operation* op = (operation*)malloc(sizeof(operation));
    op->name = (char*)malloc(sizeof(char)*strlen(name));
    *op->name = '\0';
    strcpy(op->name, name);
    op->func = func;
    return op;
}
void deleteOperation(operation& op) {
    free(op.name);
}
void deleteOperation(operation* op) {
    return deleteOperation(*op);
}
void addOp(operation op, module& mod) {
    mod.opssize++;
    mod.ops = (operation*)realloc((void*)mod.ops, sizeof(operation)*mod.opssize);
    *(mod.ops+mod.opssize-1) = op;
}
void addOp(operation op, module* mod) {
    return addOp(op, *mod);
}
operation* getOp(int x, module& mod) {
    if(x >= 0 && x<mod.opssize)
        return mod.ops+x;
    return NULL;
}
operation* getOp(int x, module* mod) {
    return getOp(x, *mod);
}
operation* getOp(const char* x, module& mod) {
    for(int i=0;i >= 0 && i<mod.opssize;i++) {
        if(!strcmp((mod.ops+i)->name, x)) {
            return mod.ops+i;
        }
    }
    return NULL;
}
operation* getOp(const char* x, module* mod) {
    return getOp(x, *mod);
}
module* createModule(const char* name, void* data) {
    module* mod = (module*)malloc(sizeof(module));
    mod->name = (char*)malloc(sizeof(char)*strlen(name));
    *mod->name = '\0';
    strcpy(mod->name, name);
    mod->data = data;
    mod->opssize = 0;
    mod->ops = (operation*)malloc(sizeof(operation)*mod->opssize);
    return mod;
}
void deleteModule(module& mod) {
    free(mod.name);
    free(mod.data);
    free(mod.ops);
}
void deleteModule(module* mod) {
    return deleteModule(*mod);
}
void addMod(module mod, modboard& brd) {
    brd.size++;
    brd.array = (module*)realloc((void*)brd.array, sizeof(module)*brd.size);
    *(brd.array+brd.size-1) = mod;
}
void addMod(module mod, modboard* brd) {
    return addMod(mod, *brd);
}
module* getMod(int x, modboard& brd) {
    if(x >= 0 && x<brd.size)
        return brd.array+x;
    return NULL;
}
module* getMod(int x, modboard* brd) {
    return getMod(x, *brd);
}
module* getMod(const char* x, modboard& brd) {
    for(int i=0;i >= 0 && i<brd.size;i++) {
        if(!strcmp((brd.array+i)->name, x)) {
            return brd.array+i;
        }
    }
    return NULL;
}
module* getMod(const char* x, modboard* brd) {
    return getMod(x, *brd);
}
modboard* createModBoard() {
    modboard* brd = (modboard*)malloc(sizeof(modboard));
    brd->size = 0;
    brd->array = (module*)malloc(sizeof(module)*brd->size);
    return brd;
}
void deleteModBoard(modboard& brd) {
    free(brd.array);
}
void deleteModBoard(modboard* brd) {
    return deleteModBoard(*brd);
}
void resetModBoard(modboard& brd) {
    brd.size = 0;
    free(brd.array);
    brd.array = (module*)malloc(sizeof(module)*brd.size);
}
void resetModBoard(modboard* brd) {
    return resetModBoard(*brd);
}
