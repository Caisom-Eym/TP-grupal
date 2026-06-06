#include<random>
#include"numeroAleatorio.h"


//AleatorioVersion1 puede dar siempre 1 dependiendo del SO

int aleatorioVersion1(int minimo, int maximo){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(minimo, maximo);
    int retornar = dis(gen);

    return retornar;
}

int aleatorioVersion2(int minimo, int maximo)
{
    static std::mt19937 gen(std::random_device{}());

    std::uniform_int_distribution<int> dis(minimo, maximo);

    return dis(gen);
}
