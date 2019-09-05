/*
 * game.c
 *
 *      Author: roee
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "solver.h"
#include "game.h"


ERROR executeSetCommand(game *game,moveNode *move, int x, int y, int z,int redoInd);

ERROR executeGenerateCommand(game *game,moveNode* move, int x, int y);

ERROR executeGuessCommand(game *game, double thresh);

/*n is the number of rows in each board block
 *m is the number of columns in each board block
 *the entire board contains mXn blocks- each contains nXm cells */


game* createGame(int seed){
	game* res = (game*) malloc(sizeof(game));
	if (res == NULL){
        printf("Error: malloc has failed\n");
        exit(-42);
	}
	res->seed = seed;
	res->board = createBoard(blockRows,blockColumns);
	res->boardSol = createBoard(blockRows,blockColumns);
	res->boardTypes = createBoard(blockRows,blockColumns);
	res->undoList = createMovesList();
	resetBoard(res->boardTypes, REGULAR_CELL);
	res->currMode = INIT_MODE;
	res->mark_errors=1;

	return res;
}

void destroyGame(game* game){
    destroyBoard(game->board);
    destroyBoard(game->boardSol);
    destroyBoard(game->boardTypes);
    printf("TMP boards were successfully destroyed\n");
    destroyMovesList(game->undoList);
    printf("TMP undoList was successfully destroyed\n");
	free(game);
}

int initializeGame(int seed, game **newGame) {
    /* function was changed to return int for result status instead of board*/
    game* res;
	int fixedCells= getInitializationInput();
	if (fixedCells == -1){
	    return 1; /*exitInd = 1 */
	}
	res = createGame(seed);
	buildBoardRandom(fixedCells,res);
	*newGame = res;
	return 0; /*exitInd = 0*/
}
int buildBoardRandom(int fixedCells,game* Pgame){
    findRandomSolution(Pgame->board, Pgame->boardSol);
	buildBoardFromSolution(Pgame,fixedCells);
	return 1;
}

