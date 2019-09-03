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
#include "error.h"
#include "files.h"
#include "movesList.h"
#define blockRows 3
#define blockColumns 3



typedef struct{
	int seed;
	board* board;
	board* boardSol;
	board* boardTypes;
	/*0 - for regular cell
	* 1 - for fixed cell
	* 2 - for erroneous cell
	* In EDIT_MODE there are only 2 categories: 2 - for erroneous 1\0 - for not erroneous since
	* in EDIT_MODE all cells are considered fixed*/
	movesList* undoList;
	/*while using the undoList we must be careful with our commands object:
	 *NEVER override a command point object
	 *NEVER destroy a move command unless it's through destroyMovesList */
	int currMode;
	int mark_errors;


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
 * @param PcurrCommand the pointer to the command
 * @param Pgame the relevent game
 * @param ind whether or not we want to update the undolist during the execution
 * executes the command according to assignment details
 * uses commandName field to determine which command to execute
 * if the command is exit or restart, the main function handles the
 * flow of the game
 * ind==1 iff we wish to update the undolist. in every call ind should always be 1 except for a command
 * that is executed as part of a redo command where ind should be 0
 */
ERROR executeCommand(command* pCommand, game* pGame,int ind);
void executeCommandDEPRECATED(command* PcurrCommand, game* Pgame);
int commandMightHaveChangedBoard(command* c);


#endif /* GAME_H_ */
