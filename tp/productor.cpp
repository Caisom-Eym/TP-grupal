#include "productor.h"
#include "producto.h"
#include "semaforo.h"
#include "numeroAleatorio.h"
#include "waitingQueue.h"

#include <iostream>
#include <deque>
#include <mutex>
#include <ctime>
#include <thread>

extern Semaforo hay_espacio;
extern Semaforo hay_producto;

std::mutex mtxProducidos;

std::mutex mtxGuardarWaiting; //Evita condicion de carrera en la funcion de productor
extern std::mutex mtxWaiting; //Evita condicion de carrera en la waiting deque
std::mutex mtxCout;

//Variables globales
//Obligatorias
int cantProductos; //Se establece en el main
extern int cantProducidos; //Cantidad de productos producidos o por producir

//Solo para determinar cuantos productos de X prioridad queremos
int cantP1 = -1; //-1 aleatorio. Cantidad de productos de prioridad 1
int cantP0 = -1; //-1 aleatorio. Cantidad de productos de prioridad 0
int cantP1Producidos = 0; //Cantidad de productos de prioridad 1 producidos o por producir
int cantP0Producidos = 0; //Cantidad de productos de prioridad 0 producidos o por producir


void productor(){
    mtxProducidos.lock();
    while (cantProducidos < cantProductos){
        Producto nuevoProducto;

        //Logica para la creacion de productos de una prioridad determinada
        int aleatorizarCreacionPrioridad;
        if (cantP1Producidos < cantP1 && cantP0Producidos < cantP0){
            aleatorizarCreacionPrioridad = aleatorio(0,5);
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
        }else nuevoProducto = crearProducto(cantProducidos, aleatorio(0,1));
        //Fin de la logica
        //nuevoProducto = crearProducto(cantProducidos, aleatorio(0,1));

        cantProducidos++;
        mtxProducidos.unlock();

        mtxGuardarWaiting.lock();

        guardarProductoWaiting(nuevoProducto);

        std::cout << nuevoProducto.prioridad << std::endl;
        mtxGuardarWaiting.unlock();

        signal(hay_producto);
        mtxProducidos.lock();
    }
    mtxProducidos.unlock();
}
void mostrar();


//Determinar cantidad de productos creados
//Determinar cuantos productos de X prioridad crear
int getCantProductos(){
    return cantProductos;
};

void setCantProductos(int nuevaCant){
    cantProductos = nuevaCant;
};
void setCantP1(int nuevaCant){
    cantP1 = nuevaCant;
};
void setCantP0(int nuevaCant){
    cantP0 = nuevaCant;
};
