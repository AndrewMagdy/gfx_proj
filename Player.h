#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>
#include "Location.h"
#include "Tic.h"
using namespace std;
#ifndef PLAYER_H_
#define PLAYER_H_
class Player {
public:
    string name;
    char piece;
    Location locationToMove;
    Player();
    Player(string s, char p);
    string getName();
    char getPiece();
    bool equals(const Player &p);
};
#endif
