#include "processingQueue.h"
#include <deque>
#include <mutex>
#include <chrono>
#include "producto.h"
#include "semaforo.h"
#include <thread>

std::deque<Producto> processing;

void agregarAProcessing(Producto p) {

    mtxProcessing.lock();

    std::this_thread::sleep_for(std::chrono::milliseconds(420));
    depositado(&p);
    processing.push_back(p);

    mtxProcessing.unlock();

};

void quitarDeProcessing() {

    mtxProcessing.lock();
    Producto p = processing.front();

    while (getDepositado(p) < 550){ //Espera a que el primer paquete del buffer este minimo 550ms en la cinta
        std::this_thread::sleep_for(std::chrono::milliseconds(5)); //Mejora de rendimiento
    }
    processing.pop_front();

    mtxProcessing.unlock();

};
