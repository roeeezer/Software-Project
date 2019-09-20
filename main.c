/*
 * main.c
 *
 *    Author: roee
 *    For each function returning int: 0 - failed call, 1- successful call
 */
#include <stdio.h>
#include "parser.h"
#include "game.h"
#include "command.h"
#include "board.h"
#include "error.h"


int playSudoku(){
    int isMoveCommand;
	ERROR error;
	game* pGame;
	command* currCommand;
    pGame= createGame();
    error = NO_ERROR;
	printWelcomeMessage();
	while(error != EXIT_MESSAGE){
		askForCommand(pGame);
        currCommand = createCommand();
        error=readCommand(currCommand, pGame);
		if(error == NO_ERROR){
            isMoveCommand = commandIsAMove(currCommand);
            error = executeCommand(currCommand, pGame);
            if(error == NO_ERROR){
                if(commandMightHaveChangedBoard(currCommand)){
                    printBoard(pGame->board, pGame->boardTypes, pGame->currMode, pGame->mark_errors);
                }
                if(!isMoveCommand){
                    destroyCommand(currCommand);
                }
            }
            if(error != NO_ERROR){ /*Error destroyed inside executeCommand*/
                printErrorMessage(error, currCommand, pGame->board->squareSideSize);

            }
		}
		else{
            printErrorMessage(error, currCommand, pGame->board->squareSideSize);
            destroyCommand(currCommand);
		}

		if(pGame->currMode == BOARD_SOLVED_CORRECTLY_MODE){
			printf("Congratulations! You solved the board correctly!\n");
            pGame->currMode = INIT_MODE;

		}

	}
	printf("Exiting...\n");
	/*all the commands outside the undoList are already destroyed so we only need to destroy the game*/
	destroyGame(pGame);

	return 1;


}
int main(){
    playSudoku();
	return 1;
}

