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
	ERROR e;
	game* g;
	command* c;
	g= createGame();
	e = NO_ERROR;
	printWelcomeMessage();
	while(e!=EXIT_MESSAGE){
		askForCommand(g);
		c = createCommand();
		e=readCommand(c, g);
		if(e==NO_ERROR){
            isMoveCommand = commandIsAMove(c);
			e = executeCommand(c, g);
            if(e==NO_ERROR){
                if(commandMightHaveChangedBoard(c)){
                    printBoard(g->board,g->boardTypes,g->currMode,g->mark_errors);
                }
                if(!isMoveCommand){
                    destroyCommand(c);
                }
            }
            if(e!=NO_ERROR){ /*Error destroyed inside MoveCommand*/
                printErrorMessage(e, c, g->board->squareSideSize);
                /*if (!isMoveCommand) destroyCommand(c);*/
            }
		}
		else{
            printErrorMessage(e, c, g->board->squareSideSize);
            destroyCommand(c);
		}

		if(g->currMode==BOARD_SOLVED_CORRECTLY_MODE){
			printf("Congratulations! You solved the board correctly!\n");
			g->currMode = INIT_MODE;

		}

	}
	printf("Exiting...\n");
	/*all the commands outside the undoList are already destroyed so we only need to destroy the game*/
	destroyGame(g);

	return 1;


}
int main(){
    playSudoku();
	return 1;
}