void buildBoardFromSolution(game*Pgame,int fixedCells){
	int i, x, y, size = Pgame->board->squareSideSize;
	for (i=0; i < fixedCells; i++){
	    x = rand() % size;
        y = rand() % size;
        while (getCell(Pgame->boardTypes, y, x) ==1){
            x = rand() % size;
            y = rand() % size;
        }
        setCell(Pgame->board, y, x, getCell(Pgame->boardSol,y,x));
        setCell(Pgame->boardTypes, y, x, 1);
	}
}
ERROR executeSolveOrEditCommand(command* pCommand,game* pGame,ERROR error){
    board *newBoard=NULL,*newBoardTypes=NULL;
    int n=0,m=0,currMode;
    if(pCommand->name==SOLVE){/*the currentMode might not be updated (it could be init for example)
    but we need the set and erroneous checks to fit the new (after command) mode*/
    	currMode=SOLVE_MODE;
    }
    else{
    	currMode=EDIT_MODE;
    }
    error = loadGame(&newBoard,&newBoardTypes,pCommand->param1,&n,&m,currMode);
    if (error == NO_ERROR){
    	destroyBoard(pGame->board);
    	destroyBoard(pGame->boardTypes);
    	destroyBoard(pGame->boardSol);
    	destroyMovesList(pGame->undoList);
    	pGame->board = newBoard;
    	pGame->boardTypes = newBoardTypes;
    	pGame->boardSol = createBoard(n,m);
    	pGame->undoList = createMovesList();
        pGame->currMode = currMode;
        return error;
    }
    if(error==FCLOSE_ERROR){/*all the errors that occur after the new boards are created*/
    	destroyBoard(newBoard);
    	destroyBoard(newBoardTypes);
    }
    return error;

}
int executeNumSolutions(board* b){
	board* tmpCopy;
	int res;

	tmpCopy= createBoard(b->rows,b->columns);
	copyBoard(tmpCopy,b);
	res = exhaustiveBackTracingWithStack(b,tmpCopy);
	destroyBoard(tmpCopy);
	return res;


}
ERROR executeAutofill(game* g,moveNode* move,int redoInd){
	if(erroneousBoard(g->boardTypes)){
		return COMMAND_UNAVAILABLE_WITH_ERRONEOUS_BOARD;
	}
	return autofillBoard(g->board,g->boardTypes,move,redoInd,g->currMode,1);

}
void undoChange(game* g,changeNode* change){
	setCellAndUpdateErroneous(g->board,g->boardTypes,change->i,change->j,change->prevVal,g->currMode,STANDART_COMMAND_IND);
}
void undoChangesListStartingFrom(game* g,changeNode* start){
	if(start->next==NULL){
		undoChange(g,start);
		return;
	}
	undoChangesListStartingFrom(g,start->next);
	undoChange(g,start);
}
void undoChangesList(game* g,changesList* list){
	if(emptyChangesList(list)){
		return;
	}
	undoChangesListStartingFrom(g,list->first);
}
ERROR executeUndo(game* g,int printChanges){
	moveNode* currMove;
	if(emptyMovesList(g->undoList)||nodeIsStartSentinel(g->undoList,g->undoList->curr)){
		return NO_MOVES_TO_UNDO_ERROR;
	}

	currMove=g->undoList->curr;
	undoChangesList(g,currMove->changes);
	if(printChanges){
		printChangesList(currMove->changes);}
	demoteCurrPointer(g->undoList);
	return NO_ERROR;

}
ERROR executeReset(game* g){
	ERROR e = NO_ERROR;

	while(e!=NO_MOVES_TO_UNDO_ERROR){
		e=executeUndo(g,0);
	}
	return NO_ERROR;
}
ERROR executeRedo(game* g){
	moveNode* moveToRedo;
	ERROR error;
	if(emptyMovesList(g->undoList)||(g->undoList->curr->next==NULL&&!nodeIsStartSentinel(g->undoList,g->undoList->curr))){
		return NO_MOVES_TO_REDO_ERROR;
	}
	if(nodeIsStartSentinel(g->undoList,g->undoList->curr)){
		/*In this case we can image the curr pointer as if its pointing to and empty sentinel
		 * before undoList->first therefore we need to redo the undoList->first move*/
		moveToRedo=g->undoList->curr;
	}
	else{
		moveToRedo=g->undoList->curr->next;
	}
	error= executeCommand(moveToRedo->command,g,REDO_COMMAND_IND);
	promoteCurrPointer(g->undoList);
	return error;

}
ERROR executeCommand(command* pCommand, game* pGame,int redoInd){
    ERROR error;
    moveNode *move;
    int res;
    if(commandIsAMove(pCommand)&&redoInd==STANDART_COMMAND_IND){
    	move = createMoveNode(pCommand);
    }
    error = checkLegalParam(pCommand, pGame);

    error = NO_ERROR;  /*TODO: remove this after finishing checkLegalParam*/

    if (error != NO_ERROR)
        return error;

    /*After this point, command is assumed legal for this game state.*/
    switch(pCommand->name) {
        case SOLVE:
            error= executeSolveOrEditCommand(pCommand,pGame,error);
            break;
        case EDIT:
        	if(strcmp(pCommand->param1,"\0")!=0){/*strcmp returns 0 if the strings are identical*/
        		error= executeSolveOrEditCommand(pCommand,pGame,error);
        	}
        	else{
        		destroyGame(pGame);
        		pGame = createGame(1);/*seed=1 is arbitrary and temporary (no need for a seed)*/
        	}
            if (error == NO_ERROR)
                pGame->currMode = EDIT_MODE;
            break;
        case MARK_ERRORS:
            pGame->mark_errors = atoi(pCommand->param1);
            error = NO_ERROR;
            break;
        case PRINT_BOARD:
            printBoard(pGame->board, pGame->boardTypes,pGame->currMode,pGame->mark_errors);
            error = NO_ERROR;
            break;
        case GUESS:
            error = executeGuessCommand(pGame, atof(pCommand->param1));
            break;
        case GENERATE:
            error = executeGenerateCommand(pGame,move, atoi(pCommand->param1), atoi(pCommand->param2));
            break;
        case UNDO:
        	error = executeUndo(pGame,1);
            break;
        case REDO:
        	error = executeRedo(pGame);
            break;
        case SAVE:
            error = saveGame(pGame->board,pGame->boardTypes, pCommand->param1,pGame->currMode);
            break;
        case GUESS_HINT:
            /* error = guessHint(pGame, atoi(pCommand->param1), atoi(pCommand->param2)); TODO: uncomment this*/
            break;
        case NUM_SOLUTIONS:
        	if(erroneousBoard(pGame->boardTypes)){
        		error= COMMAND_UNAVAILABLE_WITH_ERRONEOUS_BOARD;
        	}
        	else{
				res = executeNumSolutions(pGame->board);
				printf("Number of solutions is:%d\n",res);
				error = NO_ERROR;}
            break;
        case AUTOFILL:
           error = executeAutofill(pGame,move,redoInd);
            break;
        case RESET:
            error = executeReset(pGame);
            break;
        case SET:
            error = executeSetCommand(pGame,move, atoi(pCommand->param1), atoi(pCommand->param2), atoi(pCommand->param3),redoInd);
            /*TODO: @Omer implement TODO: uncomment this
             * Comment from Roee: remember that you cannot set a fixed cell to the value 0*/
            break;
        case HINT:
            /*error = hint(pGame, atoi(pCommand->param1), atoi(pCommand->param2));TODO: uncomment this*/
            break;
        case VALIDATE:
            /*error = validateBoard(pGame);TODO: uncomment this*/
            break;
        case EXIT:
            error = EXIT_MESSAGE;
            break;
        case NONE:
            error = UNKNOWN_ERROR;
            break;
    }
    if(commandIsAMove(pCommand)&&error==NO_ERROR&&redoInd==STANDART_COMMAND_IND){
    	makeMoveTheLastInTheList(pGame->undoList,pGame->undoList->curr);
    	addMove(pGame->undoList,move);
    	promoteCurrPointer(pGame->undoList);
    }

    if (pGame->currMode == SOLVE_MODE && pGame->board->emptyCellsCounter == 0&& pCommand->name!=REDO){
    	/*when the command is REDO then the 'real' command to be redone has already entered this if block
    	 * in the recursive call to executeCommand*/
           if (erroneousBoard(pGame->boardTypes)){
           	printf("This solution is incorrect!\n");
           	printf("You can undo you last move to continue solving\n");
           }
           else{
        	   return BOARD_SOLVED_CORRECTLY;}
    }
    return error;
}

