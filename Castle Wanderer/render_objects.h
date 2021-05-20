#pragma once
#include "basic_init.h"

struct Object
{
    Object(int posY = 0, int order = -1, void (*renderFunc) (int, SDL_Renderer*) = nullptr);
    ~Object();

    Object* addNext(Object* min, int dataPosY, int orderData, void (*_renderFunc) (int, SDL_Renderer*));
    void renderObjs(Object* min, SDL_Renderer* render);


    int posY;
    int order;
    void (*renderFunc) (int, SDL_Renderer*);

    Object* next;
    Object* prev;
};

