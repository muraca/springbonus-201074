//
//  ex4.h
//  springbonus
//
//  Created by Matteo Muraca
//  Copyright © 2020 Matteo Muraca. All rights reserved.
//

#ifndef ex4_h
#define ex4_h
#include "myTools.h"
#include "omp.h"
#include <iostream>
using namespace std;
unsigned long findInVec(int* vector, unsigned long dim, int n, int nthreads){ //looks for a number in the vector, using multiple threads and cancellation
    int pos = 0; 
    #pragma omp parallel num_threads(nthreads) shared(vector, dim, n, pos)
    {
        #pragma omp for
        for(unsigned long i=0; i<dim; ++i){
                #pragma omp cancellation point for
            if(vector[i]==n){
                #pragma omp atomic write
                pos = i;
                #pragma omp cancel for
            }
        }
    }
    
    if(vector[pos]==n)
        return pos;
    return dim+1;
}

void ex4(){ //called by doSomething, creates a random vector and calls the function that looks for a number in the vector
    if(omp_get_cancellation()!=1){
        cout<<"OMP_CANCELLATION environment variable is not set to 1!"<<endl;
        return;
    }
    
    cout<<"Please insert desired vectors dimension: ";
    unsigned long dim;
    cin>>dim;
    
    int *vector = randomParallelVec(dim, 8);
    cout<<"Random vector generated."<<endl;
    
    int number;
    cout<<"Insert the number to look for: ";
    cin>>number;
    
    double starttime = omp_get_wtime();
    unsigned long s = findInVec(vector, dim, number, 1);
    cout<<endl<<"Serial says ";
    if(pos==dim+1)
        cout<<"not found!"<<endl;
    else
        cout<<pos<<endl<<"vector["<<pos<<"] is "<<vector[pos]<<endl;
    double stoptime = omp_get_wtime();
    cout<<"Time: "<< 1000*(stoptime-starttime) <<" milliseconds"<<endl<<endl;
    for(int i=2; i<10; i*=2){
        starttime = omp_get_wtime();
        unsigned long pos = findInVec(vector, dim, number, i);
        cout<<"Parallel using "<<i<<" threads says ";
        if(pos==dim+1)
            cout<<"not found!"<<endl;
        else
            cout<<pos<<endl<<"vector["<<pos<<"] is "<<vector[pos]<<endl;
        stoptime = omp_get_wtime();
        cout<<"Time: "<< 1000*(stoptime-starttime) <<" milliseconds "<<endl<<endl;
    }
    
    delete[] vector;
    
}

#endif /* ex4_h */
