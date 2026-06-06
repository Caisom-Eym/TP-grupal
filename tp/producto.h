#ifndef PRODUCTO_H_INCLUDED
#define PRODUCTO_H_INCLUDED

#include <ctime>
#include <string>
#include <chrono>

//Se deberia de crear un puntero y declarar los campos en el .cpp
//Pero para simplificar el trabajo practico, definimos los campos en el header
struct Producto{
    int id;
    int prioridad;
    //time_t fechaCreacion;
    std::chrono::steady_clock::time_point fechaCreacion;
    std::chrono::steady_clock::time_point depositadoCinta; //Probablemente lo cambie
};

int getId(Producto p);
int getPrioridad(Producto p);
//std::string getFecha(Producto p);
int getTiempo(Producto p); //Devuelve el tiempo que estuvo depositado
int getDepositado(Producto p);
//No se debe de poder establecer otros valors en los Productos


struct Producto crearProducto(int id, int prioridad);
void depositado(Producto* p);

#endif // PRODUCTO_H_INCLUDED
