#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "Player.h"
#include "Cell.h"
//#include "Tic.h"
using namespace std;
#ifndef SMALLBOARD_H_
#define SMALLBOARD_H_
class SmallBoard {
public:
    Cell grid[3][3];
    Player BoardWinner;
    SmallBoard();
    bool full();
    bool play(int x, int y, char c, Player *p);
    Player getWinner();
    bool checkX(int x, char c);
    bool checkY(int y, char c);
    bool checkDiagonal1(char c);
    bool checkDiagonal2(char c);
    vector<Location> getPossiblePlays();
    void display();
};
#endif
