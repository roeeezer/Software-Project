
#ifndef GIT_GUROBI_H
#define GIT_GUROBI_H

#include "error.h"
#include "gurobi_c.h"

#define DEBUG 1

typedef struct{
   int row, col, val;
}VAR;

/**
 *
 * @param N number of sudoku variables
 * @param pBoard, assumed to have been autofilled at least once to avoid wastefulness
 * @return the number of vars needed for gurobi
 */
int countVars(board *pBoard, int N);
/**
 * create block vars
 */

#endif
