//
//  main.h
//  2048Bot
//
//  Created by Nicholas Mercier on 2017-05-03.
//  Copyright © 2017 Nicholas Mercier. All rights reserved.
//

#ifndef main_h
#define main_h


void print(const char* value);
void printArr(double* arr, int size);
void printHex(const unsigned long value);
void benchmark(unsigned long value);

//Out-of-class board manipulations
void printBinaryBoard(const unsigned long board);
unsigned long moveBoardUp(const unsigned long value);
unsigned long up(const unsigned long value, unsigned long& state);
unsigned long moveBoardDown(const unsigned long value);
unsigned long down(const unsigned long value, unsigned long& state);
unsigned long moveBoardLeft(const unsigned long value);
unsigned long left(const unsigned long value, unsigned long& state);
unsigned long moveBoardRight(const unsigned long value);
unsigned long right(const unsigned long value, unsigned long& state);
int numEmptySpaces(const unsigned long board);
unsigned long spawnInEmptySpaceRandom(const unsigned long board, const int spaces);
double frand();

#endif /* main_h */
