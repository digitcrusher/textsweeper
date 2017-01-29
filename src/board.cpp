#include <iostream>
#include "board.h"

V tocheck[] = {{0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1},{-1,0},{-1,-1}};

using namespace std;

tile* getTile(int x, int y, board* brd) {
    if(sqrInSqr({0,0,brd->size.x-1,brd->size.y-1}, {(float)x,(float)y,(float)x,(float)y}))
        return brd->map+x+y*(int)brd->size.y;
    return brd->null;
}
void createBoard(board* brd, int w, int h, tile null) {
    brd->size.x = w;
    brd->size.y = h;
    brd->map = (tile*)malloc(sizeof(tile)*w*h);
    brd->null = (tile*)malloc(sizeof(tile));
    *brd->null = null;
}
void deleteBoard(board* brd) {
    free(brd->map);
    free(brd->null);
}
void resetBoard(board* brd, tile with) {
    for(int x=0;x<brd->size.x;x++) {
        for(int y=0;y<brd->size.y;y++) {
            tile* tile = getTile(x,y,brd);
            *tile = with;
        }
    }
}
void generateBoard(board* brd, int mines) {
    resetBoard(brd, *brd->null);
    putMines(brd, mines);
    calculateSums(brd);
}
void printBoard(board* brd) {
    cout<<" ";
    for(int i=0;i<brd->size.x;i++) {
        cout<<i;
    }
    cout<<"\n";
    for(int y=0;y<brd->size.y;y++) {
        cout<<y;
        for(int x=0;x<brd->size.x;x++) {
            tile* tile = getTile(x,y,brd);
            if(tile->flagged) {
                if(tile->wrong) {
                    cout<<"R";
                }else {
                    cout<<"P";
                }
            }else if(!tile->show) {
                cout<<"#";
            }else {
                if(tile->wrong) {
                    if(tile->mine) {
                        cout<<"X";
                    }else if(tile->showsum) {
                        cout<<tile->sum;
                    }else {
                        cout<<".";
                    }
                }else {
                    if(tile->mine) {
                        cout<<"*";
                    }else if(tile->showsum) {
                        cout<<tile->sum;
                    }else {
                        cout<<".";
                    }
                }
            }
        }
        cout<<"\n";
    }
}
void putMines(board* brd, int mines) {
    for(int i=0;i<mines;i++) {
        randomize:
        srand(rand()*time(0));
        int x=rand() % (int)brd->size.x;
        srand(rand()*time(0));
        int y=rand() % (int)brd->size.y;
        tile* tile = getTile(x,y,brd);
        if(tile->mine) {
            goto randomize;
        }
        tile->mine=1;
    }
}
void calculateSums(board* brd) {
    for(int x=0;x<brd->size.x;x++) {
        for(int y=0;y<brd->size.y;y++) {
            tile* on = getTile(x,y,brd);
            tile* to;
            int sum=0;
            for(int i=0;i<(int)(sizeof(tocheck)/sizeof(tocheck[0]));i++) {
                V pos = {x+tocheck[i].x,y+tocheck[i].y};
                if(!sqrInSqr({0,0,brd->size.x-1,brd->size.y-1}, {pos.x,pos.y,pos.x,pos.y})) {
                    continue;
                }
                to = getTile(pos.x,pos.y,brd);
                if(to->mine) {
                    sum+=1;
                }
            }
            if(sum>0) {
                on->showsum=1;
            }
            on->sum=sum;
        }
    }
}
void showAll(board* brd) {
    for(int x=0;x<brd->size.x;x++) {
        for(int y=0;y<brd->size.y;y++) {
            getTile(x,y,brd)->show = 1;
        }
    }
}
bool show(board* brd, int x, int y, bool recursion) {
    tile* on = getTile(x,y,brd);
    tile* to;
    bool fail=0;
    if(on->flagged) {
        return 0;
    }
    on->show=1;
    if(on->mine) {
        on->wrong=1;
        return 1;
    }
    if(!recursion && !on->showsum) {
        for(int i=0;i<(int)(sizeof(tocheck)/sizeof(tocheck[0]));i++) {
            V pos = {x+tocheck[i].x,y+tocheck[i].y};
            if(!sqrInSqr({0,0,brd->size.x-1,brd->size.y-1}, {pos.x,pos.y,pos.x,pos.y})) {
                continue;
            }
            to = getTile(pos.x,pos.y,brd);
            if(!to->show) {
                if((fail = show(brd,x+tocheck[i].x,y+tocheck[i].y,to->showsum))) {
                    break;
                }
            }
        }
    }
    return fail;
}
