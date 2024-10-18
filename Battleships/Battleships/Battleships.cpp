using namespace std;
#include <iostream>
#include <vector>
#include <ctype.h>
#include "Boat.cpp"
#include <cstdlib> 
#include <stdlib.h>
#include <map>
#include <ctime>
#define BOARD_DIM  9



void SetUpBoard(vector<vector<char>> &Board, int &ForWho, vector<Coordinate> &OnlyForEnemy);
Coordinate CoordinatesValid(const string& CheckCoord);
void PrintBoard(const vector<vector<char>>& BoardToPrint, const int& PrintFor);
bool CheckIfEnoughSpace(Coordinate StartingPlace, char orientation, vector<vector<char>> &Board, const int &size);
bool CheckIfNoMoreBoats(const vector<vector<char>>& Board);
char GenerateOrien();
void Waiting();



struct CoordinateAndDirection {
    Coordinate Coor;
    map<char, bool> DirAndValid;
};

//ToShootAt goes from 0 to 3
// 0 = right
// 1 = left
// 2 = up
// 3 = down.

vector<CoordinateAndDirection> ComputerPlaceForAttack;


int main()
{
    // 9x9 board
    // size 1 boat (1)
    // size 2 boat (2)
    // size 3 boat (1)
    // size 4 boat (1)
    // All together, suppose to hit 12
    cout << "Welcome to battleships";
    cout << "\nAll coordinates must be of form (r,c), where r is row and c is column\n";

    vector<vector<char>> BoardOfPlayer;
    BoardOfPlayer.resize(BOARD_DIM);
    for (int i = 0; i < BOARD_DIM; i++) {
        BoardOfPlayer[i].resize(BOARD_DIM);
    }
    for (int row = 0; row < BoardOfPlayer.size(); row++) {
        for (int col = 0; col < BoardOfPlayer[row].size(); col++) {
            BoardOfPlayer[row][col] = '.';
        }
    }
    // . for nothing
    // ! for a boat -> doesnt exist for enemy
    // X for a boat Destroyed
    // n for nothing hit.

    vector<vector<char>> BoardOfEnemy;
    vector<Coordinate> EnemyBoatsPosition; // Since player cant see where boats are, need store actual coordinates somewhere while replacing ! with .
    BoardOfEnemy.resize(BOARD_DIM);
    for(int i = 0; i < BOARD_DIM; i++)
        BoardOfEnemy[i].resize(BOARD_DIM);

    for (int row = 0; row < BoardOfEnemy.size(); row++) {
        for (int col = 0; col < BoardOfEnemy[row].size(); col++) {
            BoardOfEnemy[row][col] = '.';
        }
    }
    
    int SetUp_PrintFor = 0;
    //player represented with 0
    //enemy represented with 1

    PrintBoard(BoardOfPlayer, SetUp_PrintFor);
    SetUpBoard(BoardOfPlayer, SetUp_PrintFor, EnemyBoatsPosition);
    SetUp_PrintFor++;
    SetUpBoard(BoardOfEnemy, SetUp_PrintFor, EnemyBoatsPosition);

    int Turn = 0;
    bool Finish = false;
    Coordinate AttackingCoordinate;
    AttackingCoordinate.TrueCoordinate = false;
    CoordinateAndDirection CurrentAttackingMethod;
    string UserInput;
    while (!Finish) {
        while (Turn == 0) {
            PrintBoard(BoardOfPlayer, Turn);
            cout << "\n Your turn to attack... ";
            do {
                cout << "\nEnter coordinate you wish to attack ";
                cin >> UserInput;
                AttackingCoordinate = CoordinatesValid(UserInput);
            } while (AttackingCoordinate.TrueCoordinate == false);
            
            bool theyHit = false;
            for (int i = 0; i < EnemyBoatsPosition.size(); i++) {
                if (EnemyBoatsPosition[i].Row == AttackingCoordinate.Row && EnemyBoatsPosition[i].Col == AttackingCoordinate.Col) {
                    theyHit = true;
                    BoardOfEnemy[AttackingCoordinate.Row][AttackingCoordinate.Col] = 'X';
                    cout << "\n You hit their boat, your turn again ";
                    break;
                }
            }

            if(!theyHit){
                Turn = 1;
                cout << "\n You hit nothing";
            }
            PrintBoard(BoardOfEnemy, 1);
        }
        if (CheckIfNoMoreBoats(BoardOfPlayer) == true || CheckIfNoMoreBoats(BoardOfEnemy) == true)
            break;

        while (Turn == 1) {
            cout << "\n Enemy turn to attack... ";
            do {
                srand(time(0));
                AttackingCoordinate.Row = rand() % 9;
                Waiting();
                srand(time(0));
                AttackingCoordinate.Col = rand() % 9;
            } while (BoardOfPlayer[AttackingCoordinate.Row][AttackingCoordinate.Col] == 'n');

            if (ComputerPlaceForAttack.size() == 0) {

                CurrentAttackingMethod.DirAndValid.insert(make_pair('L', true));
                CurrentAttackingMethod.DirAndValid.insert(make_pair('R', true));
                CurrentAttackingMethod.DirAndValid.insert(make_pair('U', true));
                CurrentAttackingMethod.DirAndValid.insert(make_pair('D', true));
                ComputerPlaceForAttack.push_back(CurrentAttackingMethod);
                
                if (BoardOfPlayer[AttackingCoordinate.Row][AttackingCoordinate.Col] == '.') {
                    BoardOfPlayer[AttackingCoordinate.Row][AttackingCoordinate.Col] = 'n';
                    Turn = 0;
                }
                else {
                    if (AttackingCoordinate.Row == 8 && AttackingCoordinate.Col == 8) {
                        ComputerPlaceForAttack[0].DirAndValid[2] = false;
                        ComputerPlaceForAttack[0].DirAndValid[4] = false;
                    }
                    else if (AttackingCoordinate.Row == 0 && AttackingCoordinate.Col == 0) {
                        ComputerPlaceForAttack[0].DirAndValid[1] = false;
                        ComputerPlaceForAttack[0].DirAndValid[3] = false;
                    }
                    else if (AttackingCoordinate.Row == 0 && AttackingCoordinate.Col == 8) {
                        ComputerPlaceForAttack[0].DirAndValid[2] = false;
                        ComputerPlaceForAttack[0].DirAndValid[3] = false;
                    }
                    else if (AttackingCoordinate.Row == 8 && AttackingCoordinate.Col == 0) {
                        ComputerPlaceForAttack[0].DirAndValid[1] = false;
                        ComputerPlaceForAttack[0].DirAndValid[4] = false;
                    }
                    else if (AttackingCoordinate.Row == 0) {
                        ComputerPlaceForAttack[0].DirAndValid[3] = false;
                    }
                    else if (AttackingCoordinate.Row == 8) {
                        ComputerPlaceForAttack[0].DirAndValid[4] = false;
                    }
                    else if (AttackingCoordinate.Col == 0) {
                        ComputerPlaceForAttack[0].DirAndValid[1] = false;
                    }
                    else if (AttackingCoordinate.Col == 0) {
                        ComputerPlaceForAttack[0].DirAndValid[2] = false;
                    }

                }
            }



        }
    }
}

