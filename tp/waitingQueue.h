#ifndef WAITINGQUEUE.H
#define WAITINGQUEUE.H

#include <deque>

#include "producto.h"

void guardarProductoWaiting(Producto p);

Producto consumirWaiting();

#endif // WAITINGQUEUE
