#include "consumidor.h"
#include "waitingQueue.h"
#include "processingQueue.h"
#include "semaforo.h"
#include "producto.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>

// Recursos compartidos
extern Semaforo hay_producto; //Semaforo que marca la cantidad de items en la estanteria
extern Semaforo hay_espacio; //Semaforo que dice si hay espacio libre en la cinta transportadora
extern Semaforo hay_producto_transportadora;
extern int cantProductos;
extern std::vector<int> promedioEsperaProduccion1;
extern std::vector<int> promedioEsperaProduccion0;
extern std::mutex mtxEsperaProduccion;

std::mutex mtx_consola; // Para exclusión mutua de la escritura de finalización
std::mutex mtx_consumo; // protege a la variable global consumo para la verificacion final

const int tam = 5;
int consumidos = 0;

void consumidor(int id, int cantConsumidores)
{
    int trabajoRepartido = cantProductos/cantConsumidores;
    int resto = cantProductos % cantConsumidores;

    if(id == 1)   // le asigno directamente al consumidor 1 (en este caso) el resto
    {
        trabajoRepartido+= resto;
    }

    for(int i = 0; i < trabajoRepartido; i++){
        // espera pasiva, espera a que el productor ponga algo en la estantería
        wait(hay_producto);

        Producto p = consumirWaiting(); //saca paquete de estanteria

        // tiempo que esperó en la estantería
        int tiempoEspera = getTiempo(p);
        mtxEsperaProduccion.lock();

        if(p.prioridad == 1) promedioEsperaProduccion1.push_back(tiempoEspera);
        else promedioEsperaProduccion0.push_back(tiempoEspera);

        mtxEsperaProduccion.unlock();

        wait(hay_espacio); // Espera que la cinta (máx 5) tenga lugar
        agregarAProcessing(p); // Retardo de 420ms interno
        signal(hay_producto_transportadora); //Señal al worker

        mtx_consola.lock(); // Mutex para proteger la consola
        std::cout << "Consumidor [" << id << "] consumio el paquete: " << getId(p) << " Prioridad: " << getPrioridad(p) << std::endl;
        mtx_consola.unlock();
    }

    auto ahora = std::chrono::system_clock::now(); // guarda el tiempo en que el hilo finaliza
    std::time_t tiempo_final = std::chrono::system_clock::to_time_t(ahora); // lo convierte en formato visible

    mtx_consola.lock(); // Mutex para proteger la consola
    std::cout << "Consumidor [" << id << "] finalizo su ejecucion en: " << std::ctime(&tiempo_final);
    mtx_consola.unlock();

}
