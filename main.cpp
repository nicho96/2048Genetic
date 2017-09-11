//
//  main.cpp
//  2048Bot
//
//  Created by Nicholas Mercier on 2017-05-03.
//  Copyright © 2017 Nicholas Mercier. All rights reserved.
//

#include <iostream>
#include "main.h"
#include "Board.hpp"
#include <chrono>
#include <ctime>
#include "Simulator.hpp"
#include <random>
#include <iomanip>

using namespace std;
using namespace chrono;

int main(int argc, const char * argv[]) {

    long start = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    
    //Seed random (remove for constant seed)
    srand(static_cast<unsigned int>(time(NULL)));
    
    
    //simulateRandomHeuristics();
    
    Board b = Board(0);
    b.play();
    printBinaryBoard(b.board);
    
    long end = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    double duration = end - start;
    cout << duration << "ms\n";
    return 0;
}

void benchmark(unsigned long board){
    int amount = 10000000;
    
    long start = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    for(int i = 0; i < amount; ++i){
        board = moveBoardUp(board);
        board = moveBoardDown(board);
        board = moveBoardLeft(board);
        board = moveBoardRight(board);
    }
    long end = duration_cast< milliseconds >(system_clock::now().time_since_epoch()).count();
    
    double duration = end - start;
    cout << duration << "ms - " << (4 * amount / duration) << "moves/ms\n";
}

double frand(){
    random_device rd;
    mt19937 e2(rd());
    uniform_real_distribution<> dist(0, 1);
    return dist(e2);
}

void printArr(double* arr, int size){
    cout << "{";
    for (int i = 0; i < size; ++i)
        cout << arr[i] << ((i == size - 1) ? "" : ", ");
    cout << "}\n";
}

void print(const char* value){
    cout << value << "\n";
}

void printHex(const unsigned long value){
    cout << hex << value << "\n";
}

void printBinaryBoard(const unsigned long board){
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


unsigned long moveBoardUp(const unsigned long value){
    unsigned long state = 0;
    unsigned long board = up(value, state);
    while(true){
        unsigned long tmp = up(board, state);
        if(tmp == board) break;
        else board = tmp;
    }
    return board;
}

unsigned long moveBoardDown(const unsigned long value){
    unsigned long state = 0;
    unsigned long board = down(value, state);
    while(true){
        unsigned long tmp = down(board, state);
        if(tmp == board) break;
        else board = tmp;
    }
    return board;
}

unsigned long moveBoardLeft(const unsigned long value){
    unsigned long state = 0;
    unsigned long board = left(value, state);
    while(true){
        unsigned long tmp = left(board, state);
        if(tmp == board) break;
        else board = tmp;
    }
    return board;
}

unsigned long moveBoardRight(const unsigned long value){
    unsigned long state = 0;
    unsigned long board = right(value, state);
    while(true){
        unsigned long tmp = right(board, state);
        if(tmp == board) break;
        else board = tmp;
    }
    return board;
}

unsigned long up(const unsigned long value, unsigned long& state){
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

unsigned long down(const unsigned long value, unsigned long& state){
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

unsigned long left(const unsigned long value, unsigned long& state){
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

unsigned long right(const unsigned long value, unsigned long& state){
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

int numEmptySpaces(const unsigned long board){
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

unsigned long spawnInEmptySpaceRandom(const unsigned long board, const int spaces){
    unsigned long mask = 0x000000000000000F;
    int r = rand() % spaces;
    int count = 0;
    for(int i = 0; i < 16; ++i){
        if((board & mask) == 0){
            if(count == r){
                unsigned long value = (rand() % 10 == 0) ? 2 : 1; //90% chance of 2, 10% chance of 4
                value = value << (i * 4);
                return board | value;
            }
            ++count;
        }
        mask = mask << 4;
    }
    
    return board;
    
}
