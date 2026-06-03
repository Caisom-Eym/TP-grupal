#include<random>
#include"numeroAleatorio.h"


int aleatorio(int minimo, int maximo){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minimo, maximo);
    int retornar = dis(gen);

    return retornar;
}
