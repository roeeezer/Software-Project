
#ifndef GIT_GUROBI_H
#define GIT_GUROBI_H

#include "gurobi_c.h"
#include "error.h"
#include "board.h"

#define DEBUG 1

typedef struct{
   int row, col, val;
}VAR;

ERROR setUpGurobi(board *pBoard, int ilp, VAR **resultVars, double **solValues, int *resultCount);

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
