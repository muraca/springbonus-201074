//
//  ex5.h
//  springbonus
//
//  Created by Matteo Muraca
//  Copyright © 2020 Matteo Muraca. All rights reserved.
//

#ifndef ex5_h
#define ex5_h
#include <iostream>
#include "myTools.h"
#include "omp.h"
using namespace std;

class GameOfLife{
    
private:
    
    bool** m;
    unsigned int numberCells;
    int nthreads;
    unsigned int dimension;
    
    bool** initialization(){ //initialization of a new boolean matrix
        bool** mat = new bool*[dimension];
        #pragma omp parallel for num_threads(nthreads) schedule(static)
        for(unsigned long i=0; i<dimension; ++i){
            mat[i] = new bool[dimension];
            for(int j=0; j<dimension; ++j)
                mat[i][j] = false; //all elements are set to false
        }
        return mat;
    }
    
    void output(){ //counts and prints the number of alive cells in the matrix
        unsigned int cont=0;
        #pragma omp parallel for numthreads(nthreads) schedule(static) reduction(+:cont)
        for(unsigned long i=1; i<dimension-1; ++i){
            for(int j=1; j<dimension-1; ++j)
                if(m[i][j])
                    cont++;
        }
        
        cout<<"There are "<<cont<<" cells alive right now."<<endl;
        numberCells = cont;
    }
    
    void createRandomCells(){ //needed at game beginning, generates random alive cells in the matrix
        cout<<"Generating random cells"<<endl;
        #pragma omp parallel for num_threads(nthreads)
        for(unsigned long i=omp_get_thread_num(); i<dimension-1; i+=nthreads){
            unsigned int seed = (unsigned int) (omp_get_wtime()*1000.0*(double)omp_get_thread_num()/PI24);
            for(int j=1; j<dimension-1; ++j){
                int r = randomNumber<int>(seed);
                if(r%2==0)
                    m[i][j] = true;
            }
        }
            
        
        cout<<"Done: ";
    }
    
    unsigned int countNextTo(unsigned long i, int j){ //counts in a 3*3 square around the given point how many cells are alive
        unsigned int cont=0;
        for(int r = i-1; r<=i+1; ++r){
            for(int c = j-1; c<=j+1; ++c){
                if(m[r][c])
                    cont++;
            }
        }
        if(m[i][j])
            cont--;
        return cont;
    }
    
    void nextStage(){ //initialize a new empty matrix, put all the new informations of alive cells there, then deletes the old one and replaces it with the new one
        bool** mNew = initialization();
        
        #pragma omp parallel for num_threads(nthreads) schedule(static)
        for(unsigned long i=1; i<dimension-1; ++i){
            for(int j=1; j<dimension-1; ++j){
                unsigned int num = countNextTo(i, j);
                if(num==3)
                    mNew[i][j] = true;
                else if(m[i][j]==true&&num==2){
                        mNew[i][j] = true;
                }
            }
        }
        
        deleteMatrix(m, dimension);
        m = mNew;
    }
    
public:
    
    GameOfLife(int n, unsigned int d){ //constructor
        numberCells = 0;
        nthreads = n;
        dimension = d+2;
        m = initialization();
    }
    
    ~GameOfLife(){ //destructor
        deleteMatrix(m, dimension);
    }
    
    void start(){ //play the game
        cout<<"Starting The Game Of Life."<<endl;
        cout<<"Every 10 generations you will get asked if you want to continue."<<endl;
        cout<<"Please don't ctrl+c this game because it may cause problems with your RAM."<<endl;
        cout<<"Are you ready? ";
        if(ready()){
            createRandomCells();
            output();
            int count=0;
            while(numberCells>0){
                count++;
                nextStage();
                output();
                if(count%10==0){
                    cout<<count<<" generations now! Do you want to continue?"<<endl;
                    if(!ready())
                        break;
                    
                }
            }
        }
        cout<<"Game over!"<<endl;
    }
    
    void test(int trials){ //test the game
        createRandomCells();
        output();
        for(int i=0; i<trials; ++i){
            if(numberCells>0){
                double starttime = omp_get_wtime();
                nextStage();
                double stoptime = omp_get_wtime();
                output();
                cout<<"Time: "<< 1000*(stoptime-starttime) <<" milliseconds"<<endl<<endl;
            }
            else
                break;
        }
    }
};

#endif /* ex5_h */
