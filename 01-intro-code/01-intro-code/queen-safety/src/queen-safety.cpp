#include <iostream>
#include <string>
using namespace std;

#include "console.h"
#include "strlib.h"
#include "simpio.h"
#include "random.h"
#include "gevents.h"
#include "gwindow.h"
#include "grid.h"
#include "queen-safety-graphics.h"

/**
 * Function: clearBoard
 * --------------------
 * Ensures that every location on the board is devoid of queenage, and
 * updates the graphics window--presumably set up to mirror the data model
 * for the board--so that all locations are empty.
 */
static void clearBoard(Grid<bool>& board, GWindow& window) {
  for (int row = 0; row < board.numRows(); row++) {
    for (int col = 0; col < board.numCols(); col++) {
      board[row][col] = false;
      drawSquare(window, row, col, "Blue");
    }
  }
}

/**
 * Function: placeRandomQueens
 * ---------------------------
 * Searches for kNumQueens empty locations on the board, and updates
 * those locations so that each is occupied by a queen.
 */
static void placeRandomQueens(Grid<bool>& board, GWindow& window) {
  int numQueensPlaced = 0;
  while (numQueensPlaced < kNumQueens) {
    int row = randomInteger(0, board.numRows() - 1);
    int col = randomInteger(0, board.numCols() - 1);
    if (!board[row][col]) {
      board[row][col] = true;
      markLocation(window, "Q", row, col, "Black");
      numQueensPlaced++;
    }
  }
}

/**
 * Function: isDirectionSafe
 * -------------------------
 * Returns true if and only if the supplied (row, col) coordinate is
 * vulnerable from the (drow, dcol) direction.  Each of drow and dcol
 * is drawn from the set {-1, 0, 1}, so that (drow, dcol) pairs like (1, 1),
 * (0, -1), (1, 0), and (-1, -1) represent southeast, west, south, and northwest
 * directions, respectively.
 */
static bool isDirectionSafe(Grid<bool>& board, int row, int col, int drow, int dcol) {
  if (drow == 0 && dcol == 0) return true;
  
  row += drow;
  col += dcol;
  while (board.inBounds(row, col) && !board[row][col]) {
    row += drow;
    col += dcol;
  }
  
  return !board.inBounds(row, col);
}

/**
 * Function: isSafe
 * ----------------
 * Returns true if and only if the supplied (row, col) coordinate is
 * safe from attack.  Note that most of the interesting work is done
 * by the isDirectionSafe function.
 */
static bool isSafe(Grid<bool>& board, int row, int col) {
  for (int drow  = -1; drow  <= 1; drow++) {
    for (int dcol = -1; dcol <= 1; dcol++) {
      if (!isDirectionSafe(board, row, col, drow, dcol))
        return false;
    }
  }
  
  return true;
}

/**
 * Function: identifySafeLocations
 * -------------------------------
 * Iterates over the entire board and updates the companion window
 * with a collection of green S's and angry red X's as a way of communicating
 * which locations are safe from attack, and which are exposed to one or more
 * queens.
 */
static void identifySafeLocations(Grid<bool>& board, GWindow& window) {
  for (int row = 0; row < board.numRows(); row++) {
    for (int col = 0; col < board.numCols(); col++) {
      if (!board[row][col]) {
        if (isSafe(board, row, col)) {
          markLocation(window, "S", row, col, "Green");
        } else {
          markLocation(window, "X", row, col, "Red");
        }
      }
    }
  }
}

/**
 * Function: main
 * --------------
 * Defines the entry point of the entire program.  This version
 * is a superset of the version presented in the handout, because
 * it includes code that actually creates and updates a graphics window.
 *
 * Fundamentally, the program does the following:
 * + Constructs an 8-by-8 grid of Booleans,
 * + Populates that board with 8 randomly placed queens
 * + Waits for the user to click the mouse anywhere in the graphics window, and then
 * + updates all locations not occupied by a queen so signal whether they're safe
 *   or exposed.
 */
int main() {
    GWindow window;
    window.setWindowTitle(kWindowTitle);
    window.setVisible(true);
    Grid<bool> board(kNumRows, kNumColumns);
    clearBoard(board, window);
    placeRandomQueens(board, window);
    cout << "Click the mouse anywhere in the window to see which locations are safe." << endl;
    waitForClick();
    identifySafeLocations(board, window);
    cout << "All done!" << endl;
    cout << "Click the mouse anywhere in the window to close it." << endl;
    waitForClick();
    window.close();
    return 0;
}
