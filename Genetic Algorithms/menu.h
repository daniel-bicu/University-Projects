#include "Euristica.h"
#include "Genetic.h"
#include <iostream>
#include <string>

using namespace std;

double total[100];


void readMyInput(){
   
    int dim;
    fin >> dim;

    for(int  i =  0 ; i < dim; ++i)
        for(int j = 0 ; j < dim; ++j)       
                fin >> cost[i][j];


}


void menu(){

    double MAX = 0;
    double min = 0;
    readMyInput();
    double mdd=FLT_MAX;
  min=euristic();
  // min=genetic();
  // cout<<min<<endl<<endl;
    for (int i = 0; i < 10000; ++i) {
       //min=genetic();
      //  cout<<endl;
        min=euristic();
        if(min<mdd){
            mdd=min;
        }

        if(min > MAX)
        MAX = min;
        cout<<min<<endl;
     //   min=0;
    }
    cout << MAX << endl;

    cout<<mdd<<endl;
}