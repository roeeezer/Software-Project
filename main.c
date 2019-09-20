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
#include "files.h"
#include <time.h>

void setUpLoadedTest(board **pBoard, board **btypes);

void testGuessHintCommand(int x, int y){
    board * pBoard, *bTypes;
    setUpLoadedTest(&pBoard, &bTypes);
    printf("the cell at <%d,%d> is %d \n", x, y, getCell(pBoard, y-1, x-1));
    printBoard(pBoard, bTypes, SOLVE_MODE, 1);
    executeGuessHintCommand(pBoard, bTypes, x, y);
}

void testHintCommand(int x, int y){
    board * pBoard, *bTypes;
    setUpLoadedTest(&pBoard, &bTypes);
    printf("the cell at <%d,%d> is %d \n", x, y, getCell(pBoard, y-1, x-1));
    printBoard(pBoard, bTypes, SOLVE_MODE, 1);
    executeHintCommand(pBoard, bTypes, x, y);
    solveILP(pBoard);
    printBoard(pBoard, bTypes, SOLVE_MODE, 1);
}

void testValidateCommand(){
    board * pBoard, *bTypes;
    setUpLoadedTest(&pBoard, &bTypes);
    printBoard(pBoard, bTypes, SOLVE_MODE, 1);
    executeValidateCommand(pBoard);
}

void testSimpleAutofill(){
    board *pBoard, *btypes;
    ERROR error;
    setUpLoadedTest(&pBoard, &btypes);
    printf("Before:\n");
    printBoard(pBoard, btypes, SOLVE_MODE, 1);
    error = simpleAutofill(pBoard);
    if (error != NO_ERROR)
        printf("MALLOC ERROR in simpleAutofill?\n");
    printf("After:\n");
    printBoard(pBoard, btypes, SOLVE_MODE, 1);
}

int testLPGeneratesSolvableBoard(double thresh, int printAfterRun) {
    board *pBoard, *btypes;
    ERROR error;
    clock_t begin, end;
    int res;
    setUpLoadedTest(&pBoard, &btypes);
    printf("Before:\n");
    if (printAfterRun)printBoard(pBoard, btypes, SOLVE_MODE, 1);
    begin = clock();
    error = solveLPWithThreshold(pBoard, thresh);
    end = clock();
    printf("Total time for LP: %f \n", (end - begin) / (double)(CLOCKS_PER_SEC));
    if (error != NO_ERROR){
        printf("LP error!");
        return -100;
    }
    printf("After LP :\n");
    if (printAfterRun) printBoard(pBoard,btypes, SOLVE_MODE, 1);
    printf("Now checking if board is solvable with ILP:\n");
    error = solveILP(pBoard);
    if (error != NO_ERROR){
        printf("Unable to solve board with ILP!\n");
        res = 0;
    }
    else{
        printf("Succesfully solved board with ILP\nAfter:\n");
        if (printAfterRun) printBoard(pBoard, btypes, SOLVE_MODE, 1);
        res=1;
    }
    destroyBoard(pBoard);
    destroyBoard(btypes);
    return res;
}

void testLPWithThreshold(double thresh){
    board *pBoard, *btypes;
    ERROR error;
    clock_t begin, end;
    setUpLoadedTest(&pBoard, &btypes);
    printf("Before:\n");
    printBoard(pBoard, btypes, SOLVE_MODE, 1);
    begin = clock();
    error = solveLPWithThreshold(pBoard, thresh);
    end = clock();
    printf("Total time for LP: %f \n", (end - begin) / (double)(CLOCKS_PER_SEC));
    if (error != NO_ERROR){
        printf("LP error!");
        return;
    }
    printf("After:\n");
    printBoard(pBoard,btypes, SOLVE_MODE, 1);
    destroyBoard(pBoard);
    destroyBoard(btypes);

}

void setUpLoadedTest(board **pBoard, board **btypes) {
    int n, m;
    n=m=0;
    printf("Now loading\n");
    loadGame(pBoard, btypes, "./board1", &n, &m, SOLVE_MODE);
}

