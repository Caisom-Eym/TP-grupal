#include <ctime>
#include <chrono>
#include <iostream>
#include "producto.h"
#include <string>

int getId(Producto p){
    return p.id;
}
int getPrioridad(Producto p){
    return p.prioridad;
};

std::string getFecha(Producto p){
    std::string retornar = ctime(&p.fechaCreacion);
    return retornar;
}

int getTiempo(Producto p){
    std::chrono::steady_clock::time_point ahora = std::chrono::steady_clock::now();

    return std::chrono::duration_cast<std::chrono::milliseconds>(ahora - p.depositadoCinta).count();
}

//No se debe de poder establecer otros valors en los Productos

struct Producto crearProducto(int id, int prioridad){
    Producto p;
    p.id = id;
    p.prioridad = prioridad;
    p.fechaCreacion = time(NULL);

    return p;
};

void depositado(Producto* p){ //Opcional
    p->depositadoCinta = std::chrono::steady_clock::now();
}


