#include "queen-safety-graphics.h"
#include "glabel.h"  // for GLabel
using namespace std;

/** Private constants **/
static const string kFont = "Helvetica-24-Bold";
static const double kSquareSize = 32;
static const int kMillisecondDelay = 250;

/**
 * Function: drawSquare
 * --------------------
 * Reframes the request to draw a square into GWindow-specific actions.
 */
void drawSquare(GWindow& window, int row, int col, const string& color) {
  double ulx = (window.getWidth() - kBoardDimension * kSquareSize)/2 + col * kSquareSize;
  double uly = (window.getHeight() - kBoardDimension * kSquareSize)/2 + row * kSquareSize;
  window.setColor(color);
  window.drawRect(ulx, uly, kSquareSize, kSquareSize);
}

/**
 * Function: markLocation
 * ----------------------
 * Reframes the the request to mark a location into GWindow-specific actions.  The user
 * is expected to pass in text strings or length one, else it can't be expected to wedge
 * the entire string inside the square.
 */
void markLocation(GWindow& window, const string& text, int row, int col, const string& color) {
  double cx = (window.getWidth() - kBoardDimension * kSquareSize)/2 + col * kSquareSize + kSquareSize/2;
  double cy = (window.getHeight() - kBoardDimension * kSquareSize)/2 + row * kSquareSize + kSquareSize/2;
  window.setColor(color);
  
  GText *label = new GText(text); // don't ask.. needs to be dynamically allocated
  label->setFont(kFont);
  label->setColor(color);
  window.add(label, cx - label->getWidth()/2, cy + label->getFontAscent()/2);
  pause(kMillisecondDelay);
}