void PrintBoard(const vector<vector<char>>& BoardToPrint, const int& PrintFor) {
    if (PrintFor == 0)
        cout << "Your Board: \n";
    else
        cout << "Enemy Board: \n";

    cout << "   0  1  2  3  4  5  6  7  8 -> Col\n";
    
    for (int row = 0; row < BoardToPrint.size(); row++) {
        cout << row;
        for (int col = 0; col < BoardToPrint[row].size(); col++) {
            if (col % BOARD_DIM == 8)
                cout << "  " << BoardToPrint[row][col] << "\n";
            else
                cout << "  " << BoardToPrint[row][col];
        }
    }
    cout << "row\n";
}

//Check if coordinates are in correct format and its in correct range.
Coordinate CoordinatesValid(const string& CheckCoord) {
    Coordinate Temporary;
    if (CheckCoord.length() == 5) {
        if (CheckCoord[0] == '(' && CheckCoord[2] == ',' && CheckCoord[4] == ')') {
            if (isdigit(CheckCoord.at(1)) && isdigit(CheckCoord.at(3))) {
                Temporary.Row = int(CheckCoord[1]) - 48;
                Temporary.Col = int(CheckCoord[3]) - 48;
                if ((Temporary.Row >= 0 && Temporary.Row <= 8) && (Temporary.Col >= 0 && Temporary.Col <= 8)) {
                    Temporary.TrueCoordinate = true;
                    return Temporary;
                }
            }
        }
    }

    Temporary.TrueCoordinate = false;
    return Temporary;
}

