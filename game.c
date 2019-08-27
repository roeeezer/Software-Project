/*
 * game.c
 *
 *      Author: roee
 */
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "solver.h"
#include "game.h"




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
	resetBoard(res->boardTypes,0);
	res->currMode = INIT_MODE;

	return res;
}

void destroyGame(game* game){
    destroyBoard(game->board);
    destroyBoard(game->boardSol);
    free(game->boardTypes);
	/*free(game->board); *//*destroyboard?*//*
	free(game->boardSol);*/
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

ERROR executeCommand(command* pCommand, game* pGame){
    ERROR error;
    error = checkLegalParam(pCommand, pGame);
    if (error != NO_ERROR)
        return error;
    /*After this point, command is assumed legal for this game state.*/
    switch(pCommand->name) {
        case SOLVE:
            error = loadBoard(pGame->board,pGame->boardTypes, pCommand->param1); /*TODO: @Roee implement loadBoard, param1 is the path*/
            if (error == NO_ERROR)
                pGame->currMode = SOLVE_MODE;
            break;
        case EDIT:
            /*error = loadBoard(pGame, pCommand->param1); TODO: uncomment this*/
            if (error == NO_ERROR)
                pGame->currMode = EDIT_MODE;
            break;
        case MARK_ERRORS:
            pGame->mark_errors = atoi(pCommand->param1);
            error = NO_ERROR;
            break;
        case PRINT_BOARD:
            printBoard(pGame->board, pGame->boardTypes);
            error = NO_ERROR;
            break;
        case GUESS:
            /*error = guess(pGame, atof(pCommand->param1));*/
            break;
        case GENERATE:
            /*error = generate(pGame, atoi(pCommand->param1), atoi(pCommand->param2)); TODO: uncomment this*/
            break;
        case UNDO:
            /* error = undo_move(pGame); TODO: @Roee implement TODO: uncomment this*/

            break;
        case REDO:
            /*error = redo_move(pGame); TODO: @Roee implement TODO: uncomment this*/
            break;
        case SAVE:
            /*error = saveBoard(pGame, pCommand->param1); TODO: @Roee implement TODO: uncomment this*/
            break;
        case GUESS_HINT:
            /* error = guessHint(pGame, atoi(pCommand->param1), atoi(pCommand->param2)); TODO: uncomment this*/
            break;
        case NUM_SOLUTIONS:
            /*error = numSolutions(pGame->board); TODO: @Roee implement.TODO: uncomment this*/
            break;
        case AUTOFILL:
            /*error = autofillBoard(pGame->board); TODO: @Roee? implement this TODO: uncomment this*/
            break;
        case RESET:
            /*error = fullResetBoard(pGame); TODO: @Roee implement this TODO: uncomment this*/
            break;
        case SET:
            /*executeSetCommand(pGame, atoi(pCommand->param1), atoi(pCommand->param2), atoi(pCommand->param3));*/
            /*TODO: @Omer implement TODO: uncomment this*/
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
    return error;
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
	        printBoard(Pgame->board,Pgame->boardTypes);
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



