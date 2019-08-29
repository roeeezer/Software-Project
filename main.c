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


void exhaustiveBackTrackingTester(){
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

}
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

int finalMain(int argc, char *argv[]){
	int exitInd,restartInd,seed;
	command* PcurrCommand;
	ERROR commandError;

	 /* ERROR commandError;*/
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

			PcurrCommand=createCommand();
			/*check malloc success*/

             commandError = readCommand(PcurrCommand, Pgame);

			exitInd=(PcurrCommand->name == EXIT);
			/*restartInd=(PcurrCommand->name==RESTART);*/
            executeCommandDEPRECATED(PcurrCommand, Pgame);
			destroyCommand(PcurrCommand);
		}
		destroyGame(Pgame);
	}
	printf("Exiting...\n");
	/*Roee: just for compilation:*/
	printf("%d",commandError);
	return 0;
}
void loaderTester(){
	int n=0,m=0;
	board *b,*bt;
	game* Pgame=createGame(5);
	/*b = Pgame->board;
	bt = Pgame->boardTypes;*/
	buildBoardRandom(45,Pgame);
	/*resetBoard(Pgame->boardSol,0);
	resetBoard(bt,REGULAR);


	printBoard(b,bt);
	saveGame(b,bt,"board1.txt",SOLVE);*/
	printBoard(Pgame->board,Pgame->boardTypes);
	printf("before load b val=%d\n",b);

	loadGame(&b,&bt,"board1.txt",&n,&m);
	printf("after load b val=%d\n",b);

	printBoard(b,bt);


}
void executeCommandTester(){
	game* Pgame;
	command *c;
	ERROR err;
	c = createCommand();
	Pgame=createGame(700);
	buildBoardRandom(28,Pgame);
	resetBoard(Pgame->boardTypes,REGULAR);


	Pgame->currMode=SOLVE_MODE;
	c->name=SAVE;
	c->param1 = "board2.txt";
	err=executeCommand(c,Pgame);
	printBoard(Pgame->board,Pgame->boardTypes);

	c->name = SOLVE;
	c->param1 = "invalidBoard.txt";

	err=executeCommand(c,Pgame);
	if(err==NO_ERROR){
		printBoard(Pgame->board,Pgame->boardTypes);}

}
void erroneousCheckTester(){
	game* Pgame;
	int i=4,j=2,v=2;
	Pgame=createGame(5);
	buildBoardRandom(45,Pgame);
	resetBoard(Pgame->boardTypes,FIXED);
	printBoard(Pgame->board,Pgame->boardTypes);
	printf("i=%d,j=%d,val=%d,erroneous=%d\n",i,j,v,setCausesErroneousCell(Pgame->board,Pgame->boardTypes,i,j,v,2));
	printBoard(Pgame->board,Pgame->boardTypes);
}
void autofillTester(){
	game* Pgame;
	command *c;
	ERROR err=NO_ERROR;
	c = createCommand();
	c->name = AUTOFILL;
	Pgame=createGame(5);

	buildBoardRandom(55,Pgame);
	resetBoard(Pgame->boardTypes,REGULAR);

	printBoard(Pgame->board,Pgame->boardTypes);
	printf("emptyCellsCounter=%d\n",Pgame->board->emptyCellsCounter);
	executeCommand(c,Pgame);
	printErrorMessage( err, c);
	if(err==NO_ERROR){
		printBoard(Pgame->board,Pgame->boardTypes);
		printf("emptyCellsCounter=%d\n",Pgame->board->emptyCellsCounter);
	}


}
int main(){
	autofillTester();
	return 1;
}