void SetUpBoard(vector<vector<char>> &Board, int &ForWho, vector<Coordinate>& OnlyForEnemy) {
    Coordinate TemporaryCoordinate;
    TemporaryCoordinate.TrueCoordinate = false;
    char Orien;
    string TemporaryPlacerForOrien;
    if (ForWho == 0) {
        string Coordinate_Entered;
        for (int TheBoats = 0; TheBoats < 5; TheBoats++) {
            switch (TheBoats) {
                case 0:
                {
                    do {
                        cout << "Enter coordinate for boat of size 1: ";
                        cin >> Coordinate_Entered;
                        TemporaryCoordinate = CoordinatesValid(Coordinate_Entered);
                    } while (TemporaryCoordinate.TrueCoordinate == false);
                    Boat Boat_size1(1, TemporaryCoordinate, Board);
                    PrintBoard(Board, ForWho);
                    break;
                }
                case 1:
                {
                    do {
                        cout << "Enter coordinate for first boat of size 2: ";
                        cin >> Coordinate_Entered;
                        cout << "\nEnter V or H for orientation of boat ";
                        cin >> TemporaryPlacerForOrien;
                        TemporaryCoordinate = CoordinatesValid(Coordinate_Entered);
                        if (TemporaryPlacerForOrien.size() != 1)
                            Orien = 'N';
                        else
                            Orien = TemporaryPlacerForOrien[0];
                    } while (TemporaryCoordinate.TrueCoordinate == false || (Orien != 'V' && Orien != 'H'));
                    Boat Boat_size2_First(2, TemporaryCoordinate, Orien, Board);
                    PrintBoard(Board, ForWho);
                    break;
                }
                case 2: {
                    do {
                        cout << "Enter coordinate for second boat of size 2: ";
                        cin >> Coordinate_Entered;
                        cout << "\nEnter V or H for orientation of boat ";
                        cin >> TemporaryPlacerForOrien;
                        TemporaryCoordinate = CoordinatesValid(Coordinate_Entered);
                        if (TemporaryPlacerForOrien.size() != 1)
                            Orien = 'N';
                        else
                            Orien = TemporaryPlacerForOrien[0];
                    } while (TemporaryCoordinate.TrueCoordinate == false || (Orien != 'V' && Orien != 'H'));
                    Boat Boat_size2_First(2, TemporaryCoordinate, Orien, Board);
                    PrintBoard(Board, ForWho);
                    break;

                }
                case 3: {
                    do {
                        cout << "Enter coordinate for boat of size 3: ";
                        cin >> Coordinate_Entered;
                        cout << "\nEnter V or H for orientation of boat ";
                        cin >> TemporaryPlacerForOrien;
                        TemporaryCoordinate = CoordinatesValid(Coordinate_Entered);
                        if (TemporaryPlacerForOrien.size() != 1)
                            Orien = 'N';
                        else
                            Orien = TemporaryPlacerForOrien[0];
                    } while(TemporaryCoordinate.TrueCoordinate == false || (Orien != 'V' && Orien != 'H'));
                    Boat Boat_size2_First(3, TemporaryCoordinate, Orien, Board);
                    PrintBoard(Board, ForWho);
                    break;
                }
                case 4: {
                    do {
                        cout << "Enter coordinate for boat of size 4: ";
                        cin >> Coordinate_Entered;
                        cout << "\nEnter V or H for orientation of boat ";
                        cin >> TemporaryPlacerForOrien;
                        TemporaryCoordinate = CoordinatesValid(Coordinate_Entered);
                        if (TemporaryPlacerForOrien.size() != 1)
                            Orien = 'N';
                        else
                            Orien = TemporaryPlacerForOrien[0];
                    } while (TemporaryCoordinate.TrueCoordinate == false || (Orien != 'V' && Orien != 'H'));
                    Boat Boat_size2_First(4, TemporaryCoordinate, Orien, Board);
                    PrintBoard(Board, ForWho);
                    break;
                }

            }
        }

    }

    else {
        for (int BoatsPC = 0; BoatsPC < 5; BoatsPC++) {
            switch (BoatsPC) {
                case 0: {
                    srand(time(0));
                    TemporaryCoordinate.Row = rand() % 9;
                    Waiting(); // purpose of function is to wait for some random time by going through a loop
                    // and waiting until exits to gurantee generated number isnt same as previous one.
                    srand(time(0));
                    TemporaryCoordinate.Col = rand() % 9;
                    Boat Boatsize1(1, TemporaryCoordinate, Board);
                    break;
                }

                case 1: {
                    do {
                        srand(time(0));
                        TemporaryCoordinate.Row = rand() % 9;
                        Waiting();
                        srand(time(0));
                        TemporaryCoordinate.Col = rand() % 9;
                        Orien = GenerateOrien();
                    } while (!CheckIfEnoughSpace(TemporaryCoordinate, Orien, Board, 2));
                    Boat Boatsize2(2, TemporaryCoordinate, Orien, Board);
                    break;

                }
                case 2: {
                    do {
                        srand(time(0));
                        TemporaryCoordinate.Row = rand() % 9;
                        Waiting();
                        srand(time(0));
                        TemporaryCoordinate.Col = rand() % 9;
                        Orien = GenerateOrien();
                    } while (!CheckIfEnoughSpace(TemporaryCoordinate, Orien, Board, 2));
                    Boat Boatsize2(2, TemporaryCoordinate, Orien, Board);
                    break;
                }
                case 3: {
                    do {
                        srand(time(0));
                        TemporaryCoordinate.Row = rand() % 9;
                        Waiting();
                        srand(time(0));
                        TemporaryCoordinate.Col = rand() % 9;
                        Orien = GenerateOrien();
                    } while (!CheckIfEnoughSpace(TemporaryCoordinate, Orien, Board, 3));
                    Boat Boatsize3(3, TemporaryCoordinate, Orien, Board);
                    break;
                }
                case 4: {
                    do {
                        srand(time(0));
                        TemporaryCoordinate.Row = rand() % 9;
                        Waiting();
                        srand(time(0));
                        TemporaryCoordinate.Col = rand() % 9;
                        Orien = GenerateOrien();
                    } while (!CheckIfEnoughSpace(TemporaryCoordinate, Orien, Board, 4));
                    Boat Boatsize4(4, TemporaryCoordinate, Orien, Board);
                    break;
                }
            }

        }

        for (int R = 0; R < BOARD_DIM; R++) {
            for (int C = 0; C < BOARD_DIM; C++) {
                if (Board[R][C] == '!') {
                    TemporaryCoordinate.Row = R;
                    TemporaryCoordinate.Col = C;
                    OnlyForEnemy.push_back(TemporaryCoordinate);
                    Board[R][C] = '.';
                }
            }
        }
    }
}
bool CheckIfNoMoreBoats(const vector<vector<char>>& Board) {
    int tempChecker = 0;
    for (int row = 0; row < BOARD_DIM; row++) {
        for (int column = 0; column < BOARD_DIM; column++) {
            if (Board[row][column] == 'X')
                tempChecker++;
        }
    }

    if (tempChecker == 12)
        return true;
    else
        return false;
}

