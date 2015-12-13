#include <vector>
#include <string>
#include <iostream>
#include <stdio.h>
#include "Tic.h"
#ifndef LOCATION_H_
#define LOCATION_H_


using namespace std;

class Location {
public:
    int x, y;
    Location(int xx, int yy);
    int getX();
    int getY();
    bool equals(const Location &l);
};

#endif
