/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <cstdio>
#include <fstream>
#include <cstdlib>
#include <string>
#include <ctime>
#include <cctype>
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "strlib.h"
#include "grid.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay
#define OCCUPIED 1

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */
static void welcome() {
    cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
    cout << "Cells live and die by the following rules:" << endl << endl;
    cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
    cout << "\tLocations with 2 neighbors remain stable" << endl;
    cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
    cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
    cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
    getLine("Hit [enter] to continue....   ");
}

void randomValue(int &val) {
    srand(time(NULL));
    val = rand();
    return;
}

void fillGrid(Grid<int> *g, const int &x, const int &y, int IsOccupied) {
    if (!IsOccupied) {
        g->set(x, y, -1);
    }
    else {
        int value;
        randomValue(value);
        value = (value%kMaxAge) + 1;
        g->set(x, y, value);
    }
    return;
}

void reInitializeGrid(int r, int c, Grid<int> *g) {
    g->resize(r,c);
    return;
}

void generateOneRow(Grid<int> *g,
                    const string &line,
                    colonyMeta &MetaInfo,
                    unsigned int *pNoRow) {
    if (isdigit(line[0]) != 0 ) {
        if (MetaInfo.NumOfRows == -1 ) {
            MetaInfo.NumOfRows = stoi(line);
        }
        else if (MetaInfo.NumOfCols == -1){
            MetaInfo.NumOfCols = stoi(line);
            reInitializeGrid(MetaInfo.NumOfRows, MetaInfo.NumOfCols, g);
        }
        else {
            cout << "unexpected file format" <<endl;
            exit(-1);
        }
    }
    else {
        (*pNoRow)++;
        int x = *pNoRow;
        for (size_t ColIdx = 0; ColIdx < line.size(); ColIdx++) {
            if (line[ColIdx] == 'X') {
                fillGrid(g, x, ColIdx, OCCUPIED);
            }
        }
    }
    return;
}

bool checkIsValidline(const string &s) {
    bool isvalid = false;
    for (size_t i =0; i<s.size(); i++) {
        if (s[i] == ' ') {
            continue;
        }
        else if (s[i] == '#') {
            break;
        }
        else {
            isvalid = true;
            break;
        }
    }
    return isvalid;
}

void initGridRandomly(Grid<int> *g) {
    int WidthGrid = 0, HeightGrid = 0;
    randomValue(WidthGrid);
    WidthGrid = WidthGrid % 21 + 40;

    randomValue(HeightGrid);
    HeightGrid = HeightGrid % 21 + 40;

    reInitializeGrid(HeightGrid, WidthGrid, g);

    for (int r = 0; r < WidthGrid; r++) {
        for (int c = 0; c < HeightGrid; c++) {
            srand(time(NULL));
            if (rand() % 2 == 0) {
                // occupied
                fillGrid(g, r, c, OCCUPIED);
            }
            else {
                // unoccupied.
                fillGrid(g, r, c, !OCCUPIED);
            }
        }
    }

    return ;
}

void initGrid(Grid<int> *g, fstream &pFile){
    if (!pFile) {
        initGridRandomly(g);
    }
    else {
        colonyMeta MetaInfo = {-1, -1};
        string line("");
        unsigned int NoRow = 0;
        unsigned int *pNoRow = &NoRow;
        while (true) {
            std::getline(pFile, line);
            if (checkIsValidline(line)) {
                generateOneRow(g, line, MetaInfo, pNoRow);
            }
        }

    }
    return ;
}

void init(Grid<int> *pGrid) {
    fstream pFile;
    while (true) {
        cout << "You can start your colony with random cells or read from a prepared file.\n";
        string fileName = getLine("Enter name of colony file (or RETURN to seed randomly):\n");
        if (fileName.empty()) {
            break;
        }
        pFile.open(fileName);
        if (pFile) {
            cout << "open colony file successfully" << endl;
            break;
        }
        else {
            cout << "colony file not exists or open failed." << endl;
        }

    }
    initGrid(pGrid, pFile);
    return;
}

void choicePrompt() {
    cout << "You choose how fast to run the simulation" <<endl;
    cout << "\t1 = ..." <<endl;
    cout << "\t2 = ..." <<endl;
    cout << "\t3 = ..." <<endl;
    cout << "Your choice: ";
    return;
}

bool makeChoice(int *pChoice) {
    choicePrompt();
    string choiceStr("");
      while(true) {
        choiceStr = getLine("enter an integer\n");
        if (choiceStr.size() == 1) {
            if (choiceStr[0]-'0' >= 1 && choiceStr[0]-'0' <= 4) {
                *pChoice = choiceStr[0]-'0';
                break;
            }
        } //TODO further detect illegal input
        cout << "Illegal integet format. try again" << endl;
    }
    if (*pChoice != 0) {
        // TODO set choice bit.
    }
    return true;
}

/**
 * Function: main
 * --------------
 * Provides the entry point of the entire program.
 */
int main() {
    LifeDisplay display;
    display.setTitle("Game of Life");

    Grid<int> G;// TODO this grid.
    Grid<int> *pGrid =&G;

    int Simulation_choice = -1;
    int *pChoice = &Simulation_choice;
    welcome();
    init(pGrid);
    makeChoice(pChoice);
    
    return 0;
}
