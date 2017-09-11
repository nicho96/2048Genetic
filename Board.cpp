//
//  Board.cpp
//  2048Bot
//
//  Created by Nicholas Mercier on 2017-05-03.
//  Copyright © 2017 Nicholas Mercier. All rights reserved.
//

#include <iostream>
#include "Board.hpp"

using namespace std;


Board::Board(unsigned long board){
    this->board = board;
    this->lastMove = NONE;
}

void Board::play(){
        
    //Spawn 2 starting values
    spawnInEmptySpaceRandom();
    spawnInEmptySpaceRandom();
    
    while(true){
        Direction move = getOptimalMove(1);
        if(move != NONE){
            makeMove(move);
            spawnInEmptySpaceRandom();
        }else{
            break;
        }
    }
    cout << "Game Ended \n";
    
}

unsigned long Board::moveBoardUp(const unsigned long value){
    lastMove = UP;
    unsigned long state = 0;
    unsigned long board = up(value, state);
    while(true){
        unsigned long tmp = up(board, state);
        if(tmp == board) break;
        else board = tmp;
    }
    lastMove = (board == value) ? NONE : UP;
    return board;
}

unsigned long Board::moveBoardDown(const unsigned long value){
    lastMove = DOWN;
    unsigned long state = 0;
    unsigned long board = down(value, state);
    while(true){
        unsigned long tmp = down(board, state);
        if(tmp == board) break;
        else board = tmp;
    }
    lastMove = (board == value) ? NONE : DOWN;
    return board;
}

unsigned long Board::moveBoardLeft(const unsigned long value){
    lastMove = LEFT;
    unsigned long state = 0;
    unsigned long board = left(value, state);
    while(true){
        unsigned long tmp = left(board, state);
        if(tmp == board) break;
        else board = tmp;
    }
    lastMove = (board == value) ? NONE : LEFT;
    return board;
}

unsigned long Board::moveBoardRight(const unsigned long value){
    lastMove = RIGHT;
    unsigned long state = 0;
    unsigned long board = right(value, state);
    while(true){
        unsigned long tmp = right(board, state);
        if(tmp == board) break;
        else board = tmp;
    }
    lastMove = (board == value) ? NONE : RIGHT;
    return board;
}

unsigned long Board::up(const unsigned long value, unsigned long& state){
    int shiftAmount = 16;
    unsigned long board = value;
    for(int i = 0; i < 4; ++i){
        int shift = 4 * i; //Amount of bits needed to shift for this iteration
        unsigned long mask = 0xF000000000000000L >> shift; //Mask which can extract the value
        unsigned long current = (board & mask) >> (60 - shift); //Get value at the mask position
        
        int nextShift = shift + shiftAmount; //Shift by 4 more indices
        unsigned long nextMask = mask >> shiftAmount; //Shift mask to next row
        
        while(nextMask != 0){
            
            unsigned long next = (board & nextMask) >> (60 - nextShift);
            
            if(current == 0 && next != 0){ //Swap the value to the front
                
                board = board & ~mask; //Invert mask and make room for next value
                unsigned long tmp = (next) << (60 - shift); //Pad the value that is to be swapped
                board = board | tmp; //Copy next value to current position
                board = board & ~nextMask; //Clear out next tile
                current = next; //Set next to be current (since current is 0)
                
            }else if(next == current && current != 0 && (state & mask) == 0 && (state & nextMask) == 0){ //Merge the tiles
                
                board = board & ~mask; //Invert mask and make room for next value
                unsigned long tmp = (current + 1) << (60 - shift); //Increment value, then pad it to the current index
                board = board | tmp; //Insert the value
                board = board & ~nextMask; //Clear out next tile
                
                state = state | mask; //Set current state flag (indicates this tile has merged)
                
                //Go to the next position
                shift += shiftAmount;
                nextShift = shift + shiftAmount;
                mask = 0xF000000000000000L >> shift;
                nextMask = mask >> shiftAmount;
                current = (board & mask) >> (60 - shift);
                
            }else{
                
                current = next;
                shift = nextShift;
                nextShift += shiftAmount;
                mask = nextMask;
                nextMask = mask >> shiftAmount;
                
            }
        }
    }
    return board;
}

