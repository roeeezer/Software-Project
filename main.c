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
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
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
	/*printf("Before build random\n");
	buildBoardRandom(10,Pgame);*/
	resetBoard(Pgame->board, REGULAR_CELL);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
	copyBoard(Pgame->boardSol,Pgame->board);
	printf("Before save\n");
	saveGame(Pgame->board,Pgame->boardTypes,"24Board",2);

}

int finalMain(){
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
			e = executeCommand(c, g,STANDART_COMMAND_IND);
		}
		if(e==NO_ERROR){
			if(commandMightHaveChangedBoard(c)){
				printBoard(g->board,g->boardTypes,g->currMode,g->mark_errors);
				if(!commandIsAMove(c)){
						destroyCommand(c);
						}
			}
		}
		if(e!=NO_ERROR){
			printErrorMessage( e, c);
			destroyCommand(c);
		}
		if(g->currMode==BOARD_SOLVED_CORRECTLY_MODE){
			printf("Congratulations! You solved the board correctly!\n");
			g->currMode = INIT_MODE;

		}

	}
	printf("Exit...\n");
	/*all the commands outside the undoList are already destroyed so we only need to destroy the game*/
	destroyGame(g);

	return 1;


}
int oldFinalMain(){
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
			printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
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


	printBoard(b,bt,Pgame->currMode,Pgame->mark_errors);
	saveGame(b,bt,"board1.txt",SOLVE);
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
	printf("before load b val=%d\n",b==NULL);

	loadGame(&b,&bt,"board1.txt",&n,&m,Pgame->currMode);
	printf("after load b val=%d\n",b==NULL);

	printBoard(b,bt,Pgame->currMode,Pgame->mark_errors);


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
	c->param1 = "board2.txt";
	err=executeCommand(c,Pgame,STANDART_COMMAND_IND);
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);

	if(err==NO_ERROR){
		printf("mode:%d\n",Pgame->currMode);
		printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);}
	else{
		printErrorMessage(err,c);
	}

}
void erroneousCheckTester(){
	game* Pgame;
	int i=0,j=2,v=1;
	Pgame=createGame(5);
	Pgame->currMode = EDIT_MODE;
	buildBoardRandom(45,Pgame);
	resetBoard(Pgame->boardTypes, FIXED_CELL);
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
	setCellAndUpdateErroneous(Pgame->board,Pgame->boardTypes, i, j, v,Pgame->currMode,STANDART_COMMAND_IND);
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);

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

	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
	executeCommand(c,Pgame,STANDART_COMMAND_IND);
	c->name = AUTOFILL;
	executeCommand(c,Pgame,STANDART_COMMAND_IND);
	printErrorMessage( err, c);
	if(err==NO_ERROR){
		printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
	}


}
void executeCommandAndPrintData(game* g,command* c){
	ERROR err=NO_ERROR;
	printf("Execute ");
	printCommandName(c);printf("\n");
	err=executeCommand(c,g,STANDART_COMMAND_IND);
	printErrorMessage( err, c);
	printBoard(g->board,g->boardTypes,g->currMode,g->mark_errors);
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
	Pgame->currMode = EDIT_MODE;
	/*
	buildBoardRandom(55,Pgame);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
*/

	c = createCommand();
	c->name = SOLVE;
	c->param1 = "board1.txt";
	executeCommandAndPrintData(Pgame,c);
	printf("erroneous=%d\n",setCausesErroneousCell(Pgame->board,Pgame->boardTypes,0,4,8,1,Pgame->currMode));
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
/*
	c = createCommand();
	c->name = AUTOFILL;
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = SET;
	c->param1 = "1";c->param2 = "1";c->param3 = "1";
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = SET;
	c->param1 = "1";c->param2 = "2";c->param3 = "3";
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = UNDO;
	executeCommandAndPrintData(Pgame,c);

	c = createCommand();
	c->name = SET;
	c->param1 = "1";c->param2 = "2";c->param3 = "5";
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
*/


	destroyGame(Pgame);
}
void destroyTester(){
	game* Pgame;
	Pgame=createGame(5);
	Pgame->currMode = SOLVE_MODE;
	buildBoardRandom(55,Pgame);
	resetBoard(Pgame->boardTypes, REGULAR_CELL);
	printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
	destroyGame(Pgame);
}

int main(){
	SP_BUFF_SET();
	finalMain();
	return 1;
}

