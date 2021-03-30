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
void GenerateOneRow(Grid<int> *g,
                    const string &line,
                    ColonyMeta &meta_info,
                    int& row_idx);

void InitGridRandomly(Grid<int> *g);
bool CheckNotCommentLine(const string &s);
void InitGrid(Grid<int> *g, fstream &file_ptr);
void Init(Grid<int> *grid_ptr, LifeDisplay *display_ptr);
void ChoicePrompt();
bool MakeChoice(int *choice_ptr);
void DisplayGrid(Grid<int> *g_ptr, LifeDisplay *d_ptr);
int getAlive(Grid<int> *p, int r, int c);
int ComputeLoc(int r, int c, Grid<int> *oldptr);
bool UpdateGrid(Grid<int> *g_ptr);
int RandomAge();
void FillGrid(Grid<int> *g, const int &x, const int &y, int value);
int QuitPrompt();
void ReInitGrid(int r, int c, Grid<int> *g);
bool GenerationLoop(int speed, Grid<int> *grid_ptr, LifeDisplay *display_ptr);
int StartingAnimation(int *choice_ptr, Grid<int> *grid_ptr, LifeDisplay *display_ptr);
/*
    define macro
 */
#define READ_FROM_FILE_AGE 1
#define ERROR_STATUS 9999

/**
 * Function: welcome
 * -----------------
 * Introduces the user to the Game of Life and its rules.
 */



/*
 * INIT
 */
void GenerateOneRow(Grid<int> *g,
                    const string &line,
                    ColonyMeta &meta_info,
                    int& row_idx) {
    if (isdigit(line[0]) != 0 ) { // if this line is a number
        // because the GenerateOneRow called while input file, which format is always rows firstly then cols.
        if (meta_info.num_of_rows == -1 ) {
            meta_info.num_of_rows = stoi(line);
        }
        else if (meta_info.num_of_cols == -1){
            meta_info.num_of_cols = stoi(line);
            ReInitGrid(meta_info.num_of_rows, meta_info.num_of_cols, g);
        }
    }
    else {
        for (size_t i_col = 0; i_col < line.size(); i_col++) {
            if (line[i_col] == 'X') {
                FillGrid(g, row_idx, i_col, READ_FROM_FILE_AGE);
            }
        }
        row_idx++;
    }
    return;
}

bool CheckNotCommentLine(const string &s) {
    bool is_valid = false;
    for (size_t i =0; i<s.size(); i++) {
        if (s[i] == ' ') {
            continue;
        } else if (s[i] == '#') { // is comment line
            break;
        } else if (s[i] - '0' < 10) { // is meta info line
            is_valid = true;
            break;
        } else if (s[i] - '-' == 0 || s[i] - 'X' == 0) { // is data
            is_valid = true;
            break;
        }
    }
    return is_valid;
}

void InitGridRandomly(Grid<int> *g) {
    auto width_grid = randomInteger(40,60);
    auto height_grid = randomInteger(40,60);

    ReInitGrid(height_grid, width_grid, g);
    printf("re init grid size: %d, %d", height_grid, width_grid);

    for (int r = 0; r < height_grid; r++) {
        for (int c = 0; c < width_grid; c++) {
            if (randomChance(0.5)) {
                FillGrid(g, r, c, 0);
            } else {
                FillGrid(g, r, c, RandomAge());
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
        std::string line;
        int row_idx = 0;
        while (true) {
            std::getline(file_ptr, line);
            if (file_ptr.eof()) break;
            if (CheckNotCommentLine(line)) {
                GenerateOneRow(g, line, meta_info, row_idx);
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
            cout << "you choose random initialization" << endl;
            break;
        } else {
            file_ptr.open(fileName);
            if (file_ptr) {
                cout << "open colony file successfully" << endl;
                goto success;
            } else {
                cout << "file open failed." << endl;
            }
        }
    }
success:
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
                break;
            }
        } //TODO further detect illegal input
        cout << "Illegal integet format. try again" << endl;
    }
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
        return 0; // while calculate the edge cell of grid, it will out of bounds, but the alive return 0 would be fine
    } else if (p->get(r,c) > 0) {
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

    switch (alive) {
        case 0: // loneliness
        case 1: // loneliness
        case 4: // overcrowd
            return -1;
        case 2: // stable
            return 0;
        case 3: // new born
            return 1;

    }
    if (alive > 4) return -1; // overcrowd
    throw std::runtime_error("unexpected status");
}


