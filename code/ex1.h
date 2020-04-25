//
//  ex1.h
//  springbonus
//
//  Created by Matteo Muraca
//  Copyright © 2020 Matteo Muraca. All rights reserved.
//

#ifndef ex1_h
#define ex1_h
#include <iostream>
#include "omp.h"
#include "myTools.h"
using namespace std;

void ex1Static(unsigned long dimension, int nthreads){ //exercise 1, static schedule
    if(nthreads==1)
        cout<<"Serial 2D matrix computation with static schedule."<<endl;
    else
        cout<<"Parallel 2D matrix computation with "<<nthreads<<" threads and static schedule."<<endl;
    long long unsigned **a = new long long unsigned*[dimension];
    long long unsigned **b = new long long unsigned*[dimension];
    double starttime = omp_get_wtime();
    #pragma omp parallel for num_threads(nthreads) schedule(static)
    for(unsigned long i=0; i<dimension; ++i){
        a[i] = new long long unsigned[dimension];
        b[i] = new long long unsigned[dimension];
        for(int j=0; j<dimension; ++j){
            a[i][j] = b[i][j] = (long long unsigned) 15.0 * i * i + 5 * PI24 * j * j * j * j * j * j;
            b[i][j] *= 11 / 5;
        }
    }
    double stoptime = omp_get_wtime();
    cout<<"Time: "<< 1000*(stoptime-starttime) <<" milliseconds"<<endl<<endl;
    deleteMatrix(a, dimension);
    deleteMatrix(b, dimension);
}

void ex1Dynamic(unsigned long dimension, int nthreads){ //exercise 1, dynamic schedule
    if(nthreads==1)
        cout<<"Serial 2D matrix computation with dynamic schedule."<<endl;
    else
        cout<<"Parallel 2D matrix computation with "<<nthreads<<" threads and dynamic schedule."<<endl;
    long long unsigned **a = new long long unsigned*[dimension];
    long long unsigned **b = new long long unsigned*[dimension];
    double starttime = omp_get_wtime();
    #pragma omp parallel for num_threads(nthreads) schedule(dynamic)
    for(unsigned long i=0; i<dimension; ++i){
        a[i] = new long long unsigned[dimension];
        b[i] = new long long unsigned[dimension];
        for(int j=0; j<dimension; ++j){
            a[i][j] = b[i][j] = (long long unsigned) 15.0 * i * i + 5 * PI24 * j * j * j * j * j * j;
            b[i][j] *= 11 / 5;
        }
    }
    double stoptime = omp_get_wtime();
    cout<<"Time: "<< 1000*(stoptime-starttime) <<" milliseconds"<<endl<<endl;
    deleteMatrix(a, dimension);
    deleteMatrix(b, dimension);
}

void ex1(){ //called by doSomething, calls the function the user wants for ex1
    cout<<"Insert dimension: ";
    int dim;
    cin>>dim;
    
    int nt = inputNumThreads();
    
    cout<<"Insert an even number for static schedule, or an odd number for dynamic schedule.";
    int schedule;
    cin>>schedule;
    
    if(schedule%2==0)
        ex1Static(dim, nt);
    else
        ex1Dynamic(dim, nt);
}

#endif /* ex1_h */
