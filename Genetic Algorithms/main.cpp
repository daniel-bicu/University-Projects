#include "menu.h"
#include <cstdio>
#include <ctime>

int main() {
    std::clock_t start;
    double duration;
    srand(time(NULL));
    start = std::clock();
    menu();
    duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    std::cout<<"Timp rulare:  "<< duration <<'\n';
    return 0;
}