void testGenerate(){
    command* comm;
    game *pGame = createGame(1);
    pGame->currMode = EDIT_MODE;
    printBoard(pGame->board, pGame->boardTypes, SOLVE_MODE, 1);
    printf("Done initing\n");
    comm = createCommand();
    readCommand(comm, pGame);
    executeCommand(comm, pGame);
    printf("done executing\n");
    printBoard(pGame->board, pGame->boardTypes, SOLVE_MODE, 1);
    /*destroyCommand(comm);*/
    destroyGame(pGame);
}

void testILP(){
    board *pBoard, *btypes;
    ERROR error;
    int n, m;
    clock_t begin, end;
    n=m=0;
    printf("Now loading\n");
    loadGame(&pBoard, &btypes, "./board1", &n, &m, SOLVE_MODE);
    printf("Before:\n");
    printBoard(pBoard,btypes, SOLVE_MODE, 1);
    begin = clock();
    error = solveILP(pBoard);
    end = clock();
    printf("Total time for ILP: %f \n", (end - begin) / (double)(CLOCKS_PER_SEC));
    if (error == NO_ERROR){
        printf("After:\n");
        printBoard(pBoard,btypes, SOLVE_MODE, 1);
    }
    else
        printf("ERROR!\n");
    destroyBoard(pBoard);
    destroyBoard(btypes);
}

void testReadCommand(){
    game* pGame;
    command* pCommand;
    ERROR error;
    pGame = createGame(5);
    pCommand = createCommand();
    error = readCommand(pCommand, pGame);
    printf("param 1: %s\n param 2: %s, param 3: %s\n",pCommand->param1, pCommand->param2, pCommand->param3);
    printf("DONE READING \n");
    printErrorMessage(error, pCommand, 0);
    printf("Setting game mode to solve and trying again\n");
    pGame->currMode = SOLVE_MODE;
    destroyCommand(pCommand);
    pCommand = createCommand();
    error = readCommand(pCommand, pGame);
    printf("DONE READING \n");
    printErrorMessage(error, pCommand, 0);
}



int finalMain(){
	int seed=1; /*TODO need seed?*/
    int isMoveCommand;
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
            if(e!=NO_ERROR){
                printErrorMessage(e, c, g->board->squareSideSize);
                if (!isMoveCommand) destroyCommand(c);
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

void executeCommandAndPrintData(game* g,command* c){
	ERROR err=NO_ERROR;
	printf("Execute ");
	printCommandName(c);printf("\n");
	err=executeCommand(c,g);
    printErrorMessage(err, c, 0);
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

void numSolutionsTester(){
	game* g;
	command *c;
	g=createGame(5);
	g->currMode = EDIT_MODE;
	c = createCommand();
	c->name = SOLVE;
	c->param1 = "filledBoard";
	executeCommand(c,g);
	c->name = NUM_SOLUTIONS;
	executeCommand(c,g);

}
int main(){
	/*int x, y;
    printf("Enter x: ");
    scanf("%d", &x);
    printf("Enter y: ");
    scanf("%d", &y);
    testHintCommand(x, y);
    return 1;
    int choice, successCounter, numOfRuns, i;
    double thresh;
    successCounter = 0;
    printf("Enter threshold as double \n");
    scanf("%lf", &thresh);
    printf("Enter number of runs\n");
    scanf("%d", &numOfRuns);
    for (i = 0; i < numOfRuns; i++) {
        successCounter += testLPGeneratesSolvableBoard(thresh, 0);
    }
    printf("Total successes: %d out of %d runs\nSuccess rate of %f with threshold %f\n", successCounter,
           numOfRuns, (double) successCounter / numOfRuns, thresh);
    return 1;
    printf("Now testing ILP\n");
    testILP();
    return 1;
    printf("enter input: 1 - testGenerate\t2 - testILP\t3 - testReadCommand\n");
    scanf("%d", &choice);
    if (choice == 1)
        testGenerate();
    else if (choice == 2)
        testILP();
    else if (choice == 3)
        testReadCommand();
    return 1;*/

	SP_BUFF_SET();
	finalMain();
	return 1;
}

