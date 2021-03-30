/**
 * File: life-constants.h
 * ----------------------
 * Defines those constants which are shared by both the
 * life-graphics module and the main life module.
 */

#pragma once

/**
 * Constants
 * ---------
 * Both the graphics module and the main module need to agree
 * that cells of age kMaxAge and above are equally old--at least
 * for display purposes.
 */
const int kMaxAge = 12;


typedef struct {
    int num_of_rows;
    int num_of_cols;
} ColonyMeta;


#define OCCUPIED 1
#define MAXDEPTH 10000000
enum DisplaySpeed {
    kSlow = 2000,
    kNotTooFast = 500,
    kMostFast = 100,
};
