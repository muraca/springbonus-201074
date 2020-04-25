//
//  myTools.h
//  springbonus
//
//  Created by Matteo Muraca
//  Copyright © 2020 Matteo Muraca. All rights reserved.
//

#ifndef myTools_h
#define myTools_h
#include <iostream>
#include "omp.h"
using namespace std;

const double PI24 = 3.141592653589793238462643; //pi with the first 24 decimal digits

template <class T>
void vectorOut(T* vector, unsigned long dim){ //output of a vector
    cout<<"[";
    for(unsigned long i=0; i<dim; ++i){
        cout<<" "<<vector[i];
    }
    cout<<" ]";
}

template <class T>
void deleteMatrix(T **a, unsigned long dim){ //deletes a dynamic matrix
    for(unsigned long i=0; i<dim; ++i){
        delete[] a[i];
    }
    delete[] a;
}

template <class T>
void deleteMatrixParallel(T **a, unsigned long dim){ //deletes a dynamic matrix using parallelism
    #pragma omp parallel for schedule (static)
    for(unsigned long i=0; i<dim; ++i){
        delete[] a[i];
    }
    delete[] a;
}

int *randomVec(unsigned long dim){ //random vector of integers
    int *newVec = new int[dim];
    unsigned int seed = (unsigned int) (omp_get_wtime()*10000.0/PI24);
    for(unsigned long i=0; i<dim; ++i){
        newVec[i] = (int) rand_r(&seed);
    }
    return newVec;
}

int *randomParallelVec(unsigned long dim, int nthreads=1){ //random vector of integers using parallelism
    int *newVec = new int[dim];
    #pragma omp parallel num_threads(nthreads)
    {
        unsigned int seed = (unsigned int) (omp_get_wtime()*10000.0*(double)omp_get_thread_num()/PI24);
        for(unsigned long i=omp_get_thread_num(); i<dim; i+=nthreads){
            newVec[i] = rand_r(&seed) % 100;
        }
    }
    return newVec;
}

template <class T>
T randomNumber(){ //random number without a given seed
    unsigned int seed = (unsigned int) (omp_get_wtime()*PI24/9);
    return (T) rand_r(&seed);
}

template <class T>
T randomNumber(unsigned int seed){ //random number with a given seed
    return (T) rand_r(&seed);
}

int inputNumThreads(){ //input for number of threads
    int nt=0;
    while(nt<1||nt>8){
        cout<<"Insert number of threads: ";
        cin>>nt;
    }
    return nt;
}

bool ready(){ //input for answer to current questions
    cout<<"Insert y for yes: ";
    char c; cin>>c;
    if(c=='y')
        return true;
    return false;
}

#endif /* myTools_h */
