//
//  Board.hpp
//  2048Bot
//
//  Created by Nicholas Mercier on 2017-05-03.
//  Copyright © 2017 Nicholas Mercier. All rights reserved.
//

#ifndef Board_hpp
#define Board_hpp

#include <stdio.h>
#include "main.h"


enum Direction {UP, DOWN, LEFT, RIGHT, NONE};

class Board{

    public:
    double heuristics[3] = {0.400772, 0.722464, 0.96385};
    unsigned long board;
    Board(unsigned long board);
    unsigned long moveBoardUp(const unsigned long value);
    unsigned long moveBoardDown(const unsigned long value);
    unsigned long moveBoardLeft(const unsigned long value);
    unsigned long moveBoardRight(const unsigned long value);
    int numEmptySpaces();
    Direction getOptimalMove(const int depth);
    void lookAhead(int depth);
    void spawnInEmptySpaceRandom();
    void makeMove(Direction direction);
    void printBinaryBoard();
    double getBoardScore();
    void pollHeuristicData();
    void play();
    
    int emptyTileCount = 0;
    long maxTile = 1;
    Direction lastMove;
    int scoreSum = 0;
    
    private:
    unsigned long right(const unsigned long value, unsigned long& state);
    unsigned long up(const unsigned long value, unsigned long& state);
    unsigned long down(const unsigned long value, unsigned long& state);
    unsigned long left(const unsigned long value, unsigned long& state);
    

};

#endif /* Board_hpp */