ERROR executeGuessCommand(game *game, double thresh) {
    ERROR error;
    if (erroneousBoard(game->boardTypes))
        return GUESS_ERRONEOUS_BOARD;
    error = solveLPWithThreshold(game->board, thresh);
    return error;
}

/**
 * Generates a board on a copy of current board, and then copies back all correct values to original board if possible.
 * Does NOT maintain fixedCell/regularCell notation (deemed unnecessary as this command is available in EDIT mode only)
 * @param game
 * @param x number of random cells to fill before solving
 * @param y number of cells to clear after solving
 * @return appropriate error
 */
ERROR executeGenerateCommand(game *game,moveNode* move, int x, int y) {
    board* origBoard, *cpBoard;
    int i, success, N;
    ERROR error;
    success=0;
    origBoard = game->board;
    N = origBoard->squareSideSize;
    if (origBoard->emptyCellsCounter < x)
        return GENERATE_NOT_ENOUGH_CELLS;
    cpBoard = createBoard(origBoard->rows, origBoard->columns);
    copyBoard(cpBoard, origBoard);
    for (i = 0; i < 1000; i++) {
        success = fillXRandomCells(cpBoard, x);
        if (success){
            error = solveILP(cpBoard);
            if (error == NO_ERROR)
                break;
            else
                copyBoard(cpBoard, origBoard);
        }
        else
            copyBoard(cpBoard, origBoard);
    }
    if (!success){
        destroyBoard(cpBoard);
        return FAILED_TO_GENERATE;
    }
    for (i = 0; i < (N*N)-y; i++) {
        clearRandomCell(cpBoard);
    }
    copyBoardAndUpdateMove(origBoard, cpBoard,move);
    destroyBoard(cpBoard);
    return NO_ERROR;
}

