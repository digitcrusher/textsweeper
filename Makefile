# Makefile
# textsweeper Source Code
# Available on Github
#
# Copyright (C) 2017 Karol "digitcrusher" Łacina
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
SD=./src
BD=./build
CC=g++
CCFLAGS=-Wall -Wno-write-strings -std=c++11 -ggdb -O0 -I.
LD=$(CC)
LDFLAGS=
LDLIBS=-L./karolslib/build/ -lkarolslib -L/usr/lib/X11R6/lib -lX11
#For windows:
#LDFLAGS=-mwindows
#LDLIBS=-L./karolslib/build/ -lkarolslib -L/usr/lib/X11R6/lib -lX11

all: clean textsweeper

textsweeper: board.o main.o
	$(LD) $(LDFLAGS) $(BD)/board.o $(BD)/main.o $(LDLIBS) -o $(BD)/textsweeper

board.o:
	$(CC) $(CCFLAGS) $(SD)/board.cpp -c -o $(BD)/board.o

main.o:
	$(CC) $(CCFLAGS) $(SD)/main.cpp -c -o $(BD)/main.o

clean:
	rm -f $(BD)/textsweeper $(BD)/board.o $(BD)/main.o
