/*
 * main.c
 *
 *    Author: roee
 *    For each function returning int: 0 - failed call, 1- successful call
 */
#include "SPBufferset.h"
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "game.h"
#include "solver.h"
#include "command.h"
#include "board.h"
#include "error.h"
#include "gurobi_c.h"
#include "files.h"
#include <time.h>

void loaderTester(){
	loadBoard(NULL,NULL,"board1.txt");
}
/*void exhaustiveBackTrackingTester(){
	clock_t start, end;
	double cpu_time_used;
	game* Pgame=createGame(2);
	resetBoard(Pgame->board,0);
	resetBoard(Pgame->boardSol,0);
	resetBoard(Pgame->boardTypes,0);
	buildBoardRandom(35,Pgame);
	printBoard(Pgame->board,Pgame->boardTypes);
	copyBoard(Pgame->boardSol,Pgame->board);

     start = clock();
     printf("old func:%d\n",exhaustiveBackTracingRec(Pgame->board,Pgame->boardSol,0));
     end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("time:%f\n",cpu_time_used);
     start = clock();
     printf("new func:%d\n",exhaustiveBackTracingWithStack(Pgame->board,Pgame->boardSol));
     end = clock();
     cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
     printf("time:%f\n",cpu_time_used);

}*/
void saveGameTester(){
	game* Pgame=createGame(1);
	resetBoard(Pgame->board,0);
	resetBoard(Pgame->boardSol,0);
	buildBoardRandom(35,Pgame);
	resetBoard(Pgame->boardTypes,REGULAR);
	printBoard(Pgame->board,Pgame->boardTypes);
	copyBoard(Pgame->boardSol,Pgame->board);
	saveGame(Pgame->board,Pgame->boardTypes,"board.txt",2);

}




/*
int finalMain(int argc, char *argv[]){
	int exitInd,restartInd,seed;

	 * ERROR commandError;*
	game* Pgame; *need to use createGame?*
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
			*check malloc success*
            *Roee for compilation
             * commandError = readCommand(PcurrCommand, Pgame);

			exitInd=(PcurrCommand->name == EXIT);
			restartInd=(PcurrCommand->name==RESTART);

            executeCommandDEPRECATED(PcurrCommand, Pgame);
			destroyCommand(PcurrCommand);*
		}
		destroyGame(Pgame);
	}
	printf("Exiting...\n");
	return 0;
}
*/
int main(){
	loaderTester();
	return 1;
}

