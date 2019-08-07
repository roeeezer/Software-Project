/*
 * main.c
 *
 *    Author: roee
 *    For each function returning int: 0 - failed call, 1- successful call
 */
#include "SPBufferset.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "parser.h"
#include "game.h"
#include "solver.h"
#include "command.h"
#include "board.h"

int main(int argc, char *argv[]){
	int exitInd,restartInd,seed;
	game* Pgame; /*need to use createGame?*/
	seed = argc;
	seed = atoi(argv[1]);
    srand(seed);
	exitInd=0;

	while(!exitInd){
	    restartInd = 0;
		exitInd = initializeGame(seed, &Pgame);
		if(exitInd){
            printf("Exiting...\n");
            return 0;
		}
			printBoard(Pgame->board,Pgame->boardTypes);
		while(!exitInd&&!restartInd){

			command* PcurrCommand=createCommand();
			/*check malloc success*/
            readCommand(PcurrCommand);

			exitInd=(PcurrCommand->name==EX);
			restartInd=(PcurrCommand->name==RESTART);

			executeCommand(PcurrCommand,Pgame);
			destroyCommand(PcurrCommand);
		}
		destroyGame(Pgame);
	}
	printf("Exiting...\n");
	return 0;
}


