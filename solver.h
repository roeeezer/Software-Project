/*
 * solver.h
 *
 * solver module contains the auto-fill process and all the functions that searches for solutions of the
 * sudoku-board such as exhaustive backtracking, ILP solver and functions
 *
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include <stdio.h>
#include "board.h"
#include "stack.h"
#include "error.h"
#include "moveNode.h"
#include "gurobi.h"


/*@post: valuesList contains all the valid values we can assign to the (i,j) cell in the
 *  board bSol in ascending order
 *  the functions returns the size of the list - @ret=size, namely the valid values
 *  are valuesList[0],...,valuesList[size-1]*/
int createValidValuesList(int* valuesList,board* bSol,int i,int j);
/*delete the ind'th cell value in the array by shifting the array cells 1 step to the left
 * (only the cells to the right of the ind'th cell of course)*/
void deleteIndexFromList(int* valuesList,int ind,int size);

/*@pre bSol is a copy of the board*/
int exhaustiveBackTracingWithStack(board* b,board* bSol);
/*the options of ind are described in game.h*/
ERROR autofillBoard(board* b,board* bt,moveNode* move,int gameMode,int printInd);
ERROR solveILP(board *b);
ERROR solveLPForTargetCell(board *pBoard, int i, int j, int *cellValues, double *cellScores, int *numOfValuesInCell);
ERROR solveLPWithThreshold(board *pBoard, double threshold);
int fillXRandomCells(board* pBoard, int x);
void clearRandomCell(board* pBoard);
/**
 * Simple autofill, to be used by LP methods for adding "obvious" cells before starting LP/ILP
 * Writes result to board.
 * @param pBoard board to be filled.
 */
ERROR simpleAutofill(board *pBoard);

int boardHasASolution(board* pBoard);
/*TMP just for debuging the real function with stack*/
int exhaustiveBackTracingRec(board* b,board* bSol,int startInd);
#endif /* SOLVER_H_ */
