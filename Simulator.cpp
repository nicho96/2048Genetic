//
//  Simulator.cpp
//  2048Bot
//
//  Created by Nicholas Mercier on 2017-05-04.
//  Copyright © 2017 Nicholas Mercier. All rights reserved.
//

#include <iostream>
#include "Simulator.hpp"

using namespace std;

void simulateRandomHeuristics(){

    int it = 0;
    while(it < 1000){
        
        Board b = Board(0);
        for(int i = 0; i < 3; i++){
            b.heuristics[i] = frand();
        }
        b.play();
        b.pollHeuristicData();
        cout << "Max: " << b.maxTile << "\n";
        printArr(b.heuristics, 3);
        
        it++;
    }
    
}


