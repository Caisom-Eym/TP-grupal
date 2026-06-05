#include <iostream>
#include <thread>
#include <queue>
#include <deque>
#include <vector>
#include <chrono>;
#include "producto.h"
#include "semaforo.h"
#include "productor.h"
#include "numeroAleatorio.h"
//Comentario de github
Semaforo hay_producto; //Semaforo que marca la cantidad de items en la estanteria
Semaforo hay_espacio; //Semaforo que dice si hay espacio libre en la cinta transportadora
Semaforo hay_producto_transportadora; //Semaforo que marca la cantidad de items en la transportadora

int cantProducidos = 0; //Cantidad de productos producidos o por producir
std::vector<int> promedioEsperaProduccion1; //Tiempo promedio de espera de paquetes producidos (discriminados por prioridad)
std::vector<int> promedioEsperaProduccion0; //Prioridad 0

std::mutex mtxProcessing; //Evita condicion de carrera en la processing queue


int main(){
    std::vector<std::thread> hilosProductores;
    const int cantidadProductores = 100;

    setCantProductos(100);
    setCantP0(0);
    setCantP1(100);

    init(hay_producto,0);
    init(hay_espacio,5);
    init(hay_producto_transportadora, 0);

    for (int i = 0; i < cantidadProductores; ++i) {
        hilosProductores.emplace_back(productor);
    }

    for (int i = 0; i < cantidadProductores; i++) hilosProductores.at(i).join();

    int sumaTiempoP1 = 0;
    for (int i = 0; i < promedioEsperaProduccion1.size(); i++){
        sumaTiempoP1 += promedioEsperaProduccion1.at(i);
    }

    int sumaTiempoP0 = 0;
    for (int i = 0; i < promedioEsperaProduccion0.size(); i++){
        sumaTiempoP0 += promedioEsperaProduccion0.at(i);
    }

    int promedio1 = 0;
    int promedio0 = 0;
    if (sumaTiempoP1 != 1){
        promedio1 = sumaTiempoP1/promedioEsperaProduccion1.size();
        std::cout << "Promedio de Espera de Produccion de prioridad 1: " << promedio1 << "ms" << std::endl;
    }else std::cout << "Sin productos de prioridad 1" << std::endl;
    if (sumaTiempoP0 != 0) {
        promedio0 = sumaTiempoP0/promedioEsperaProduccion0.size();
        std::cout << "Promedio de Espera de Produccion de prioridad 0: " << promedio0 << "ms" << std::endl;
    }else std::cout << "Sin productos de prioridad 0"  << std::endl;





    std::cout << "Cantidad de paquetes: " << cantProducidos<< std::endl;

    return 0;
}