unsigned long Board::down(const unsigned long value, unsigned long& state){
    int shiftAmount = 16;
    unsigned long board = value;
    for(int i = 0; i < 4; ++i){
        int shift = 4 * i; //Amount of bits needed to shift for this iteration
        unsigned long mask = 0x000000000000000FL << shift; //Mask which can extract the value
        unsigned long current = (board & mask) >> shift; //Get value at the mask position
        
        int nextShift = shift + shiftAmount; //Shift by 4 more indices
        unsigned long nextMask = mask << shiftAmount; //Shift mask to next row
        
        while(nextMask != 0){
            unsigned long next = (board & nextMask) >> nextShift;
            
            if(current == 0 && next != 0){ //Swap the value to the front
                
                board = board & ~mask; //Invert mask and make room for next value
                unsigned long tmp = next << shift; //Pad the value that is to be swapped
                board = board | tmp; //Copy next value to current position
                board = board & ~nextMask; //Clear out next tile
                current = next; //Set next to be current (since current is 0)
                
            }else if(next == current && current != 0 && (state & mask) == 0 && (state & nextMask) == 0){ //Merge the tiles
                
                board = board & ~mask; //Invert mask and make room for next value
                unsigned long tmp = (current + 1) << shift; //Increment value, then pad it to the current index
                board = board | tmp; //Insert the value
                board = board & ~nextMask; //Clear out next tile
                
                state = state | mask; //Set current state flag (indicates this tile has merged)
                
                //Go to the next position
                shift += shiftAmount;
                nextShift = shift + shiftAmount;
                mask = 0x000000000000000FL << shift;
                nextMask = mask << shiftAmount;
                current = (board & mask) >> shift;
                
            }else{
                
                current = next;
                shift = nextShift;
                nextShift += shiftAmount;
                mask = nextMask;
                nextMask = mask << shiftAmount;
                
            }
            
        }
    }
    return board;
}

unsigned long Board::left(const unsigned long value, unsigned long& state){
    int shiftAmount = 4;
    unsigned long board = value;
    for(int i = 0; i < 4; ++i){
        int shift = 16 * i; //Amount of bits needed to shift for this iteration
        unsigned long mask = 0xF000000000000000L >> shift; //Mask which can extract the value
        unsigned long current = (board & mask) >> (60 - shift); //Get value at the mask position
        
        int nextShift = shift + shiftAmount; //Shift by 4 more indices
        unsigned long nextMask = mask >> shiftAmount; //Shift mask to next row
        
        int pos = 0;
        
        while(pos < 3){
            
            unsigned long next = (board & nextMask) >> (60 - nextShift);
            
            if(current == 0 && next != 0){ //Swap the value to the front
                
                board = board & ~mask; //Invert mask and make room for next value
                unsigned long tmp = (next) << (60 - shift); //Pad the value that is to be swapped
                board = board | tmp; //Copy next value to current position
                board = board & ~nextMask; //Clear out next tile
                current = next; //Set next to be current (since current is 0)
                
            }else if(next == current && current != 0 && (state & mask) == 0 && (state & nextMask) == 0){ //Merge the tiles
                
                board = board & ~mask; //Invert mask and make room for next value
                unsigned long tmp = (current + 1) << (60 - shift); //Increment value, then pad it to the current index
                board = board | tmp; //Insert the value
                board = board & ~nextMask; //Clear out next tile
                
                state = state | mask; //Set current state flag (indicates this tile has merged)
                
                //Go to the next position
                shift += shiftAmount;
                nextShift = shift + shiftAmount;
                mask = 0xF000000000000000L >> shift;
                nextMask = mask >> shiftAmount;
                current = (board & mask) >> (60 - shift);
                ++pos;
                
            }else{
                
                current = next;
                shift = nextShift;
                nextShift += shiftAmount;
                mask = nextMask;
                nextMask = mask >> shiftAmount;
                ++pos;
                
            }
        }
    }
    return board;
}

unsigned long Board::right(const unsigned long value, unsigned long& state){
    int shiftAmount = 4;
    unsigned long board = value;
    for(int i = 0; i < 4; ++i){
        int shift = 16 * i; //Amount of bits needed to shift for this iteration
        unsigned long mask = 0x000000000000000FL << shift; //Mask which can extract the value
        unsigned long current = (board & mask) >> shift; //Get value at the mask position
        
        int nextShift = shift + shiftAmount; //Shift by 4 more indices
        unsigned long nextMask = mask << shiftAmount; //Shift mask to next row
        
        int pos = 0;
        while(pos < 3){
            unsigned long next = (board & nextMask) >> nextShift;
            
            if(current == 0 && next != 0){ //Swap the value to the front
                
                board = board & ~mask; //Invert mask and make room for next value
                unsigned long tmp = next << shift; //Pad the value that is to be swapped
                board = board | tmp; //Copy next value to current position
                board = board & ~nextMask; //Clear out next tile
                current = next; //Set next to be current (since current is 0)
                
            }else if(next == current && current != 0 && (state & mask) == 0 && (state & nextMask) == 0){ //Merge the tiles
                
                board = board & ~mask; //Invert mask and make room for next value
                unsigned long tmp = (current + 1) << shift; //Increment value, then pad it to the current index
                board = board | tmp; //Insert the value
                board = board & ~nextMask; //Clear out next tile
                
                state = state | mask; //Set current state flag (indicates this tile has merged)
                
                //Go to the next position
                shift += shiftAmount;
                nextShift = shift + shiftAmount;
                mask = 0x000000000000000FL << shift;
                nextMask = mask << shiftAmount;
                current = (board & mask) >> shift;
                ++pos;
                
            }else{
                
                current = next;
                shift = nextShift;
                nextShift += shiftAmount;
                mask = nextMask;
                nextMask = mask << shiftAmount;
                ++pos;
                
            }
            
        }
    }
    return board;
}

