/*
 * board.h
 *
 *      Author: roee
 */

#ifndef BOARD_H_
#define BOARD_H_
#include <stdio.h>
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
#define REGULAR 0
#define FIXED 1
#define ERRONEOUS 2
typedef struct{
	int* boardArr;
	int rows;
	int columns;
	/*meaning blockRows and blockColumns
	 * n = rows, m = =columns*/
	int squareSideSize;


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
int printBoard(board* b,board* bTypes);
/*prints row of blocks number blockRow in the board
 * @pre: 0<=blockRow<#rowsOfBlocks=board.columns*/
void printRowOfBlocks(board* b,int blockRow,board* bTypes);
/*prints cell row number rowInBlocksRow in the row of blocks number blocksRow
 * @pre: 0<=blockRow<#rowsOfBlocks=board.columns
 * @pre: 0<=rowInBlocksRow<#(number of cell rows in one block)=board.rows*/
void printOneRowInRowsOfBlocks(board* b,int blocksRow,int rowInBlocksRow,board* bTypes);
/*
 * @pre: 0<=blockRow<#rowsOfBlocks=board.columns
 * @pre: 0<=rowInBlocksRow<#(number of cell rows in one block)=board.rows
 * @pre: 0<=blockComlumnInSingleRow<#(columns of blocks in board)=b.rows*/
void printSingleRowInSingleBlock(board* b,int blocksRow,int rowInBlocksRow,int blockComlumnInSingleRow,board* bTypes);
/*@ret == 1 if and only if all cells in board are filled.*/
int boardSolved(board * pBoard);
/*prints a single cell according to the described format.
 * negative values represent fixed cell and zero values represent empty cells*/
void printCellValue(board* b,int i,int j,board* bTypes);
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
int boardHasASolution(board* b);
/*$ret==1 iff the bTypes contains and erroneous cell*/
int erroneousBoard(board* bTypes);
int setCausesErroneousCell(board* b,board* bTypes,int i,int j,int v,int markErroneousCells);
void setCellAndMarkErroneous(board* b,board* bTypes,int i,int j,int val);
int boardContainsFixedErroneousCells(board *b,board *bTypes);
#endif /* BOARD_H_ */
