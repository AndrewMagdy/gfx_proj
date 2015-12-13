#include "Tic.h"
using namespace std;

SmallBoard::SmallBoard():BoardWinner(){
}

bool SmallBoard::full() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (!grid[i][j].isTaken())
                return false;
    return true;
}

bool SmallBoard::play(int x, int y, char c, Player *p) {
    c = tolower(c);
    if (grid[x][y].putPiece(c)) {
        if (BoardWinner.getName() == "Null"){
            if (checkX(x, c) || checkY(y, c) || checkDiagonal1(c) || checkDiagonal2(c)){
				BoardWinner = *p;
			}
		}
        return true;
    }
    return false;
}

bool SmallBoard::checkX(int x, char c) {
    for (int i = 0; i < 3; i++)
        if (grid[x][i].getPiece() != c)
            return false;
    return true;
}

bool SmallBoard::checkY(int y, char c) {
    for (int i = 0; i < 3; i++)
        if (grid[i][y].getPiece() != c)
            return false;
    return true;
}

bool SmallBoard::checkDiagonal1(char c) {
    for (int i = 0; i < 3; i++)
        if (grid[i][i].getPiece() != c)
            return false;
    return true;
}

bool SmallBoard::checkDiagonal2(char c) {
    for (int i = 0, j = 2; i < 3 && j >= 0; i++, j--)
        if (grid[i][j].getPiece() != c)
            return false;
    return true;
}

Player SmallBoard::getWinner() {
    return BoardWinner;
}

vector<Location> SmallBoard::getPossiblePlays() {
    vector<Location> v;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i][j].getPiece() == ' ')
                v.push_back(Location(i, j));
    return v;
}

void SmallBoard::display() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            printf("%c ", grid[i][j].getPiece());
        printf("\n");
    }
}
