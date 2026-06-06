#ifndef WAITINGQUEUE_H_INCLUDED
#define WAITINGQUEUE_H_INCLUDED

#include <deque>

#include "producto.h"

void guardarProductoWaiting(Producto p);

Producto consumirWaiting();

#endif // WAITINGQUEUE_H_INCLUDED
