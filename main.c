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


void testReadCommand(){
    game* pGame;
    command* pCommand;
    ERROR error;
    pGame = createGame(5);
    pCommand = createCommand();
    error = readCommand(pCommand, pGame);
    printf("DONE READING \n");
    printErrorMessage(error, pCommand);
    printf("Setting game mode to solve and trying again\n");
    pGame->currMode = SOLVE_MODE;
    destroyCommand(pCommand);
    pCommand = createCommand();
    error = readCommand(pCommand, pGame);
    printf("DONE READING \n");
    printErrorMessage(error, pCommand);
}

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
	printf("Before build random\n");
	buildBoardRandom(60,Pgame);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);
	printBoard(Pgame->board,Pgame->boardTypes);
	copyBoard(Pgame->boardSol,Pgame->board);
	printf("Before save\n");
	saveGame(Pgame->board,Pgame->boardTypes,"bigBoard.txt",2);

}

int newFinalMain(){
	int seed=1;
	ERROR e;
	game* g;
	command* c;
	g= createGame(seed);
	e = NO_ERROR;
	printWelcomeMessage();
	while(e!=EXIT_MESSAGE){
		askForCommand(g);
		c = createCommand();
		e=readCommand(c, g);
		if(e==NO_ERROR){
			e = executeCommand(c, g,1);
		}
		if(e==NO_ERROR){
			if(commandMightHaveChangedBoard(c)){
				printBoard(g->board,g->boardTypes);
				/*printMovesList(g->undoList);*/
				if(!commandIsAMove(c)){
						destroyCommand(c);
						}
			}
		}
		if(e!=NO_ERROR){
			printErrorMessage( e, c);
			destroyCommand(c);
		}

	}
	printf("Exit...");
	destroyGame(g);
	destroyCommand(c);
	return 1;


}
int finalMain(){
	int exitInd,restartInd,seed;
	command* PcurrCommand;
	ERROR commandError;
	 /* ERROR commandError;*/
	game* Pgame; /*need to use createGame?*/
	seed=1;
    srand(seed);
	exitInd=0;
	while(!exitInd){
	    restartInd = 0;
		exitInd = initializeGame(seed, &Pgame);
		if(exitInd){
            printf("Exiting...\n");
            return 0;}
			printBoard(Pgame->board,Pgame->boardTypes);
		while(!exitInd&&!restartInd){
			PcurrCommand=createCommand();
			/*check malloc success*/
             commandError = readCommand(PcurrCommand, Pgame);
			exitInd=(PcurrCommand->name == EXIT);
			/*restartInd=(PcurrCommand->name==RESTART);*/
            executeCommandDEPRECATED(PcurrCommand, Pgame);
			destroyCommand(PcurrCommand);
			/*TODO: check if executionError (returns from executeCommand) is BOARD_SOLVED_CORRECTLY and act*/
		}
		destroyGame(Pgame);}
	printf("Exiting...\n");
	/*Roee: just for compilation:*/
	printf("%d",commandError);
	return 0;
}

