/*
 * board.h
 *
 *      Author: roee
 */

#ifndef BOARD_H_
#define BOARD_H_
#include <stdio.h>
#include "moveNode.h"
#define INIT_MODE 0
#define EDIT_MODE 1
#define SOLVE_MODE 2
#define BOARD_SOLVED_CORRECTLY_MODE 3
/*this is for redoInd: if redoInd==REDO_COMMAND_IND we should print every change in the board
 * and shouldn't track the changes in the undoList
 * if printInd==1 we should print the changes in the board */
#define STANDART_COMMAND_IND 0
#define REDO_COMMAND_IND 1
/*Board Structure:
 * the board structure was necessary because the modules game and solver need to
 * exchange game boards among them in a way that is not depended on the data structure
 * that stores the board's data (the numbers) because we want the implementation of the
 * board to be like a "black box" to the other modules.
 *
 * the board contains the array "boardArr" that contains the board cell's values
 * such that negative values represent fixed cell and zero values represent empty cells
 * in addition the board contains the members:
 * rows - number of rows in each block of the soduko board
 * columns - number of columns in each block of the soduko board
 * squareSideSize = rows*columns - saved for readability and convenience
 * */
/*cell Types in boardTypes of the game */
#define REGULAR_CELL 0
#define FIXED_CELL 1
#define ERRONEOUS_CELL 2
typedef struct{
	int* boardArr;
	int rows;
	int columns;
	/*meaning blockRows and blockColumns
	 * n = rows, m = =columns*/
	int squareSideSize;
	int emptyCellsCounter;


}board;
board* createBoard(int blockRows,int blockColumns);
void destroyBoard(board* board);
/*@pre: i<board.rows && j<board.columns
 * returns the value of the of the (i,j) cell in the board*/
int getCell(board* Pboard,int i,int j);
/*@pre: i<board.rows && j<board.columns
 * sets the value of the of the (i,j) cell in the board to be val*/
void setCell(board* Pboard,int i,int j,int val);
/*@post: all cells in the board contains the value: v*/
void resetBoard(board* b,int v);
/*returns 1 if cell is empty*/
int emptyCell(board* Pboard,int i,int j);
/*prints the board according the format described in the instructions*/
int printBoard(board* b,board* bTypes,int gameMode,int markErrors);
/*prints row of blocks number blockRow in the board
 * @pre: 0<=blockRow<#rowsOfBlocks=board.columns*/
void printRowOfBlocks(board* b,int blockRow,board* bTypes,int gameMode,int markErrors);
/*prints cell row number rowInBlocksRow in the row of blocks number blocksRow
 * @pre: 0<=blockRow<#rowsOfBlocks=board.columns
 * @pre: 0<=rowInBlocksRow<#(number of cell rows in one block)=board.rows*/
void printOneRowInRowsOfBlocks(board* b,int blocksRow,int boardRow,board* bTypes,int gameMode,int markErrors);
/*
 * @pre: 0<=blockRow<#rowsOfBlocks=board.columns
 * @pre: 0<=rowInBlocksRow<#(number of cell rows in one block)=board.rows
 * @pre: 0<=blockComlumnInSingleRow<#(columns of blocks in board)=b.rows*/
void printSingleRowInSingleBlock(board* b,int blocksRow,int boardRow,
		int blockComlumnInSingleRow,board* bTypes,int gameMode,int markErrors);
/*@ret == 1 if and only if all cells in board are filled.*/
int boardSolved(board * pBoard);
/*prints a single cell according to the described format.
 * negative values represent fixed cell and zero values represent empty cells*/
void printCellValue(board* b,int i,int j,board* bTypes,int gameMode,int markErrors);
/*@ret>1 if and only if the assignment of value v in the (i,j) cell follows the soduko rules*/
int validAsignment(board* b,int v,int i,int j);
/*returns the next empty cell index in the board s.t $result and fromInd is 1D index
 * if cell[fromInd] is empty then res=fromInd
 * if the board is full res = MaxIndex+1 */
int findNextEmptyCell(board* b,int fromInd);
/*convert one 1D index in the board, namely the index of a cell as if the board
 * was a continuous 1 dimension array, to a pair of 2D indices representing the
 * same cell as if the array is a matrix or 2 dimension array*/
void oneDto2Dindices(board* b,int* indices,int oneDind);
/* @pre: copy->rows == orig->rows && copy->columns == orig->columns
 * replace the values of the cells in the board copy to have the same values
 * of the cells in the board orig*/
void copyBoard(board* copy,board* orig);
void copyBoardAndUpdateMove(board* copy,board* orig,moveNode* move);
int boardHasASolution(board* b);
/*$ret==1 iff the bTypes contains and erroneous cell*/
int erroneousBoard(board* bTypes);

/*ind=0 for checking if this set command causes erroneous cells
 * ind=1 for checking if this set command causes erroneous cells and updating
 * the erroneous cells in bTypes (adding of removing erroneous flags according to the value in th set)
 * ind=2 if we want to set a fixed cell (i,j) and to check if the set will cause a collision with
 * other fixed cells - used in function boardContainsFixedErroneousCells
 * getCell(i,j) current value does not affect this function!
 * @POST: $RET=1 iff the answer to the question determined by ind is YES*/

int setCausesErroneousCell(board* b,board* bTypes,int i,int j,int v,int ind,int gameMode);

void setCellAndUpdateErroneous(board* b,board* bTypes,int i,int j,int val,int gameMode,int redoInd);
/*set the cell in board b, mark erroneous cells in bTypes and update the changes in move*/
void setCellUpdateErroneousAndMove(board* b,board* bTypes,moveNode* move,int i,int j,int val,int gameMode,int printInd);
void setCellUpdateMove(board* b,moveNode* move,int i,int j,int val,int printInd);
int boardContainsFixedErroneousCells(board *b,board *bTypes);
void markAllErroneousCellsInBoard(board* b,board* bt,int gameMode);
void clearCell(board* pBoard, int i, int j);
#endif /* BOARD_H_ */