/**
 * Executes the set command
 * @param game
 * @param x
 * @param y
 * @param z
 * @return NO_ERROR if set sucessfully,FIXED_CELL_ERROR if trying to set fixed cell in SOLVE
 * BOARD_SOLVED_ERRONEOUS if board solved incorrectly, or BOARD_SOLVED_CORRECTLY if board solved correctly.
 */
ERROR executeSetCommand(game *game,moveNode *move, int x, int y, int z,int redoInd) {
    int type, currMode, i, j;
    currMode = game->currMode;
    i = y - 1;
    j = x - 1;
    type = getCell(game->boardTypes, i, j);
    if (currMode == SOLVE_MODE && type == FIXED_CELL){
        	return FIXED_CELL_ERROR;}
    if(redoInd==STANDART_COMMAND_IND){
    	setCellUpdateErroneousAndMove(game->board, game->boardTypes,move, i,j, z,game->currMode,0);
    }
    else{
    	setCellAndUpdateErroneous(game->board, game->boardTypes, i,j, z,game->currMode,redoInd);
    }

    return NO_ERROR;
}


/*ERROR loadBoard(game *game, char *path){} TODO: implement this*/

/*TODO: Remove this function*/

void executeCommandDEPRECATED(command* PcurrCommand, game* Pgame){
	/*Currently treats NONE command name as illegal command*/
	int validBoard;
	int j, i, z;
	int currval;
	j=i=z=1;/*= PcurrCommand->param1 - 1,i= PcurrCommand->param2 - 1,z=PcurrCommand->param3; j=column(i.e j) i = row(i.e. i)*/
	currval = getCell(Pgame->board, i, j);
	if ((PcurrCommand->name != EXIT && PcurrCommand->name != NONE && boardSolved(Pgame->board)) ||
	     PcurrCommand->name == NONE) {
	    printf("Error: invalid command\n");
	    return;
	}
	if(PcurrCommand->name==SET){
        if (currval < 0) /*TODO: Change to use some is_fixed_cell function*/
            printf("Error: cell is fixed\n");
        else if (z != 0 && !validAsignment(Pgame->board, z, i, j))
	            printf("Error: value is invalid\n"); /*TODO: This is allowed, need to confirm printing*/
        else{ /*No error, should set cell value*/
	        setCell(Pgame->board, i, j, z);
	        printBoard(Pgame->board,Pgame->boardTypes,Pgame->currMode,Pgame->mark_errors);
	        if (boardSolved(Pgame->board)){
	            printf("Puzzle solved successfully\n");
	        }

	    }
	}
	else if(PcurrCommand->name==HINT){
        printf("Hint: set cell to %d\n", abs(getCell(Pgame->boardSol, i, j)));
		}
	else if(PcurrCommand->name==VALIDATE){ /*TODO: change to using gurobi ILP. First check if board is errnoeus!*/
		validBoard=findDeterministicSolution(Pgame->board,Pgame->boardSol);
		if(validBoard==1){
			printf("Validation passed: board is solvable\n");
		}
		else{
			printf("Validation failed: board is unsolvable\n");
		}

			}
}
int commandMightHaveChangedBoard(command* c){
	commandName name = c->name;
	return name==EDIT||name==SOLVE||name==SET||name==AUTOFILL||name==UNDO||name==REDO||name==GENERATE||name==GUESS||name==RESET;
}





