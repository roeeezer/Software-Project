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
	board* board;
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
game* createGame();
void destroyGame(game* game);


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
ERROR executeCommand(command* pCommand, game* pGame);
int commandMightHaveChangedBoard(command* c);


#endif /* GAME_H_ */
