//
//  ex2.h
//  springbonus
//
//  Created by Matteo Muraca
//  Copyright © 2020 Matteo Muraca. All rights reserved.
//

#ifndef ex2_h
#define ex2_h
#include <iostream>
#include "omp.h"
#include "myTools.h"
using namespace std;

void ex2(){ //called by doSomething, only function for exercise 2
    cout<<"Please insert desired vectors dimension: ";
    unsigned long dim;
    cin>>dim;
    
    int *a = randomParallelVec(dim, 8);
    int *b = randomParallelVec(dim, 8);
    cout<<"Random vectors generated."<<endl;
    
    int nthreads = inputNumThreads();
    if(nthreads==1)
        cout<<"Vectors serial computation."<<endl;
    else
        cout<<"Vectors parallel computation with "<<nthreads<<" threads."<<endl;
    int *c = new int[dim], scal = 0;
    cout<<"Computing vector c."<<endl;
    double starttime = omp_get_wtime();
    #pragma omp parallel for num_threads(nthreads) reduction(+:scal) schedule(static)
    for(unsigned long i=0; i<dim; ++i){
        c[i] = a[i] + b[i];
        scal += c[i] * b[i];
    }
    double stoptime = omp_get_wtime();
    cout<<"Scalar product between vectors c and b is: "<<scal<<endl;
    cout<<"Time: "<< 1000*(stoptime-starttime) <<" milliseconds"<<endl<<endl;
    
    delete[] a;
    delete[] b;
    delete[] c;
}

#endif /* ex2_h */
