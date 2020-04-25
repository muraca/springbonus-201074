//
//  ex3.h
//  springbonus
//
//  Created by Matteo Muraca
//  Copyright © 2020 Matteo Muraca. All rights reserved.
//

#ifndef ex3_h
#define ex3_h
#include <iostream>
#include <stdlib.h>
#include "omp.h"
#include "myTools.h"

void output(double pi, double time){ //output for all the functions: pi, error and time
    double error = pi - PI24;
    cout<<"The computed value of Pi is "<<pi<<endl<<"The error is "<<error<<endl;
    cout<<"Time: "<< 1000*time <<" milliseconds"<<endl<<endl;
}

void piCiclic(unsigned num_steps, unsigned numt){ //ciclic algorithm
    double pi=0.0, deltaX=1.0/(double)num_steps, starttime = omp_get_wtime();
    
    double *sum = new double[numt];
    #pragma omp parallel num_threads(numt)
    {
        sum[omp_get_thread_num()]=0.0;
        #pragma omp for
        for (unsigned i=0;i<num_steps;++i){
            double area=(i+0.5)*deltaX;
            sum[omp_get_thread_num()]+= 4.0/(1.0+area*area);
        }
    }
    for(unsigned i=0;i<numt;++i){
        pi+=sum[i];
    }
    pi*=deltaX;
    
    double stoptime = omp_get_wtime();
    output(pi, (stoptime-starttime));
}

void piPad(unsigned num_steps, unsigned numt, unsigned pad){ //ciclic algorithm with padding
    double pi=0.0, deltaX=1.0/(double)num_steps, starttime = omp_get_wtime();
    
    double **sum = new double*[numt];
    #pragma omp parallel num_threads(numt)
    {
        sum[omp_get_thread_num()] = new double[pad];
        sum[omp_get_thread_num()][0] = 0.0;
        #pragma omp for
        for (unsigned i=0;i<num_steps;++i){
            double area=(i+0.5)*deltaX;
            sum[omp_get_thread_num()][0]+= 4.0/(1.0+area*area);
        }
    }
    for(unsigned i=0;i<numt;++i){
        pi+=sum[i][0];
    }
    pi*=deltaX;
    
    double stoptime = omp_get_wtime();
    output(pi, (stoptime-starttime));
}

void piCritical(unsigned num_steps, unsigned numt){ //ciclic algorithm with critical section
    double deltaX=1.0/(double)num_steps, pi=0.0, sum=0.0, starttime = omp_get_wtime();
    
    #pragma omp parallel for num_threads(numt) shared(sum, num_steps)
    for (unsigned i=0;i<num_steps;++i){
        double area=(i+0.5)*deltaX;
        double tosum = 4.0/(1.0+area*area);
        #pragma omp critical
        sum += tosum;
    }
    pi = sum * deltaX;
    
    double stoptime = omp_get_wtime();
    output(pi, (stoptime-starttime));
}

void piAtomic(unsigned num_steps, unsigned numt){ //ciclic algorithm with atomic operation
    double deltaX=1.0/(double)num_steps, pi=0.0, sum=0.0, starttime = omp_get_wtime();
    
    #pragma omp parallel for num_threads(numt) shared(sum, num_steps)
    for (unsigned i=0;i<num_steps;++i){
        double area=(i+0.5)*deltaX;
        double tosum = 4.0/(1.0+area*area);
        #pragma omp atomic
        sum += tosum;
    }
    pi = sum * deltaX;
    
    double stoptime = omp_get_wtime();
    output(pi, (stoptime-starttime));
}

void piReduction(unsigned num_steps, unsigned numt){ //ciclic algorithm with reduction
    double deltaX=1.0/(double)num_steps, pi=0.0, sum=0.0, area=0.0, starttime = omp_get_wtime();
    
    #pragma omp parallel for private(area) reduction(+:sum) num_threads(numt)
    for (unsigned i=0;i<num_steps;++i){
        area=((double)i+0.5)*deltaX;
        sum += 4.0/(1.0+area*area);
    }
    pi = sum * deltaX;
    
    double stoptime = omp_get_wtime();
    output(pi, (stoptime-starttime));
}

void piMonteCarlo(long num_trials, unsigned numt){ //monte carlo algorithm
    cout<<"Computing pi with Monte Carlo algorithm: "<<endl;
    
    long ncirc = 0;
    double pi = 0.0, x = 0.0, y = 0.0;
    
    double starttime = omp_get_wtime();
    #pragma omp parallel private(x,y) reduction(+:ncirc) num_threads(numt)
    {
        unsigned int seed = (unsigned int) (omp_get_wtime()*10000.0*(double)omp_get_thread_num()/PI24);
        for(long i=omp_get_thread_num(); i<num_trials; i+=omp_get_num_threads()){
            x = (double) rand_r(&seed)/RAND_MAX;
            y = (double) rand_r(&seed)/RAND_MAX;
            if((x*x + y*y) <= 1)
                ncirc++;
        }
    }
    
    pi = 4.0 * (double)ncirc/(double)num_trials;
    
    double stoptime = omp_get_wtime();
    output(pi, (stoptime-starttime));
    
}

void ex3(){ //called by doSomething, calls the function the user wants for pi computation
    cout<<"Which method do you want to use?"<<endl;
    cout<<"1 - Cyclic Algorithm"<<endl<<"2 - Cyclic Algorithm with padding"<<endl<<"3 - Cyclic Algorithm with Critical clause"<<endl<<"4 - Cyclic Algorithm with Atomic clause"<<endl<<"5 - Cyclic Algorithm with Reduction for final sum"<<endl<<"6 - Monte Carlo Algorithm"<<endl;
    cout<<"Insert corresponding number: ";
    int number;
    cin>>number;
    if(number<1||number>6){
        return;
    }
    else if(number==6){
        cout<<"Insert number of trials: ";
        long trials;
        cin>>trials;
        
        int nt = inputNumThreads();
        
        piMonteCarlo(trials, nt);
    }
    else{
        cout<<"Insert number of steps for integral approximation: ";
        long steps;
        cin>>steps;
        
        int nt = inputNumThreads();
        
        cout<<"Computing pi. "<<endl;
        
        switch(number){
            case 1:
                piCiclic(steps, nt);
                break;
            case 2:
                unsigned pad;
                cout<<"Insert number of pads: ";
                cin>>pad;
                piPad(steps, nt, pad);
                break;
            case 3:
                piCritical(steps, nt);
                break;
            case 4:
                piAtomic(steps, nt);
                break;
            case 5:
                piReduction(steps, nt);
                break;
            default:
                break;
        }
    }
}

#endif /* ex3_h */
