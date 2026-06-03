#include <iostream>
#include <thread>
#include <queue>
#include <deque>
#include <vector>
#include <ctime>
#include "producto.h"
#include "semaforo.h"
#include "productor.h"
#include "numeroAleatorio.h"

Semaforo hay_producto; //Semaforo que marca la cantidad de items en la estanteria
Semaforo hay_espacio; //Semaforo que dice si hay espacio libre en la cinta transportadora
Semaforo hay_producto_transportadora; //Semaforo que marca la cantidad de items en la transportadora

std::mutex mtxProcessing; //Evita condicion de carrera en la processing queue


int main(){
    std::vector<std::thread> hilosProductores;
    const int cantidadProductores = 100;

    setCantProductos(100);
    setCantP0(5);
    setCantP1(20);

    init(hay_producto,0);
    init(hay_espacio,5);
    init(hay_producto_transportadora, 0);

    for (int i = 0; i < cantidadProductores; ++i) {
        hilosProductores.emplace_back(productor);
    }

    for (int i = 0; i < cantidadProductores; i++) hilosProductores.at(i).join();

    return 0;
}
