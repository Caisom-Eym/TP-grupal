#include <deque>
#include <thread>
#include <mutex>
#include <chrono>
#include <iostream>

#include "producto.h"
#include "waitingQueue.h"
std::mutex mtxWaiting; //Evita condicion de carrera en la waiting deque
std::mutex mtxNuevasPeticiones;
extern std::mutex mtxCout;


std::chrono::steady_clock::time_point cambiarPrioridad = std::chrono::steady_clock::now();

int priorizar = 1;

std::deque<Producto> waiting;



void guardarProductoWaiting(Producto p){
    mtxNuevasPeticiones.lock();
    std::this_thread::sleep_for(std::chrono::milliseconds(90));
    mtxWaiting.lock();
    waiting.push_back(p);
    mtxWaiting.unlock();
    mtxNuevasPeticiones.unlock();
};

Producto consumirWaiting(){
    Producto retornar;
    mtxWaiting.lock();
    //Anti starvation //Cada 6000ms cambia la prioridad
    std::chrono::steady_clock::time_point ahora = std::chrono::steady_clock::now();
    int duracionPrograma = std::chrono::duration_cast<std::chrono::milliseconds>(ahora - cambiarPrioridad).count();
    if (duracionPrograma == 6000){
        cambiarPrioridad = std::chrono::steady_clock::now();
        if (priorizar == 1) priorizar = 0;
        else priorizar = 1;
        mtxCout.lock();
        std::cout << "Cambio de prioridad a " << priorizar << std::endl;
        mtxCout.unlock();
    }
    if (waiting.size() == 1 || getPrioridad(waiting.front()) == priorizar){
        retornar = waiting.front();
        waiting.pop_front();
    }else if (waiting.size() > 1){
        int indiceRetornar = 0;
        for (int i = 0; i < waiting.size(); i++){
            if (getPrioridad(waiting.at(i)) == priorizar){
                indiceRetornar = i;
                break;
            }
        }
        retornar = waiting.at(indiceRetornar);
        waiting.erase(waiting.begin()+indiceRetornar);
    }


    mtxWaiting.unlock();
    return retornar;
}
