
#ifndef __BOAT_H
#define __BOAT_H

using namespace std;
#include <iostream>
#include <vector>
#include <ctype.h>

struct Coordinate {
    int Row;
    int Col;
    bool TrueCoordinate;
};

struct PointStatus {
    Coordinate A_Coordinate;
    bool status;
    // true for alive point
    // false for dead point
};

class Boat {

public:
    bool IsBoatDead();
    Boat(const int &size, Coordinate StartingCoordinate, char &ProposeOrientation, vector<vector<char>>& TheBoard);
    //for all boats not size 1

    Boat(int size, Coordinate StartingCoordinate, vector<vector<char>>& TheBoard);
    //for boat of size 1
    char GetOrientation();
    Coordinate CoordinatesValid(const string& CheckCoord);
    ~Boat();



private:
    int sizeOfBoat;

    vector<PointStatus> BoatStatus;
    //to indicate each coordinate of the boat along with whether its been hit or not

    char Orientation;
    //either V or H for vertical & horizontal respectively
    //if V, starting coordinate must head down
    //if H, starting coordinate must head right



};

#endif // __BOAT_H
