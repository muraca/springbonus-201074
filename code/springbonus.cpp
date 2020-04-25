//
//  springbonus.cpp
//  springbonus
//
//  Created by Matteo Muraca
//  Copyright © 2020 Matteo Muraca. All rights reserved.
//

#include <iostream>
#include <climits>
#include "ex1.h"
#include "ex2.h"
#include "ex3.h"
#include "ex4.h"
#include "ex5.h"
using namespace std;

void explain(){ //explains how to choose an exercise via input
    cout<<"Every exercise corresponds to the equivalent number:"<<endl;
    cout<<"1 - 2D Matrix Computation"<<endl<<"2 - Vectors Computation"<<endl<<"3 - Pi Computation"<<endl<<"4 - Find a number in a vector"<<endl<<"5 - Conway's Game of Life"<<endl<<"0 - Exit"<<endl;
    cout<<"Insert a number to test my code: ";
}

void testEx5(){ //calls test mode for exercise 5
    for(int i=1; i<=8; i*=2){
        for(int j=1000; j<=100000; j*=10){
            cout<<"Testing with "<<i<<" threads a matrix of "<<j<<" booleans."<<endl;
            GameOfLife *game = new GameOfLife(i, j);
            game->test(2);
            delete game;
        }
    }
    
}

void ex5(){ //calls exercise 5
    cout<<"Do you want to enable test mode? If you just want to play, please don't enable it. ";
    bool test = ready();
    if(test){
        testEx5();
        return;
    }
    int nt = inputNumThreads();
    unsigned int dim=1;
    while(dim<2){
        cout<<"Matrix dimension should be at least 2. Insert dimension: ";
        cin>>dim;
    }
    GameOfLife *game = new GameOfLife(nt, dim);
    game->start();
    
    delete game;
}

void doSomething(int c){ //this function calls the correct exercise for every number
    switch(c){
        case INT_MIN: //I wanted to not display the error at the first instance of this function, so I initialized c as INT_MIN because it's very unlikely for the user to write this number in input.
            break;
        case 1:
            ex1();
            break;
        case 2:
            ex2();
            break;
        case 3:
            ex3();
            break;
        case 4:
            ex4();
            break;
        case 5:
            ex5();
            break;
        default:
            cout<<"Please insert a valid number!";
            break;
    }
}

int main(){
    cout<<"Spring Bonus project by Matteo Muraca, mat. 201074"<<endl;
    
    int c=INT_MIN;
    while(c!=0){ // 0 - Exit
        doSomething(c);
        
        explain();
        cin>>c;
    }
    
    cout<<"Goodbye!"<<endl;
    return 0;
}
