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


game* createGame(){
	game* res = (game*) malloc(sizeof(game));
	if (res == NULL){
        printf("Error: malloc has failed\n");
        exit(-42);
	}
	res->board = createBoard(blockRows,blockColumns);
	res->boardTypes = createBoard(blockRows,blockColumns);
	res->undoList = createMovesList();
	resetBoard(res->boardTypes, REGULAR_CELL);
	res->currMode = INIT_MODE;
	res->mark_errors=1;

	return res;
}

void destroyGame(game* game){
    destroyBoard(game->board);
    destroyBoard(game->boardTypes);
    destroyMovesList(game->undoList);
	free(game);
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
    	destroyMovesList(pGame->undoList);
    	pGame->board = newBoard;
    	pGame->boardTypes = newBoardTypes;
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
	destroyMovesList(g->undoList);
	g->board = createBoard(n,m);
	g->boardTypes = createBoard(n,m);
	g->undoList = createMovesList();
    g->currMode = EDIT_MODE;
    return NO_ERROR;/*the only possible error is malloc fatal error*/
}

/**
 * Receives a command and a game, and performs it.
 * Calls checkLegalParam to make sure that the command is legal.
 * Errors arising from command execution are returned.
 * @param pCommand
 * @param pGame
 * @return appropriate error, or NO_ERROR if command executed succesfully
 */
ERROR executeCommand(command* pCommand, game* pGame){
    ERROR error;
    moveNode *move;
    int res;
    error = checkLegalParam(pCommand, pGame);
    if (error != NO_ERROR)
        return error;

    /*After this point, command is assumed legal for this game state.*/
    if(commandIsAMove(pCommand)){
    	move = createMoveNode(pCommand);
    }

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
            error = executeGuessHintCommand(pGame->board, pGame->boardTypes, atoi(pCommand->param1), atoi(pCommand->param2));
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
            break;
        case HINT:
            error = executeHintCommand(pGame->board, pGame->boardTypes, atoi(pCommand->param1), atoi(pCommand->param2));
            break;
        case VALIDATE:
            error = executeValidateCommand(pGame->board);
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
    else if(commandIsAMove(pCommand)&&error==NO_ERROR){
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

ERROR executeValidateCommand(board *pBoard) {
    board * cpBoard;
    ERROR error;
    cpBoard = createBoard(pBoard->rows, pBoard->columns);
    copyBoard(cpBoard, pBoard);
    error = solveILP(cpBoard);
    destroyBoard(cpBoard);
    if (error == NO_ERROR)
        printf("Validation passed! Board is solvable\n");
    return error;
}
/**
 *
 * @param pBoard
 * @param bTypes
 * @param x the column (1-indexed)
 * @param y the row (1-indexed)
 * @return
 */
ERROR executeHintCommand(board *pBoard, board *bTypes, int x, int y) {
    ERROR error;
    board * cpBoard;
    int i, j;
    i = y-1;
    j = x-1;
    if (erroneousBoard(bTypes)) {
        return HINT_OR_GUESS_BOARD_ERRONEOUS;
    }
    if (getCell(bTypes, i, j) == FIXED_CELL)
        return HINT_OR_GUESS_FIXED_CELL;
    if (!emptyCell(pBoard, i, j))
        return HINT_OR_GUESS_NON_EMPTY_CELL;
    cpBoard = createBoard(pBoard->rows, pBoard->columns);
    copyBoard(cpBoard, pBoard);
    error = solveILP(cpBoard);
    if (error == NO_ERROR)
        printf("Hint: the value of cell <%d,%d> is: %d\n", x, y, getCell(cpBoard, i, j));
    destroyBoard(cpBoard);
    return error;
}

/**
 *
 * @param pBoard
 * @param boardTypes
 * @param x the column (1-indexed)
 * @param y the row (1-indexed)
 * @return appropriate error
 */
ERROR executeGuessHintCommand(board *pBoard, board *boardTypes, int x, int y) {
    int N, len, counter, *cellValues, i, j;
    double * cellScores;
    ERROR error;
    i = y - 1;
    j = x - 1;
    N = pBoard->squareSideSize;
    if (erroneousBoard(boardTypes)) {
        return HINT_OR_GUESS_BOARD_ERRONEOUS;
    }
    if (getCell(boardTypes, i, j) == FIXED_CELL) {
        if (DEBUG) printf("Guess hint on fixed cell!");
        return HINT_OR_GUESS_FIXED_CELL;
    }
    if (!emptyCell(pBoard, i, j))
        return HINT_OR_GUESS_NON_EMPTY_CELL;
    cellValues = (int *) malloc(N * sizeof(int));
    if (cellValues == NULL)
        return MALLOC_ERROR;
    cellScores = (double *) malloc(N * sizeof(double));
    if (cellScores == NULL)
        return MALLOC_ERROR;
    error = solveLPForTargetCell(pBoard, i, j, cellValues, cellScores, &len);
    if (error == NO_ERROR) {
        printf("These are the possible values and scores guessed for cell <%d,%d>\n", x, y);
        for (counter = 0; counter < len; counter++) {
            if (cellScores[counter] > 0)
                printf("The value: %d with a score of: %f\n", cellValues[counter], cellScores[counter]);
        }
    }
    free(cellScores);
    free(cellValues);
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
    for (i = 0; i < 2; i++) { /*TODO CHANGE BACK TO 1000*/
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
    if (DEBUG)printf("cleared %d cells\n y is %d, N is %d\n", debug_clears, y, N); /*TODO debugPrint*/
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

int commandMightHaveChangedBoard(command* c){
	commandName name = c->name;
	return name==EDIT||name==SOLVE||name==SET||name==AUTOFILL||name==UNDO||name==REDO||name==GENERATE||name==GUESS||name==RESET;
}





