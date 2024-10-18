
#include "Boat.h" 

Coordinate Boat::CoordinatesValid(const string& CheckCoord){
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
Boat::Boat(int size, Coordinate StartingCoordinate, vector<vector<char>>& TheBoard) {
    PointStatus NewP;
    if (TheBoard[StartingCoordinate.Row][StartingCoordinate.Col] == '.') {
        NewP.A_Coordinate.Row = StartingCoordinate.Row;
        NewP.A_Coordinate.Col = StartingCoordinate.Col;
        NewP.status = true;
    }
    else {
        cout << "Position already taken please do it again\n";
        string Again;
        do {
            cout << "Enter coordinates again ";
            cin >> Again;
            StartingCoordinate = CoordinatesValid(Again);
        } while (StartingCoordinate.TrueCoordinate == false);
        NewP.A_Coordinate.Row = StartingCoordinate.Row;
        NewP.A_Coordinate.Col = StartingCoordinate.Col;
        NewP.status = true;
    }
    BoatStatus.push_back(NewP);
    TheBoard[BoatStatus[0].A_Coordinate.Row][BoatStatus[0].A_Coordinate.Col] = '!';
    Orientation = 'N';
    //N for none;
}
Boat::Boat(const int &size, Coordinate StartingCoordinate, char &ProposeOrientation, vector<vector<char>>& TheBoard) {
    bool SetUp = false;
    while (!SetUp) {
        int tempChecker = 0;
        //to check there is enough space on the section of the grid for the boat
        if (ProposeOrientation == 'V') {
            if (StartingCoordinate.Row + size > 9) {
                cout << "Not enough space to put ship vertically downwards, try again\n";
                SetUp = false;
            }
            else {
                for (int i = 0; i < size;i++) {
                    if (TheBoard[StartingCoordinate.Row + i][StartingCoordinate.Col] == '.')
                        tempChecker++;
                }
                if (size == tempChecker) {
                    SetUp = true;
                    PointStatus NewP;
                    for (int i = 0; i < size;i++) {
                        NewP.A_Coordinate.Row = StartingCoordinate.Row + i;
                        NewP.A_Coordinate.Col = StartingCoordinate.Col;
                        NewP.status = true;
                        BoatStatus.push_back(NewP);
                    }
                    break;
                }
                else {
                    cout << "Some positions is already occupied, please try again\n";
                }
            }   
        }
        else {
            if (StartingCoordinate.Col + size > 9) {
                cout << "Not enough space to put ship horizontally to the right, try again\n";
                SetUp = false;
            }
            else {
                for (int i = 0; i < size;i++) {
                    if (TheBoard[StartingCoordinate.Row][StartingCoordinate.Col + i] == '.')
                        tempChecker++;
                }
                if (size == tempChecker) {
                    SetUp = true;
                    PointStatus NewP;
                    for (int i = 0; i < size;i++) {
                        NewP.A_Coordinate.Row = StartingCoordinate.Row;
                        NewP.A_Coordinate.Col = StartingCoordinate.Col + i;
                        NewP.status = true;
                        BoatStatus.push_back(NewP);
                    }
                    break;
                }
                else {
                    cout << "Some positions is already occupied, please try again\n";
                }
            }
        }
        tempChecker = 0;
        cout << "\nOne of your paramaters is invalid, please try again ";
        string CoorAgain;
        string OrienAgain;
        do {
            cout << "Enter coordinate again ";
            cin >> CoorAgain;
            StartingCoordinate = CoordinatesValid(CoorAgain);
            cout << "Enter position you want boat to be, V or H ";
            cin >> OrienAgain;
            if (OrienAgain.size() != 1) 
                ProposeOrientation = 'N';
            else
                ProposeOrientation = OrienAgain[0];
            
        } while (
            (!StartingCoordinate.TrueCoordinate) || 
            (ProposeOrientation != 'V' && ProposeOrientation != 'H')
            );
    }

    for (int s = 0; s < size; s++) {
        TheBoard[BoatStatus[s].A_Coordinate.Row][BoatStatus[s].A_Coordinate.Col] = '!';
    }

    Orientation = ProposeOrientation;

}

Boat::~Boat() {

}
char Boat::GetOrientation() {
    return Orientation;
}

