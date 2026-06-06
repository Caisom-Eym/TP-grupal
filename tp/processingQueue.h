#ifndef PROCESSINGQUEUE_H_INCLUDED
#define PROCESSINGQUEUE_H_INCLUDED

#include <deque>
#include <mutex>
#include "producto.h"
#include "semaforo.h"

// recursos compartidos
extern std::mutex mtxProcessing;
extern std::deque<Producto> processing;

void agregarAProcessing(Producto p);
void quitarDeProcessing();



#endif // PROCESSINGQUEUE_H_INCLUDED
