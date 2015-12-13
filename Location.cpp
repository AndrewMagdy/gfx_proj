#include "Tic.h"
using namespace std;

Location::Location(int xx, int yy) {
    x = xx;
    y = yy;
}

int Location::getX() {
    return x;
}

int Location::getY() {
    return y;
}

bool Location::equals(const Location &l) {
    return x == l.x && y == l.y;
}
