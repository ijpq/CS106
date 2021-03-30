/**
 * File: queen-safety-graphics.h
 * -----------------------------
 * Defines the collection of constants and UI functions that need to be
 * used by both queen-safety.cpp and queen-safety-graphics.cpp.
 */

#pragma once
#include <string>
#include "gwindow.h"

/** Constants **/
static const int kNumQueens	= 3;
static const int kBoardDimension = 8;
static const int kNumRows = kBoardDimension;
static const int kNumColumns = kBoardDimension;
static const std::string kWindowTitle = "Queen Safety";

/**
 * Function: drawSquare
 * --------------------
 * Draws the (row, col) square within the supplied window, using the supplied
 * color for the square's border.
 */
void drawSquare(GWindow& window, int row, int col, const std::string& color);

/**
 * Function: markLocation
 * ----------------------
 * Inserts the provided text into the square presumably already drawn at (row, col), and
 * draws that text in the provided color.
 */
void markLocation(GWindow& window, const std::string& text, int row, int col, const std::string& color);
