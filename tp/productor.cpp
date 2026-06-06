#include "productor.h"
#include "producto.h"
#include "semaforo.h"
#include "numeroAleatorio.h"
#include "waitingQueue.h"

#include <iostream>
#include <deque>
#include <vector>
#include <mutex>
#include <chrono>
#include <thread>

extern Semaforo hay_espacio;
extern Semaforo hay_producto;

std::mutex mtxProducidos;

std::mutex mtxEsperaProduccion; //Evita condicion de carrera en la funcion de productor
extern std::mutex mtx_consola;

extern std::vector<int> promedioEsperaProduccion1; //Tiempo promedio de espera de paquetes producidos (discriminados por prioridad)
extern std::vector<int> promedioEsperaProduccion0; //Prioridad 0

std::chrono::steady_clock::time_point esperaProduccion1 = std::chrono::steady_clock::now();
std::chrono::steady_clock::time_point esperaProduccion0 = std::chrono::steady_clock::now();


//Variables globales
//Obligatorias
extern int cantProductos; //Se establece en el main
extern int cantProducidos; //Cantidad de productos producidos o por producir

//Solo para determinar cuantos productos de X prioridad queremos
extern int cantP1; // Cantidad de productos de prioridad 1
extern int cantP0; // Cantidad de productos de prioridad 0
int cantP1Producidos = 0; //Cantidad de productos de prioridad 1 producidos o por producir
int cantP0Producidos = 0; //Cantidad de productos de prioridad 0 producidos o por producir


void productor(int id){
    mtxProducidos.lock();
    while (cantProducidos < cantProductos){

        Producto nuevoProducto;

        //Logica para la creacion de productos de una prioridad determinada
        int aleatorizarCreacionPrioridad;

        if (cantP1Producidos < cantP1 && cantP0Producidos < cantP0){
            aleatorizarCreacionPrioridad = aleatorioVersion2(0,5);

            if (aleatorizarCreacionPrioridad >= 1) {
                cantP1Producidos++;
                nuevoProducto = crearProducto(cantProducidos, 1);

            }else if (aleatorizarCreacionPrioridad == 0){
                nuevoProducto = crearProducto(cantProducidos, 0);
                cantP0Producidos++;
            }

        }else if (cantP1 > 0 && cantP1Producidos < cantP1) {
            cantP1Producidos++;
            nuevoProducto = crearProducto(cantProducidos, 1);

        }else if (cantP0 > 0 && cantP0Producidos < cantP0){
            nuevoProducto = crearProducto(cantProducidos, 0);
            cantP0Producidos++;

        }else nuevoProducto = crearProducto(cantProducidos, aleatorioVersion2(0,1));
        //Fin de la logica
        //nuevoProducto = crearProducto(cantProducidos, aleatorio(0,1));

        cantProducidos++;
        mtxProducidos.unlock();

        guardarProductoWaiting(nuevoProducto);

        mtx_consola.lock();
        std::cout << "Productor [" << id << "] puso paquete ID: " << nuevoProducto.id << " en estanteria (Total: " << cantProducidos << ")" << std::endl;
        mtx_consola.unlock();
        /*
        std::chrono::steady_clock::time_point auxEspera = std::chrono::steady_clock::now();;
        mtxEsperaProduccion.lock();

        if (getPrioridad(nuevoProducto) == 1){
            int promedio = std::chrono::duration_cast<std::chrono::milliseconds>(auxEspera - esperaProduccion1).count();
            esperaProduccion1 = std::chrono::steady_clock::now();
            promedioEsperaProduccion1.push_back(promedio);

        }else{
            ///int promedio = std::chrono::duration_cast<std::chrono::milliseconds>(auxEspera - esperaProduccion0).count(); calcula tiempo que pasa entre q se crea un paquete y otro
            esperaProduccion0 = std::chrono::steady_clock::now();
            promedioEsperaProduccion0.push_back(promedio);
        }

        mtxEsperaProduccion.unlock();*/

        signal(hay_producto);
        mtxProducidos.lock();
    }
    mtxProducidos.unlock();

}
