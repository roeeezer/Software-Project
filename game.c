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


ERROR executeSetCommand(game *game,moveNode *move, int x, int y, int z);

ERROR executeGenerateCommand(game *game,moveNode* move, int x, int y);

ERROR executeGuessCommand(game *game, double thresh, moveNode *move);

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
    destroyMovesList(game->undoList);
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
    if(error==FCLOSE_ERROR||error==INVALID_FILE_FORMAT_BOARDS_WERE_CREATED||error==FIXED_ERRONEOUS_CELLS_IN_FILE){/*all the errors that occur after the new boards are created*/
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
ERROR executeAutofill(game* g,moveNode* move){
	if(erroneousBoard(g->boardTypes)){
		return COMMAND_UNAVAILABLE_WITH_ERRONEOUS_BOARD;
	}
	return autofillBoard(g->board,g->boardTypes,move,g->currMode,1);

}
void undoOrRedoChange(game* g,changeNode* change,int ind){
	if(ind==UNDO_CHANGE_IND){
		setCellAndUpdateErroneous(g->board,g->boardTypes,change->i,change->j,change->prevVal,g->currMode);
	}
	if(ind==REDO_CHANGE_IND){
			setCellAndUpdateErroneous(g->board,g->boardTypes,change->i,change->j,change->newVal,g->currMode);
		}
}
void undoOrRedoChangesListStartingFrom(game* g,changeNode* start,int ind){
	if(start->next==NULL){
		undoOrRedoChange(g,start,ind);
		return;
	}
	undoOrRedoChangesListStartingFrom(g,start->next,ind);
	undoOrRedoChange(g,start,ind);
}
void undoOrRedoChangesList(game* g,changesList* list,int ind){
	if(emptyChangesList(list)){
		return;
	}
	undoOrRedoChangesListStartingFrom(g,list->first,ind);
}
ERROR executeUndo(game* g){
	moveNode* currMove;
	if(emptyMovesList(g->undoList)||nodeIsStartSentinel(g->undoList,g->undoList->curr)){
		return NO_MOVES_TO_UNDO_ERROR;
	}

	currMove=g->undoList->curr;
	undoOrRedoChangesList(g,currMove->changes,UNDO_CHANGE_IND);

	printChangesList(currMove->changes,UNDO_CHANGE_IND);
	demoteCurrPointer(g->undoList);
	return NO_ERROR;

}
ERROR executeRedo(game* g){
	moveNode* moveToRedo;
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
	undoOrRedoChangesList(g,moveToRedo->changes,REDO_CHANGE_IND);

	printChangesList(moveToRedo->changes,REDO_CHANGE_IND);
	promoteCurrPointer(g->undoList);
	return NO_ERROR;
}

ERROR executeReset(game* g){
	ERROR e = NO_ERROR;

	while(e!=NO_MOVES_TO_UNDO_ERROR){
		e=executeUndo(g);
	}
	return NO_ERROR;
}

ERROR executeDefaultEdit(game* g){
	int n,m;
	n=blockRows;
	m=blockColumns;
	destroyBoard(g->board);
	destroyBoard(g->boardTypes);
	destroyBoard(g->boardSol);
	destroyMovesList(g->undoList);
	g->board = createBoard(n,m);
	g->boardTypes = createBoard(n,m);
	g->boardSol = createBoard(n,m);
	g->undoList = createMovesList();
    g->currMode = EDIT_MODE;
    return NO_ERROR;/*the only possible error is malloc fatal error*/
}
ERROR executeCommand(command* pCommand, game* pGame){
    ERROR error;
    moveNode *move;
    int res;
    if(commandIsAMove(pCommand)){
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
        		error=executeDefaultEdit(pGame);

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
            error = executeGuessCommand(pGame, atof(pCommand->param1), NULL);
            break;
        case GENERATE:
            error = executeGenerateCommand(pGame,move, atoi(pCommand->param1), atoi(pCommand->param2));
            break;
        case UNDO:
        	error = executeUndo(pGame);
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
           error = executeAutofill(pGame,move);
            break;
        case RESET:
            error = executeReset(pGame);
            break;
        case SET:
            error = executeSetCommand(pGame,move, atoi(pCommand->param1), atoi(pCommand->param2), atoi(pCommand->param3));
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
    if(commandIsAMove(pCommand)&&error!=NO_ERROR){
    	destroyMoveNode(move);
    }
    if(commandIsAMove(pCommand)&&error==NO_ERROR){
    	makeMoveTheLastInTheList(pGame->undoList,pGame->undoList->curr);
    	addMove(pGame->undoList,move);
    	promoteCurrPointer(pGame->undoList);
    }

    if (pGame->currMode == SOLVE_MODE && pGame->board->emptyCellsCounter == 0){
           if (erroneousBoard(pGame->boardTypes)){
           	printf("This solution is incorrect!\n");
           	printf("You can undo you last move to continue solving\n");
           }
           else{
        	   pGame->currMode = BOARD_SOLVED_CORRECTLY_MODE;
           }
    }
    return error;
}

ERROR executeGuessCommand(game *game, double thresh, moveNode *move) {
    ERROR error;
    board* cpBoard;
    if (erroneousBoard(game->boardTypes))
        return COMMAND_UNAVAILABLE_WITH_ERRONEOUS_BOARD;
    cpBoard = createBoard(game->board->rows,game->board->columns);
    copyBoard(cpBoard, game->board);
    simpleAutofill(cpBoard);
    error = solveLPWithThreshold(cpBoard, thresh);
    if (error == NO_ERROR)
        copyBoardAndUpdateMove(game->board, cpBoard, move);
    destroyBoard(cpBoard);
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
    /*TODO add autofill before start, as well as checking erroneous*/
    board* origBoard, *cpBoard;
    int i, success, N, debug_clears;
    ERROR error;
    debug_clears = 0;
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
    for (i = 0; i < ((N*N)-y); i++) {
        clearRandomCell(cpBoard);
        debug_clears++;
    }
    /*printBoard(cpBoard, game->boardTypes, SOLVE_MODE, 1); TODO debugPrint*/
    printf("cleared %d cells\n y is %d, N is %d\n", debug_clears, y, N); /*TODO debugPrint*/
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
ERROR executeSetCommand(game *game,moveNode *move, int x, int y, int z) {
    int type, currMode, i, j;
    currMode = game->currMode;
    i = y - 1;
    j = x - 1;
    type = getCell(game->boardTypes, i, j);
    if (currMode == SOLVE_MODE && type == FIXED_CELL){
        	return FIXED_CELL_ERROR;}

    setCellUpdateErroneousAndMove(game->board, game->boardTypes,move, i,j, z,game->currMode,0);



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