int Board::numEmptySpaces(){
    unsigned long mask = 0xF000000000000000;
    int count = 0;
    for(int i = 0; i < 16; ++i){
        
        if((board & mask) == 0){
            ++count;
        }
        mask = mask >> 4;
    }
    return count;
}

void Board::pollHeuristicData(){
    
    emptyTileCount = 0;
    
    unsigned long mask = 0xF000000000000000;
    
    for(int i = 0; i < 16; ++i){
        
        //Set max value
        long value = (board & mask) >> (60 - i * 4);
        if(value > maxTile){
            maxTile = value;
        }
        
        //Increment empty tile count
        if((board & mask) == 0){
            ++emptyTileCount;
        }
        
        mask = mask >> 4;
        
    }
    
}

void Board::spawnInEmptySpaceRandom(){
    int spaces = numEmptySpaces();
    unsigned long mask = 0x000000000000000F;
    int r = rand() % spaces;
    int count = 0;
    for(int i = 0; i < 16; ++i){
        if((board & mask) == 0){
            if(count == r){
                unsigned long value = (rand() % 10 == 0) ? 2 : 1; //90% chance of 2, 10% chance of 4
                value = value << (i * 4);
                board = board | value;
            }
            ++count;
        }
        mask = mask << 4;
    }
}

double Board::getBoardScore(){
    
    pollHeuristicData();
    
    double spaces = emptyTileCount * heuristics[0];
    double max = maxTile * heuristics[2];
    
    return spaces + max;
}

void Board::lookAhead(const int depth){
    
    Board boards[4] = {Board(board), Board(board), Board(board), Board(board)};
    if(depth == 0){
        boards[0].makeMove(UP);
        boards[0].scoreSum = boards[0].getBoardScore();
        boards[1].makeMove(LEFT);
        boards[1].scoreSum = boards[1].getBoardScore();
        boards[2].makeMove(DOWN);
        boards[2].scoreSum = boards[2].getBoardScore();
        boards[3].makeMove(RIGHT);
        boards[3].scoreSum = boards[3].getBoardScore();
    }else{
        boards[0].makeMove(UP);
        boards[0].lookAhead(depth - 1);
        boards[1].makeMove(LEFT);
        boards[1].lookAhead(depth - 1);
        boards[2].makeMove(DOWN);
        boards[2].lookAhead(depth - 1);
        boards[3].makeMove(RIGHT);
        boards[3].lookAhead(depth - 1);
    }
    
    for(Board b : boards){
        scoreSum += b.scoreSum;
    }
    
}

Direction Board::getOptimalMove(const int depth){
    
    Board boards[4] = {Board(board), Board(board), Board(board), Board(board)};
    boards[0].makeMove(UP);
    boards[0].lookAhead(depth);
    boards[1].makeMove(LEFT);
    boards[1].lookAhead(depth);
    boards[2].makeMove(DOWN);
    boards[2].lookAhead(depth);
    boards[3].makeMove(RIGHT);
    boards[3].lookAhead(depth);
    
    Board max = boards[0];
    for(int i = 0 ; i < 4; ++i){
        Board b = boards[i];
        if(max.scoreSum <= b.scoreSum && b.lastMove != NONE){
            max = boards[i];
        }
    }
    
    return max.lastMove;
}

void Board::makeMove(Direction direction){
    switch(direction){
        case UP: board = moveBoardUp(board);
            break;
        case DOWN: board = moveBoardDown(board);
            break;
        case LEFT: board = moveBoardLeft(board);
            break;
        case RIGHT: board = moveBoardRight(board);
            break;
        case NONE: ;
    }
}

void Board::printBinaryBoard(){
    unsigned long mask = 0xF000000000000000;
    int shift = 60;
    for(int i = 0; i < 4; ++i){
        for(int o = 0; o < 4; ++o){
            cout << (((board & mask) >> shift)) << " ";
            mask = mask >> 4;
            shift -= 4;
        }
        cout << "\n";
    }
    cout << "========\n";
}


