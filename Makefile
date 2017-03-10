# Makefile
# Textsweeper Source Code
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
CCFLAGS=-Wall -std=c++11 -ggdb -O0
CCINCLUDE=./
CCLIBS=-L/usr/lib/X11R6/lib -lX11

all: clean textsweeper

textsweeper: main.o math.o module.o terminal.o textsweeper/board.o textsweeper/textsweeper.o utils.o
	$(CC) $(BD)/main.o $(BD)/math.o $(BD)/module.o $(BD)/terminal.o \
		$(BD)/board.o $(BD)/textsweeper.o $(BD)/utils.o $(CCLIBS) -o $(BD)/textsweeper

main.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/main.cpp -c -o $(BD)/main.o

math.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/math.cpp -c -o $(BD)/math.o

module.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/module.cpp -c -o $(BD)/module.o

terminal.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/terminal.cpp -c -o $(BD)/terminal.o

textsweeper/board.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/textsweeper/board.cpp -c -o $(BD)/board.o

textsweeper/textsweeper.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/textsweeper/textsweeper.cpp -c -o $(BD)/textsweeper.o

utils.o:
	$(CC) $(CCFLAGS) -I $(CCINCLUDE) $(SD)/utils.cpp -c -o $(BD)/utils.o

clean:
	rm -f $(BD)/textsweeper $(BD)/main.o $(BD)/math.o $(BD)/module.o $(BD)/terminal.o \
		$(BD)/textsweeper/board.o $(BD)/textsweeper/textsweeper.o $(BD)/utils.o
