/**
 * File: narcissistics-numbers.cpp
 * -------------------------------
 * This file defines a program that prints out
 * the first 25 narcissistic numbers.
 */

#include <iostream>
#include <iomanip>
#include <cmath>
using namespace std;

#include "console.h"
#include "strlib.h"

/** Constants **/
static const int kNumToPrint = 25;

/**
 * Function: introduce
 * -------------------
 * Self-explanatory function used to inform the user what the
 * program is about to do.
 */
static void introduce() {
    cout << "Here's a great little program that prints out the first "
         << kNumToPrint << " narcissistic numbers." << endl << endl;
}

/**
 * Function: pow
 * -------------
 * Simple little function to quickly compute
 * base to the exp power.  Assumes base and
 * exp are small and positive.
 */
static int pow(int base, int exp) {
    int ans = 1;
    for (int i = 0; i < exp; i++) {
        ans *= base;
    }
    return ans;
}

/**
 * Function: isNarcissistic
 * ------------------------
 * Predicate function that returns true if and only
 * if the supplied number is narcissistic.
 */
static bool isNarcissistic(int n) {
    int original = n;
    int sum = 0;
    int numDigits = int(integerToString(n).size());

    while (n > 0) {
        sum += pow(n % 10, numDigits);
        n /= 10;
    }

    return sum == original;
}

/**
 * Function: printNarcissisticNumbers
 * ----------------------------------
 * As expected, printNarcissisticNumbers prints
 * out the first kNumToPrint narcissistic numbers,
 * in sequence, as it discovers them.
 */
static void printNarcissisticNumbers() {
    int numFound = 0;
    int n = 1;

    while (numFound < kNumToPrint) {
        if (isNarcissistic(n)) {
            numFound++;
            cout << setw(2) << numFound << ".) " << n << endl;
        }
        n++;
    }
    cout << "All done!" << endl;
}

/**
 * Function: main
 * --------------
 * Serves as the entry point to the entire program, as
 * all main functions do.  This one just passes the buck
 * to two helper functions.
 */
int main() {
    introduce();
    printNarcissisticNumbers();
    return 0;
}
