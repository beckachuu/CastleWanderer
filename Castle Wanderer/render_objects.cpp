#include "render_objects.h"

Object::Object(int _posY, int order, void (*_renderFunc) (int, SDL_Renderer*)) {
	posY = _posY;
	renderFunc = _renderFunc;
	next = nullptr;
	prev = nullptr;
}

Object::~Object() {

}

Object* Object::addNext(Object* min, int dataPosY, int orderData, void (*renderFunc) (int, SDL_Renderer*)) {
    Object* newObject = new Object(dataPosY, orderData, renderFunc);

    Object* p = min;
    if (dataPosY > min->posY) {
        for (; dataPosY > p->posY && p->next != nullptr; p = p->next) {}

        if (p->next == nullptr && dataPosY > p->posY) {
            p->next = newObject;
            newObject->prev = p;
        }
        else {
            newObject->prev = p->prev;
            newObject->next = p;

            p->prev->next = newObject;
            p->prev = newObject;
        }

        return min;
    }
    else {
        newObject->next = p;
        p->prev = newObject;
        min = newObject;
        return min;
    }
}

void Object::renderObjs(Object* min, SDL_Renderer* render) {
    for (Object* p = min; p != nullptr; p = p->next) {
        p->renderFunc(p->order, render);
    }
}
