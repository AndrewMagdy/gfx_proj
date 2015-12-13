#include "Tic.h"
using namespace std;

Player::Player() : locationToMove(100, 100) {
    name = "Null";
    piece = ' ';
}
Player::Player(string s, char p) : locationToMove(100, 100) {
    name = s;
    piece = p;
}

string Player::getName() {
    return name;
}

char Player::getPiece() {
    return piece;
}

bool Player::equals(const Player &p) {
    return name == p.name && piece == p.piece;
}
