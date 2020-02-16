#include <iostream>
#include <cmath>
#include <cfloat>
#include <vector>
#include <cstdlib>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <fstream>
#define CR_PROB 0.3
#define M_PROB 0.01
#define dimension 48


using namespace std;




float RandomDouble(float minVal, float maxVal) {
    float valoare;
    valoare = (float)rand() / RAND_MAX;
    return minVal + valoare * (maxVal - minVal);
}


void buid_population(int pop[100][dimension]){
    int ales[dimension];
    for (int k = 0; k < dimension; ++k) {
        ales[k]=0;
    }
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < dimension; ++j) {
            pop[i][j]=rand()%dimension;
            while (ales[pop[i][j]] == 1){ 
                pop[i][j]=rand()%dimension;
            }
            ales[pop[i][j]]=1;
        }
        for (int k = 0; k < dimension; ++k) { 
            ales[k]=0;
        }
    }
}

void evaluation(int pop[100][dimension]){

    for (int l = 0; l < 50; ++l) {
        costuri[l]=0;
    }

    int aux=0;
    int aux1=0;

    for (int k = 0; k < 100; ++k) {
        for (int i = 1; i < dimension; ++i) {
            aux=pop[k][i-1];
            aux1=pop[k][i];
            costuri[k]+=cost[aux][aux1];
        }
    }
    for (int j = 0; j < 100; ++j) {
        if(minimul>costuri[j]){
            minimul=costuri[j];
        }
    }
}

void mutatie(int pop[100][dimension]){
    double nr=RandomDouble(0,1);
    int count=0;
    for (int i = 0; i < 100; ++i) {


        for (int l = 0; l < dimension; ++l) {
            mutat[l]=0;
        }

        count=0;
        for (int j = 0; j < dimension; ++j) {
            if(nr<M_PROB){
                mutat[count]=j;
                count++;
            }
            nr=RandomDouble(0,1);

        }

        if(count%2 == 1){
            count--;
        }
        
        for (int k = 0; k < count; k+=2) {
            swap(pop[i][mutat[k]],pop[i][mutat[k+1]]);
        }


    }
}

int vizitari[dimension];


void crosstwocrom(int pop[100][dimension],int lin1,int lin2){
    int x1,x2,y1,y2;
    y1=rand()%dimension;
    y2=rand()%(dimension-y1)+y1;
    x1=rand()%dimension;
    x2=rand()%(dimension-x1)+x1;
    int i=0,k=0;
    int vector1[dimension];
    int vector2[dimension];
    for (int l = 0; l < dimension; ++l) {
        vizitari[l]=0;
    }

    for(int j=x1;j<=x2;j++)
    {
        vector1[i]=pop[lin1][j];
        vizitari[pop[lin1][j]]=1;
        i++;
    }
    k=i;
    i=0;
    while(i<dimension)
    {
        if(vizitari[pop[lin2][i]]==0) {
            vector1[k]=pop[lin2][i];
            vizitari[pop[lin2][i]]=1;
            k++;
            i++;
        } else {
            i++;
        }
    }

///---------------------><---------------------///

    for (int l = 0; l < dimension; ++l) {
        vizitari[l]=0;
    }
    i=0;
    k=0;
    for(int j=y1;j<=y2;j++)
    {
        vector2[i]=pop[lin2][j];
        vizitari[pop[lin2][j]]=1;
        i++;
    }
    k=i;
    i=0;
    while(i<dimension)
    {
        if(vizitari[pop[lin1][i]]==0) {
            vector2[k]=pop[lin1][i];
            vizitari[pop[lin1][i]]=1;
            k++;
            i++;
        } else {
            i++;
        }
    }
    for (int m = 0; m < dimension; ++m) {
        pop[lin1][m]=vector1[m];
        pop[lin2][m]=vector2[m];
    }
}

void cross(int pop[50][dimension]){
    double nr;
    int count=0;
    for (int i = 0; i < 100; ++i) {
        nr=RandomDouble(0,1);
        if(nr<CR_PROB) {
            crossover[count] = i;
            count++;

        }
    }
    if(count%2==1){
        count--;
    }
    for (int j = 0; j < count; j=j+2) {
        crosstwocrom(pop,crossover[j],crossover[j+1]);
    }
}

bool AProgresat(int last,int curent){
    if(curent-last>400){
        return false;
    } else {
        return true;
    }
}

void Selection(int pop[100][dimension]){
    double nr=0;
    int auxpop[100][dimension];
    for (int m = 0; m < 100; ++m) {
        for (int i = 0; i < dimension; ++i) {
            auxpop[m][i]=0;
        }
    }
    cost_intreg=0;
    for (int i = 0; i < 100; ++i) {
        cost_intreg+=costuri[i];
    }
    cost_normalizat[0]=0;
    cost_normalizat[1]=costuri[0]/cost_intreg;
    for (int j = 2; j < 101; ++j) {
        cost_normalizat[j]=cost_normalizat[j-1]+costuri[j]/cost_intreg;
    }
    cost_normalizat[100]=1;
    int i=0;
    while (i<100){
        nr=RandomDouble(0,1);
        for (int j = 1; j < 102; ++j) {
            if(cost_normalizat[j-1]<nr && nr<cost_normalizat[j]){
                for (int k = 0; k < dimension; ++k) {
                    auxpop[i][k]=pop[j-1][k];
                }
                break;
            }
        }
        i++;
    }
    for (int l = 0; l < 100; ++l) {
        for (int j = 0; j < dimension; ++j) {
            pop[l][j]=auxpop[l][j];
        }
    }
}


long double medie=0;
double genetic(){

    int population[100][dimension];
    int generatii=1;
    int curent=0;
    int last=0;
    double progres=0;
    double progres1=0;
    double maxim=-FLT_MAX;
    minimul=FLT_MAX;
    ccurent = 0;
    cost_intreg = 0;
    progres =0 ;


    buid_population(population); //pop<- random
    evaluation(population); // pop de cost min (doar minim)
    curent=last=1;
    progres=minimul;
    medie=minimul;
    progres1=minimul;

    while(AProgresat(last,curent)){
        mutatie(population);
        cross(population);
        Selection(population);
        evaluation(population);
        if(progres>minimul){
            progres=minimul;
            curent++;
            last=generatii;
        } else if(progres1<minimul) {
            progres1=minimul;
            curent++;
        } else {
            curent++;
        }
        medie+=minimul;
        generatii++;
    }
    medie=medie/generatii;
//    cout<<progres1<<endl;
//    cout<<"media este: "<<medie<<endl;
    //    cout<<"minimul este : "<<minimul<<endl;
    return minimul;
}