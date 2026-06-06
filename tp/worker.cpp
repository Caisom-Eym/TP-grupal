#include "worker.h"

#include <deque>
#include <mutex>
#include <chrono>
#include <thread>
#include <iostream>

#include "processingQueue.h"
#include "producto.h"
#include "semaforo.h"
extern Semaforo hay_espacio;
extern Semaforo hay_producto_transportadora;
extern int cantProductos;
extern std::mutex mtx_consola;

void worker(){
    for (int i = 0; i < cantProductos; i++){
        wait(hay_producto_transportadora);

        quitarDeProcessing();
        mtx_consola.lock();
        std::cout << "Worker elimino un paquete de la Processing Queue" << std::endl;
        mtx_consola.unlock();

        signal(hay_espacio);
        std::this_thread::sleep_for(std::chrono::milliseconds(270));

    }
}