bool UpdateGrid(Grid<int> *g_ptr) {
    Grid<int> next_grid;

    int next_age = 0;

    auto detectStable = [](Grid<int>* grid_ptr) { // stable because hit or exceed max age
        for (auto r = 0; r < grid_ptr->numRows(); r++) {
            for (auto c = 0; c < grid_ptr->numCols(); c++) {
                if (grid_ptr->get(r,c) < kMaxAge && grid_ptr->get(r,c) > 0) {
                    return false;
                }
            }
        }
        return true;
    };
    if (detectStable(g_ptr)) { // stable
            return true;
    }

    next_grid.resize(g_ptr->numRows(), g_ptr->numCols());
    for (int r = 0; r < next_grid.numRows(); r++) {
        for (int c = 0; c < next_grid.numCols(); c++) {
            next_age = ComputeLoc(r, c, g_ptr);
            if (next_age == -1) {
                next_grid[r][c] = 0;
            } else {
                next_grid[r][c] = g_ptr->get(r,c) + next_age;
            }
        }
    }

    // return stable status
    if (*g_ptr == next_grid) {
        return true;
    } else {
        *g_ptr = next_grid;
        return false;
    }
}

/*
 * helper
 */
int RandomAge() {
    int age;
    age = randomInteger(1,kMaxAge);
    return age;
}

void FillGrid(Grid<int> *g, const int &x, const int &y, int value) {
    if (!g->inBounds(x, y)){
        
        std::vector<char> buf(512);
        sprintf(buf.data(), "out of bounds, loc: [x,y]:[%d, %d]\n", x, y);
        throw std::runtime_error(buf.data());
    }
    g->set(x, y, value);
    return;
}

int QuitPrompt() {
    string quit_ret;
    while (true) {
        quit_ret = getLine("Would you like to run another"); 
        if (quit_ret == "yes") {
            return 1;
        } else if (quit_ret == "no") {
            return 0;
        } else {
            cout << "please enter yes or on" << endl;
        }
    }
}

void ReInitGrid(int r, int c, Grid<int> *g) {
    g->resize(r,c);
    return;
}
/*
 * helper
 */


/*
 * START EMULATION
 */

bool GenerationLoop(int speed, Grid<int> *grid_ptr, LifeDisplay *display_ptr) {
    // return quit status
    bool manual = !speed;

    if (manual) {
        // manual mode
        while (true) {
            string advance_ret = getLine("please return to advance [or quit to end]"); 
            if (advance_ret == "quit") {
                while (true) {
                    if (QuitPrompt() == 1) { // start to a new simulation
                        return true;
                    } else if (QuitPrompt() == 0) { // actually close the program entirely
                        return false;
                    }
                }
            }
            if (UpdateGrid(grid_ptr)){ // stable
                break;
            }
            DisplayGrid(grid_ptr, display_ptr);
        }
    } else {
        // automatic
        GEvent e;
        GTimer timer(speed);
        timer.start();
        while (true) {
            e = waitForEvent(TIMER_EVENT | MOUSE_CLICKED);
            if (e.getEventType() == MOUSE_CLICKED) {
                if (QuitPrompt() == 1) {
                    return true;
                } else if (QuitPrompt() == 0) {
                    return false;
                }
            } else if (e.getEventType() == TIMER_TICKED) {
                if(UpdateGrid(grid_ptr)) { //stable
                    break;
                }
                DisplayGrid(grid_ptr, display_ptr);
            }
        }
        timer.stop();
    }
    return true;

}

/*
 * EMULATION CONFIG
 */
int StartingAnimation(int *choice_ptr, Grid<int> *grid_ptr, LifeDisplay *display_ptr) {
    if (*choice_ptr == 1) {
        return GenerationLoop(kMostFast,   grid_ptr, display_ptr);
    } else if (*choice_ptr == 2) {
        return GenerationLoop(kNotTooFast, grid_ptr, display_ptr);
    } else if (*choice_ptr == 3) {
        return GenerationLoop(kSlow, 		grid_ptr, display_ptr);
    } else if (*choice_ptr == 4) {
        return GenerationLoop(0, 			grid_ptr, display_ptr);
    }
    throw std::runtime_error("unexpected status");


}
/*
 * WELCOME INFORMATION
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
        if (StartingAnimation(choice_ptr, grid_ptr, display_ptr) == false) break;
    }
    return 0;
}
