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
#include <unistd.h>
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "gevents.h" // for mouse event detection
#include "strlib.h"
#include "grid.h"
#include "gtimer.h"
#include "random.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay

/*
 * declaration functions.
 */

void DisplayGrid(Grid<int> *, LifeDisplay *);

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

int RandomAge() {
    int age;
    age = randomInteger(1,kMaxAge);
    return age;
}
//
void FillGrid(Grid<int> *g, const int &x, const int &y, int is_occupied) {
    if (!is_occupied) {
        g->set(x, y, 0);
    }
    else {
        int age = RandomAge();
        g->set(x, y, age);
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
    int width_grid = 50, height_grid = 50;
    //width_grid = randomInteger(40,60);
    //height_grid = randomInteger(40,60);

    ReInitGrid(height_grid, width_grid, g);

    for (int r = 0; r < width_grid; r++) {
        for (int c = 0; c < height_grid; c++) {
            if (randomChance(0.5)) {
                FillGrid(g, r, c, OCCUPIED);
            } else {
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

void Init(Grid<int> *grid_ptr, LifeDisplay *display_ptr) {
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
    DisplayGrid(grid_ptr, display_ptr);
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
//                cout << *choice_ptr << endl;
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
void DisplayGrid(Grid<int> *g_ptr, LifeDisplay *d_ptr) {
    d_ptr->setDimensions(g_ptr->numRows(),g_ptr->numCols());
    for (int r = 0; r < g_ptr->numRows(); ++r) {
        for (int c = 0; c < g_ptr->numCols(); ++c) {
            if (!(g_ptr->inBounds(r,c))) {
                cout << "row col error" << endl;
                exit(-1);
            }
            d_ptr->drawCellAt(r, c, g_ptr->get(r, c));
        }
    }
    d_ptr->repaint();
}
int getAlive(Grid<int> *p, int r, int c) {
    if (!(p->inBounds(r,c))) {
        return 0;
    } else if (p->get(r,c) != 0) {
        return 1;
    }
    return 0;
}

int ComputeLoc(int r, int c, Grid<int> *oldptr) {
    int alive = 0;
    alive += getAlive(oldptr, r-1, c-1);
    alive += getAlive(oldptr, r-1, c);
    alive += getAlive(oldptr, r-1, c+1);

    alive += getAlive(oldptr, r, c-1);
    alive += getAlive(oldptr, r, c+1);

    alive += getAlive(oldptr, r+1, c-1);
    alive += getAlive(oldptr, r+1, c);
    alive += getAlive(oldptr, r+1, c+1);

    if (alive <= 1) {
        return 0; // will be empty
    } else if (alive == 2 ) {
        return 1; // will be stable.
    } else if (alive == 3 ) {
        return 2; // will born.
    } else if (alive >= 4) {
        return 3; // will be empty.
    }
    return -1;

}

void UpdateGrid(Grid<int> *g_ptr, bool &stable) {
    Grid<int> next_grid;
    int next_status = -1;
    int age;


    next_grid.resize(g_ptr->numRows(), g_ptr->numCols());
    for (int r = 0; r < next_grid.numRows(); r++) {
        for (int c = 0; c < next_grid.numCols(); c++) {
            next_status = ComputeLoc(r, c, g_ptr);
            if (next_status == -1) {
                cout << "found a bug." << endl;
                exit(-1);
            }
            if (next_status == 1) {
                next_grid[r][c] = g_ptr->get(r,c);
            } else if (next_status == 0 || next_status == 3) {
                next_grid[r][c] = 0;
            } else {
                age = RandomAge();
//                age = kMaxAge;
                next_grid[r][c] = age;
            }
        }
    }


    if (next_grid == *g_ptr) {
        stable = true;
    } else {
        stable = false;
    }
    *g_ptr = next_grid;
}


int QuitPrompt(int *status_ptr) {
    string quit_ret;
    while (true) {
        quit_ret = getLine("Would you ..");
        if (quit_ret == "yes") {
            return 1;
        } else if (quit_ret == "no") {
            *status_ptr = 1;
            return 1;
        } else {
            cout << "please enter yes or on" << endl;
        }
    }
}

void StableCheck(bool &stable) {
    stable = false;
//    if (stable) {
//        cout << "stop because STABLE" << endl;
//        return true;
//    } else {
//        return false;
//    }

    }


void GenerationLoop(int speed, Grid<int> *grid_ptr, int *status_ptr, LifeDisplay *display_ptr) {
    bool manual = !speed;
    bool stable = false;

    if (manual) {
        // manual mode
        while (true) {
            // if stable , exit.
            StableCheck(stable);
            string advance_ret = getLine("please return to ..");
            if (advance_ret == "quit") {
                string quit_ret;
                while (true) {
                    if (QuitPrompt(status_ptr) == 1 ) {
                        return;
                    }
                }
            }
            UpdateGrid(grid_ptr, stable);
            DisplayGrid(grid_ptr, display_ptr);
        }
    } else {
        // automatic
        GEvent e;
        GTimer timer(speed);
        timer.start();
        while (true) {
            // if stable , exit.
            e = waitForEvent(TIMER_EVENT | MOUSE_CLICKED);
//            StableCheck(stable);
            if (e.getEventType() == MOUSE_CLICKED) {
                if (QuitPrompt(status_ptr) == 1 ) {
                    return;
                }
            } else if (e.getEventType() == TIMER_TICKED) {
                UpdateGrid(grid_ptr, stable);
                DisplayGrid(grid_ptr, display_ptr);
            }
        }
        timer.stop();
    }

}

int StartingAnimation(int *choice_ptr, Grid<int> *grid_ptr, LifeDisplay *display_ptr) {
    int quit_status = 0;
    int *q_status_ptr = &quit_status;
    if (*choice_ptr == 1) {
        GenerationLoop(kMostFast,   grid_ptr, q_status_ptr, display_ptr);
    } else if (*choice_ptr == 2) {
        GenerationLoop(kNotTooFast, grid_ptr, q_status_ptr, display_ptr);
    } else if (*choice_ptr == 3) {
        GenerationLoop(kSlow, 		grid_ptr, q_status_ptr, display_ptr);
    } else if (*choice_ptr == 4) {
        GenerationLoop(0, 			grid_ptr, q_status_ptr, display_ptr);
    }
    if (*q_status_ptr == 1) {
        return false;
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

    LifeDisplay *display_ptr = &display;

    Grid<int> graph;// TODO this grid.
    Grid<int> *grid_ptr =&graph;

    int simulation_choice = -1;
    int *choice_ptr = &simulation_choice;
    welcome();
    while (true) {
        Init(grid_ptr, display_ptr);
        MakeChoice(choice_ptr);
        if (!StartingAnimation(choice_ptr, grid_ptr, display_ptr)) {
            break;
        }
    }
    return 0;
}
