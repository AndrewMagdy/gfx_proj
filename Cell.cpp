#include "Tic.h"
using namespace std;

Cell::Cell() {
    piece = ' ';
    taken = false;
}

bool Cell::putPiece(char c) {
    c = tolower(c);
    if ((c == 'x' || c == 'o') && !isTaken()) {
        piece = c;
        taken = true;
        return true;
    }
    return false;
}

char Cell::getPiece() {
    return piece;
}

bool Cell::isTaken() {
    return taken;
}
