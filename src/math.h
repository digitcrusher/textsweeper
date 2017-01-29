#pragma once
struct V {
    float x,y;
};
struct line {
    float x1,y1,x2,y2;
};
struct square {
    float x1,y1,x2,y2;
};

bool sqrInSqr(square sqr1, square sqr2);
bool fpil(line line, V point); //float point in line
bool fpilu(line line, V point, float unprec); //float point in line with unprecision
float fatp(V p1, V p2); //float angle to point
