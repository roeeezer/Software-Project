/*
 * game.h
 * contains all the relevant data to a certain sudoku game including
 * the high-level functions that manages the interaction between the user and the game
 *
 */

#ifndef GAME_H_
#define GAME_H_

#include "solver.h"
#include "command.h"
#include "board.h"
#define blockRows 3
#define blockColumns 3
typedef struct{
	/*each board contains numbers in range 0-9
	 * the regular board (and not the boardSol) might contain negative numbers s.t:
		 * if board[i][j] = -j then the value of the (i,j) cell is j and the
		 * cell is FIXED*/
	int seed;
	board* board;
	board* boardSol;
	board* boardTypes;
	/*0 - for regular cell
	* 1 - for fixed cell
	* 2 - for error cell*/

} game;
game* createGame(int seed);
void destroyGame(game* game);

/*Initializes a new game, getting user input for number of fixed cells,
 * then generates a board.
 * Returns the value for exitInd:
 * 1 if unable to initialize game (and now we have to quit)
 * 0 if game was initialized successfully */
int initializeGame(int seed, game **newGame);
/*Finds a random solution,
 * then fills the board with fixedCells fixed cells*/
int buildBoardRandom(int fixedCells,game* game);

/* Uses a solution board to randomly build a board
 * with fixedCells fixed cells */
void buildBoardFromSolution(game* Pgame,int fixedCells);

/**
 *
 * @param PcurrCommand the pointer to the command
 * @param Pgame the relevent game
 * executes the command according to assignment details
 * uses commandName field to determine which command to execute
 * if the command is exit or restart, the main function handles the
 * flow of the game
 */
void executeCommand(command* PcurrCommand,game* Pgame);

#endif /* GAME_H_ */
