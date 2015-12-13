#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "Tic.h"
using namespace std;
#ifndef CELL_H_
#define CELL_H_
class Cell {
public:
    char piece;
    bool taken;
    Cell();
    bool putPiece(char c);
    char getPiece();
    bool isTaken();
};
#endif
