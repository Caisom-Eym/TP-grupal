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
int getTiempo(Producto p){

    auto ahora = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(ahora - p.fechaCreacion).count();
}

int getDepositado(Producto p){
    auto ahora = std::chrono::steady_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(ahora - p.depositadoCinta).count();
};


struct Producto crearProducto(int id, int prioridad){
    Producto p;
    p.id = id;
    p.prioridad = prioridad;
    p.fechaCreacion = std::chrono::steady_clock::now();

    return p;
};

void depositado(Producto* p){
    p->depositadoCinta = std::chrono::steady_clock::now();
}
