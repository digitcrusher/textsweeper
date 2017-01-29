/*
utils.h
Textsweeper Source Code Available on Github
Author: Karol "digitcrusher" Łacina 2017
*/
#pragma once
#if defined(WIN32) && defined(_WIN32) && defined(__WIN32)
long getMS();
void hidecursor();
#else
int getch();
#endif
bool stof(char* str, float* to);
int intlen(int ntgr);
char* itos(int input, char* msg);
char* itos(int input);
int htod(char* hex);
int indexof(const char* string, char search);

template<typename T> class Vector {
    private:
        int vsize;
        const int maxsize=2147483647;
        T* array;
    public:
        Vector();
        ~Vector();
        bool push_back(T t);
        bool pop_back(T* t);
        bool replace(int element, T t);
        bool resize(int newsize);
        bool getP(int element, T* t);
        bool clear();
        T* getArray();
        int size();
        T operator[](int n);
};
