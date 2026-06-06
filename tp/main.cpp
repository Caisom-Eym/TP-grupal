#include <iostream>
#include <thread>
#include <queue>
#include <deque>
#include <vector>
#include <chrono>
#include "worker.h"
#include "producto.h"
#include "semaforo.h"
#include "productor.h"
#include "numeroAleatorio.h"
#include "consumidor.h"
#include "processingQueue.h"

Semaforo hay_producto; //Semaforo que marca la cantidad de items en la estanteria
Semaforo hay_espacio; //Semaforo que dice si hay espacio libre en la cinta transportadora
Semaforo hay_producto_transportadora; //Semaforo que marca la cantidad de items en la transportadora

int cantProducidos = 0; //Cantidad de productos producidos o por producir

std::vector<int> promedioEsperaProduccion1; //Tiempo promedio de espera de paquetes producidos (discriminados por prioridad)
std::vector<int> promedioEsperaProduccion0; //Prioridad 0

std::mutex mtxProcessing; //Evita condicion de carrera en la processing queue
std::mutex mtxCout; //Evita malos prints en la consola

 // definir cantidades según la Configuración
const int cantidadProductores = 3;
const int cantidadConsumidores = 3;
int cantProductos = 10; // Para la Prueba de Carga Masiva
int cantP0 = 0; // Cantidad de productos de prioridad 0
int cantP1 = 0; // Cantidad de productos de prioridad 1

int main(){

   auto inicio_programa = std::chrono::steady_clock::now();

    // inicializar semáforos
    init(hay_producto, 0); // Estantería vacía
    init(hay_espacio, 5); // Cinta con 5 lugares
    init(hay_producto_transportadora, 0);

    // productores
    std::vector<std::thread> hilosProductores;
    for (int i = 0; i < cantidadProductores; ++i) {
        hilosProductores.emplace_back(productor, i + 1);
    }

    // consumidores
    std::vector<std::thread> hilosConsumidores;
    for (int i = 0; i < cantidadConsumidores; ++i) {
        hilosConsumidores.emplace_back(consumidor, i + 1, cantidadConsumidores);
    }

    std::thread w(worker);

    // 5. Esperar a que todos terminen
    for (auto& t : hilosProductores) t.join();
    for (auto& t : hilosConsumidores) t.join();
    w.join();

    std::cout << "Cantidad de paquetes: " << cantProducidos<< std::endl;

    std::cout << "Estado final de la cinta transportadora: ";
    if (processing.empty()) {
        std::cout << "La cinta esta vacio (se consumio todo)." << std::endl;
    } else {
        std::cout << "Quedaron " << processing.size() << " elementos. Fueron consumidos " << std::endl;
        while (!processing.empty()) {
            std::cout << "[" << processing.front().id << "] " << std::endl;
            processing.pop_front();
        }
        std::cout << std::endl;
    }

    std::cout << "--- REPORTE FINAL ---" << std::endl;

    // metricas para prioridad 1 (Alta)

    if (!promedioEsperaProduccion1.empty()) {
        int suma = 0;
        for (int t : promedioEsperaProduccion1) suma += t;
        std::cout << "Promedio de Espera Prioridad 1: " << suma / promedioEsperaProduccion1.size() << "ms" << std::endl;
    } else {
        std::cout << "Sin productos de prioridad 1" << std::endl;
    }

    // metricas para prioridad 0 (Baja)

    if (!promedioEsperaProduccion0.empty()) {
        int suma = 0;
        for (int t : promedioEsperaProduccion0) suma += t;
        std::cout << "Promedio de Espera Prioridad 0: " << suma / promedioEsperaProduccion0.size() << "ms" << std::endl;
    } else {
        std::cout << "Sin productos de prioridad 0" << std::endl;
    }


     // Capturar tiempo de fin
    auto fin_programa = std::chrono::steady_clock::now();

    auto duracion = std::chrono::duration_cast<std::chrono::milliseconds>(fin_programa - inicio_programa);

    std::cout << "------------------------------------------" << std::endl;
    std::cout << "TIEMPO TOTAL DE EJECUCION: " << duracion.count() << " ms" << std::endl;
    std::cout << "------------------------------------------" << std::endl;

    return 0;
}
