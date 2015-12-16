#include "Tic.h"
using namespace std;

Board::Board(const Player &p11, const Player &p22):p1(p11), p2(p22) {
    turn  = true;
}



bool Board::move(Location outerLocation, Location innerLocation) {
    Player *playing = getTurn();
	if ((outerLocation.equals(playing->locationToMove) || playing->locationToMove.equals(Location(100, 100)))) {
        bool played = board[outerLocation.getX()][outerLocation.getY()].play(innerLocation.getX(), innerLocation.getY(), playing->getPiece(), playing);
		if(played) {
			Player *otherPlaying = getOtherTurn();
		    otherPlaying->locationToMove = innerLocation;
		    if (board[innerLocation.getX()][innerLocation.getY()].full()){
		    	Location l(100, 100);
		    	otherPlaying->locationToMove = l;
		    }
		    turn  = !turn;
		    return true;
		}
    }
    return false;
}

bool Board::isGameOver() {
    return isWinner(p1) || isWinner(p2) || isAllFull();
}

bool Board::isAllFull() {
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (!board[i][j].full())
                return false;
    return true;
}

bool Board::isWinner(const Player &p) {
    return checkVerticals(p) || checkHorizontals(p) || checkDiagonal1(p) || checkDiagonal2(p);
}

bool Board::checkHorizontals(const Player &p) {
    for (int i = 0; i < 3; i++)
        if (checkHorizontal(i, p))
            return true;
    return false;
}

bool Board::checkHorizontal(int x, const Player &p) {
    for (int i = 0; i < 3; i++)
        if (!board[x][i].getWinner().equals(p)){
            return false;
        }
    return true;
}

bool Board::checkVerticals(const Player &p) {
    for (int i = 0; i < 3; i++)
        if (checkVertical(i, p))
            return true;
    return false;
}

bool Board::checkVertical(int y, const Player &p) {
    for (int i = 0; i < 3; i++)
        if (!board[i][y].getWinner().equals(p))
            return false;
    return true;
}

bool Board::checkDiagonal1(const Player &p) {
    for (int i = 0; i < 3; i++)
        if (!board[i][i].getWinner().equals(p))
            return false;
    return true;
}

bool Board::checkDiagonal2(const Player &p) {
    for (int i = 0, j = 2; i < 3 && j >= 0; i++, j--)
        if (!board[i][j].getWinner().equals(p))
            return false;
    return true;
}


Player Board::getWinner() {
    if (isWinner(p1))
        return p1;
    if (isWinner(p2))
        return p2;
    Player p;
	return p;
}

Player* Board::getTurn() {
    if (turn)
        return &p1;
    else
        return &p2;
}

Player* Board::getOtherTurn() {
    if (turn)
        return &p2;
    else
        return &p1;
}

int** Board::getBoard(){
	int** bigBoard = 0;
	bigBoard = new int*[9];
	for(int i = 0; i < 9; ++i){
		bigBoard[i] = new int[9];
		int x = (int) (i / 3.0);
		for(int j = 0; j < 9; ++j){
			bigBoard[i][j] = 0;
			int y = (int) (j / 3.0);
			char c = board[x][y].grid[i % 3][j % 3].getPiece();
			if(c == 'x'){
				bigBoard[i][j] = 1;
			}
			else if(c == 'o'){
				bigBoard[i][j] = 2;
			}
		}
	}
	return bigBoard;
}

int** Board::winners() {
  int** hamada = 0;
	hamada = new int*[3];
  for(int i = 0; i < 3; ++i){
		hamada[i] = new int[3];
		for(int j = 0; j < 3; ++j){
      if ((board[i][j].getWinner()).equals(p1)) {
        hamada[i][j] = 1;
      }
      else if ((board[i][j].getWinner()).equals(p2))
        hamada[i][j] = 2;
      else
        hamada[i][j] = 0;
			//hamada[i][j] = (board[i][j].getWinner()).equals(p1) ? 1 : ((board[i][j].getWinner()).equals(p2) ? 2 : 0);
		}
	}
	return hamada;
}

void Board::display() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            board[i][j].display();
		cout<<endl;
    }
}