void loaderTester(){
	int n=0,m=0;
	board *b,*bt;
	game* Pgame=createGame(5);
	b = Pgame->board;
	bt = Pgame->boardTypes;
	buildBoardRandom(45,Pgame);
	resetBoard(Pgame->boardSol,0);
	resetBoard(bt,REGULAR_CELL);


	printBoard(b,bt);
	saveGame(b,bt,"board1.txt",SOLVE);
	printBoard(Pgame->board,Pgame->boardTypes);
	printf("before load b val=%d\n",b==NULL);

	loadGame(&b,&bt,"board1.txt",&n,&m);
	printf("after load b val=%d\n",b==NULL);

	printBoard(b,bt);


}
void executeCommandTester(){
	game* Pgame;
	command *c;
	ERROR err;
	c = createCommand();
	Pgame=createGame(1);
	buildBoardRandom(28,Pgame);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);


	c->name=SOLVE;
	c->param1 = "board1.txt";
	err=executeCommand(c,Pgame,UPDATE_MOVES_LIST_IND);
	printBoard(Pgame->board,Pgame->boardTypes);

	if(err==NO_ERROR){
		printf("mode:%d\n",Pgame->currMode);
		printBoard(Pgame->board,Pgame->boardTypes);}
	else{
		printErrorMessage(err,c);
	}

}
void erroneousCheckTester(){
	game* Pgame;
	int i=0,j=2,v=1;
	Pgame=createGame(5);
	buildBoardRandom(45,Pgame);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);
	printBoard(Pgame->board,Pgame->boardTypes);
	setCellAndUpdateErroneous(Pgame->board,Pgame->boardTypes, i, j, v);
	printBoard(Pgame->board,Pgame->boardTypes);
	v=9;
	setCellAndUpdateErroneous(Pgame->board,Pgame->boardTypes, i, j, v);
	printBoard(Pgame->board,Pgame->boardTypes);
}
void autofillTester(){
	game* Pgame;
	command *c;
	ERROR err=NO_ERROR;
	c = createCommand();

	Pgame=createGame(5);
	c->name = SAVE;
	c->param1 = "tmpBoard.txt";
	buildBoardRandom(55,Pgame);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);

	printBoard(Pgame->board,Pgame->boardTypes);
	executeCommand(c,Pgame,UPDATE_MOVES_LIST_IND);
	c->name = AUTOFILL;
	executeCommand(c,Pgame,UPDATE_MOVES_LIST_IND);
	printErrorMessage( err, c);
	if(err==NO_ERROR){
		printBoard(Pgame->board,Pgame->boardTypes);
	}


}
void executeCommandAndPrintData(game* g,command* c){
	ERROR err=NO_ERROR;
	printf("Execute ");
	printCommandName(c);printf("\n");
	err=executeCommand(c,g,UPDATE_MOVES_LIST_IND);
	printErrorMessage( err, c);
	printBoard(g->board,g->boardTypes);
	printf("movesList after execution: ");
	printMovesList(g->undoList);
	printf("\n\n");

	if(!commandIsAMove(c)){
		destroyCommand(c);
	}


}
void undoListTester(){
	game* Pgame;
	command *c;
	Pgame=createGame(5);
	Pgame->currMode = SOLVE_MODE;
	buildBoardRandom(55,Pgame);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);
	printBoard(Pgame->board,Pgame->boardTypes);

	c = createCommand();
	c->name = SET;
	c->param1 = "1";c->param2 = "1";c->param3 = "5";
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = SET;
	c->param1 = "1";c->param2 = "1";c->param3 = "6";
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = AUTOFILL;
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = NUM_SOLUTIONS;
	executeCommandAndPrintData(Pgame,c);


	c = createCommand();
	c->name = SOLVE;
	c->param1 = "board1.txt";
	executeCommandAndPrintData(Pgame,c);



	c = createCommand();
	c->name = UNDO;
	executeCommandAndPrintData(Pgame,c);


	c = createCommand();
	c->name = UNDO;
	executeCommandAndPrintData(Pgame,c);


	c = createCommand();
	c->name = UNDO;
	executeCommandAndPrintData(Pgame,c);


	c = createCommand();
	c->name = UNDO;
	executeCommandAndPrintData(Pgame,c);


	c = createCommand();
	c->name = REDO;
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = SET;
	c->param1 = "1";c->param2 = "4";c->param3 = "6";
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = SET;
	c->param1 = "1";c->param2 = "1";c->param3 = "0";
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = AUTOFILL;
	executeCommandAndPrintData(Pgame,c);

	destroyGame(Pgame);
}
void destroyTester(){
	game* Pgame;
	Pgame=createGame(5);
	Pgame->currMode = SOLVE_MODE;
	buildBoardRandom(55,Pgame);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);
	printBoard(Pgame->board,Pgame->boardTypes);
	destroyGame(Pgame);
}

int main(){
	SP_BUFF_SET();
	newFinalMain();
	return 1;
}

