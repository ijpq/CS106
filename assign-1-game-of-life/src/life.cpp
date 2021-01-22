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

void RandomValue(int &val) {
    srand(time(NULL));
    val = rand();
    return;
}

void FillGrid(Grid<int> *g, const int &x, const int &y, int is_occupied) {
    if (!is_occupied) {
        g->set(x, y, -1);
    }
    else {
        int value;
        RandomValue(value);
        value = (value%kMaxAge) + 1;
        g->set(x, y, value);
    }
    return;
}

void ReInitGrid(int r, int c, Grid<int> *g) {
    g->resize(r,c);
    return;
}

void GenerateOneRow(Grid<int> *g,
                    const string &line,
                    ColonyMeta &meta_info,
                    unsigned int *no_row_ptr) {
    if (isdigit(line[0]) != 0 ) {
        if (meta_info.num_of_rows == -1 ) {
            meta_info.num_of_rows = stoi(line);
        }
        else if (meta_info.num_of_cols == -1){
            meta_info.num_of_cols = stoi(line);
            ReInitGrid(meta_info.num_of_rows, meta_info.num_of_cols, g);
        }
        else {
            cout << "unexpected file format" <<endl;
            exit(-1);
        }
    }
    else {
        (*no_row_ptr)++;
        int x = *no_row_ptr;
        for (size_t i_col = 0; i_col < line.size(); i_col++) {
            if (line[i_col] == 'X') {
                FillGrid(g, x, i_col, OCCUPIED);
            }
        }
    }
    return;
}

bool CheckValidLine(const string &s) {
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

void InitGridRandomly(Grid<int> *g) {
    int width_grid = 0, height_grid = 0;
    RandomValue(width_grid);
    width_grid = width_grid % 21 + 40;

    RandomValue(height_grid);
    height_grid = height_grid % 21 + 40;

    ReInitGrid(height_grid, width_grid, g);

    for (int r = 0; r < width_grid; r++) {
        for (int c = 0; c < height_grid; c++) {
            srand(time(NULL));
            if (rand() % 2 == 0) {
                // occupied
                FillGrid(g, r, c, OCCUPIED);
            }
            else {
                // unoccupied.
                FillGrid(g, r, c, !OCCUPIED);
            }
        }
    }

    return ;
}

void InitGrid(Grid<int> *g, fstream &file_ptr){
    if (!file_ptr.is_open()) {
        InitGridRandomly(g);
    }
    else {
        ColonyMeta meta_info = {-1, -1};
        string line("");
        unsigned int NoRow = 0;
        unsigned int *pNoRow = &NoRow;
        while (true) {
            std::getline(file_ptr, line);
            if (CheckValidLine(line)) {
                GenerateOneRow(g, line, meta_info, pNoRow);
            }
        }

    }
    return ;
}

void Init(Grid<int> *grid_ptr) {
    fstream file_ptr;
    while (true) {
        cout << "You can start your colony with random cells or read from a prepared file.\n";
        string fileName = getLine("Enter name of colony file (or RETURN to seed randomly):\n");
        if (fileName.empty()) {
//            cout << "you choose random initialization" << endl;
            break;
        } else {
            file_ptr.open(fileName);
            if (file_ptr) {
                cout << "open colony file successfully" << endl;
                break;
            } else {
                cout << "file open failed." << endl;
            }
        }
    }
    InitGrid(grid_ptr, file_ptr);
    return;
}

void ChoicePrompt() {
    cout << "You choose how fast to run the simulation" <<endl;
    cout << "\t1 = As fast as this chip can go!" <<endl;
    cout << "\t2 = Not too fast, this is a school zone." <<endl;
    cout << "\t3 = Nice and slow so I can watch everthing that happens" <<endl;
    cout << "\t4 = Require enter key be pressed before advancing to next generation" <<endl;
    cout << "Your choice: ";
    return;
}

bool MakeChoice(int *choice_ptr) {
    ChoicePrompt();
    string choice_str("");
    while(true) {
        choice_str = getLine("");
        if (choice_str.size() == 1) {
            if (choice_str[0]-'0' >= 1 && choice_str[0]-'0' <= 4) {
                *choice_ptr = choice_str[0]-'0';
                cout << *choice_ptr << endl;
                break;
            }
        } //TODO further detect illegal input
        cout << "Illegal integet format. try again" << endl;
    }
    //if (*choice_ptr != 0) {
    //    // TODO set choice bit.
    //}
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

    Grid<int> graph;// TODO this grid.
    Grid<int> *grid_ptr =&graph;

    int simulation_choice = -1;
    int *choice_ptr = &simulation_choice;
    welcome();
    Init(grid_ptr);
    MakeChoice(choice_ptr);



    
    return 0;
}
