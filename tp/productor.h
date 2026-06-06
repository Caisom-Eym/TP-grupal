#ifndef PRODUCTOR_H_INCLUDED
#define PRODUCTOR_H_INCLUDED

#include "producto.h"

void productor(int id);
void mostrar();


//Determinar cantidad de productos creados
//Determinar cuantos productos de X prioridad crear
int getCantProductos();

void setCantProductos(int nuevaCant);
void setCantP1(int nuevaCant);
void setCantP0(int nuevaCant);

#endif // PRODUCTOR_H_INCLUDED
