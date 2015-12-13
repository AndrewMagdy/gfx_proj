#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "SmallBoard.h"
using namespace std;
#ifndef BOARD_H_
#define BOARD_H_

class Board {
public:
    SmallBoard board[3][3];
    Player p1, p2;
    bool turn;
    Board(const Player &p11, const Player &p22);
    bool move(Location outerLocation, Location innerLocation);
    bool isGameOver();
    bool isAllFull();
    bool isWinner(const Player &p);
    bool checkHorizontals(const Player &p);
    bool checkHorizontal(int x, const Player &p);
    bool checkVerticals(const Player &p);
    bool checkVertical(int y, const Player &p);
    bool checkDiagonal1(const Player &p);
    bool checkDiagonal2(const Player &p);
    Player getWinner();
    Player* getTurn();
    Player* getOtherTurn();
    int** getBoard();
    void display();
};
#endif
