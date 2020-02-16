#include <iostream>
#include <cmath>
#include <cfloat>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>

#define dimension 48

using namespace std;

ifstream fin("input_tsp.txt");


double cost[dimension][dimension];
int ccurent=0;
int start;
int vizitat[dimension];
double costuri[105];
int mutat[dimension];
int crossover[105];
double minimul=FLT_MAX;
double cost_normalizat[105];
double cost_intreg=0;


double euristic(){

    int next=0;
    int precedent;
//    long double tert=0;
    double cost1=0;
    start=rand()%dimension;
    precedent=start;
    for (int i = 0; i < dimension; ++i) {
        vizitat[i]=0;
    }
    vizitat[start]=1;

    ccurent=1;
    next=start;

    while (ccurent<dimension) {
        while (vizitat[next] == 1) {
            next = rand() % dimension;
        }
        cost1+=cost[precedent][next];
        vizitat[next]=1;
        precedent=next;
        ccurent++;
//        tert+=cost1;
    }
//    tert=tert/16;
//    cout<<"medie: "<<tert<<endl;
    return cost1;
}
