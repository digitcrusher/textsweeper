/*
 * main.cpp
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
#include <iostream>
#include <src/module.h>
#include <src/terminal.h>
#include <src/textsweeper/textsweeper.h>

int main(int argc, char** argv) {
    //Inform the user that we use GPLv3
    std::cout<<"textsweeper Copyright (C) 2017 Karol \"digitcrusher\" Łacina"<<'\n';
    std::cout<<"This program comes with ABSOLUTELY NO WARRANTY."<<'\n';
    std::cout<<"This is free software, and you are welcome to redistribute it"<<'\n';
    std::cout<<"under certain conditions."<<'\n';
    //Our terminal version :)
    swrite(stdterm, "textsweeper Copyright (C) 2017 Karol \"digitcrusher\" Łacina\n");
    swrite(stdterm, "This program comes with ABSOLUTELY NO WARRANTY.\n");
    swrite(stdterm, "This is free software, and you are welcome to redistribute it\n");
    swrite(stdterm, "under certain conditions.\n");
    //Create module main and add it to the global modboard modbrd.
    addMod(*createModule("main", NULL), modbrd);
    //Create routine main pointing to casted mainf and add it to module main from modbrd.
    addRtn(*createRoutine("main", (void (*)())run), getMod("main", modbrd));
    //Create init.
    addRtn(*createRoutine("init", (void (*)())init), getMod("main", modbrd));
    //Iterate over the modules from modbrd.
    for(int i=0; i<modbrd->size; i++) {
        routine* rtn;
        //Rtn has init? If yes call it.
        if((rtn = getRtn("init", getMod(i, modbrd)))) {
            rtn->func();
        }
    }
    /*
     * getRtn("main", getMod("main", modbrd))->func) Get function pointer from main which is from main.
     * ((int (*)(int, char**)) Cast it to a function which returns int and accepts int and char** as arguments/
     * (argc, argv) Call it with argc and argv
     */
    return ((int (*)(int, char**))getRtn("main", getMod("main", modbrd))->func)(argc, argv);
}
