/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <cstdio>
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "strlib.h"
#include "grid.h"

#include <string>

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

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

//TODO generate data in grid.
void initGrid(Grid<int> *g, int isRandom, string fileName){
    cout << "init Grid finished" << endl;
}

void init(Grid<int> *pGrid) {
    cout << "You can ..\n";
    string fileName = getLine("Enter name of colony file ...\n");
    int isRandom = 0;
    if (!fileName.empty()) {
       isRandom = 1; 
    }
    initGrid(pGrid, isRandom, fileName);
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

bool makeChoice() {
    choicePrompt();
    string choiceStr("");
    int acceptChoice = 0;
    while(true) {
        choiceStr = getLine("enter an integer\n");
        if (choiceStr.size() == 1) {
            if (choiceStr[0]-'0' >= 1 && choiceStr[0]-'0' <= 4) {
                acceptChoice = choiceStr[0]-'0';
                break;
            }
        } //TODO further detect illegal input
        cout << "Illegal integet format. try again" << endl;
    }
    if (acceptChoice != 0) {
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
    welcome();
    Grid<int> g(3,3,1);// TODO this grid.
    Grid<int> *pGrid = &g;
    init(pGrid);
    makeChoice();
    
    return 0;
}