bool CheckIfEnoughSpace(Coordinate StartingPlace, char orientation, vector<vector<char>>& Board, const int& size) {
    int tempChecker = 0;
    if (orientation == 'V') {
        if (StartingPlace.Row + size > 9) 
            return false;
        else {
            for (int i = 0; i < size;i++) {
                if (Board[StartingPlace.Row + i][StartingPlace.Col] == '.')
                    tempChecker++;
            }
            if (size == tempChecker) 
                return true;
            else
                return false;
        }
    }
    else {
        if (StartingPlace.Col + size > 9) 
            return false;
        else {
            for (int i = 0; i < size;i++) {
                if (Board[StartingPlace.Row][StartingPlace.Col + i] == '.')
                    tempChecker++;
            }
            if (size == tempChecker) 
                return true;
            else
                return false;
        }
    }
}

char GenerateOrien() {
    srand(time(0));
    if (rand() % 2 == 0) {
        return 'H';
    }
    else {
        return 'V';
    }
}

void EnemyShootingTurn(vector<vector<char>>& Board){
}


void Waiting() {
    int start = -1;
    int counter = 0;
    int num = rand() % 4;
    srand(time(0));

    if (num == 0) 
        counter = 390;
    else if (num == 1)
        counter = 300;
    else if (num == 2)
        counter = 500;
    else
        counter = 250;

    while (start < counter) {
        start++;
     }
}