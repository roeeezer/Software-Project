/*
 * error.c
 *
 *      Author: roee
 */
#include "error.h"

/**
 * Prints the relevant error message
 * @param error
 * @param pCommand
 */

void printErrorMessage(ERROR error, command *pCommand){
    switch(error) {
        case NO_ERROR:
        	/*temp: just for debuging*/
        	printf("No error message!\n");
            break;
        case INPUT_TOO_LONG:
            printf("Error: input must be at most 256 characters long.\n");
            break;
        case INCORRECT_COMMAND_NAME:
            printf("Error: invalid command name\n");
            break;
        case INCORRECT_NUMBER_OF_PARAMS:
            if (pCommand->name == EDIT)
                printf("Error: this command takes exactly 0 or 1 arguments\n");
            else
                printf("Error: this command takes exactly %d arguments\n", numberOfParams(pCommand->name));
            break;
        case PARAM_OUT_OF_RANGE:
            break;
        case INPUT_ERROR:
            printf("INPUT_ERROR message not implemented\n");
            break;
        case MALLOC_ERROR:
            printf("Error: malloc has failed\n");
            break;
        case UNKNOWN_ERROR:
            printf("An unknown error occurred. This shouldn't happen\n");
            break;
        case EXIT_MESSAGE:
            printf("Thanks for playing Roee and Omer's Sudoku! We hope to see you again!\n");
            break;
        case INVALID_GAME_MODE_SOLVE_EDIT:
            printf("Error: This command is only available in Solve or Edit modes.\n");
            break;
        case INVALID_GAME_MODE_SOLVE:
            printf("Error: This command is only available in Solve mode.\n");
            break;
        case INVALID_GAME_MODE_EDIT:
            printf("Error: This command is only available in Edit mode.\n");
            break;
        case GUROBI_ERROR:
            printf("Error: Gurobi has encountered an error!\n");
            break;
        case FOPEN_ERROR:
            printf("Error: file could not be opened!\n");
            break;
        case FCLOSE_ERROR:
            printf("Error: file could not be closed!\n");
            break;
        case INVALID_FILE_FORMAT:
            printf("Error: invalid file format!\n");
            break;
        case INVALID_FILE_FORMAT_BOARDS_WERE_CREATED:
            printf("Error: invalid file format!\n");
            break;
        case FPRINTF_ERROR:
            printf("Error: failure to write to the specified file!\n");
            break;
        case SAVE_ERRONEOUS_CELLS_IN_EDIT_MODE:
            printf("Error: cannot save a board with erroneous cell in edit mode!\n");
            break;
        case SAVE_UNSOLVABLE_BOARD_IN_EDIT_MODE:
            printf("Error: cannot save a board without a solution in edit mode!\n");
            break;
        case FIXED_ERRONEOUS_CELLS_IN_FILE:
            printf("Error: cannot load a board with fixed erroneous cells!\n");
        	break;
        case COMMAND_UNAVAILABLE_WITH_ERRONEOUS_BOARD:
            printf("Error: this command is unavailable with erroneous board!\n");
        	break;
        case NO_MOVES_TO_UNDO_ERROR:
            printf("Error: no moves to undo!\n");
        	break;
        case NO_MOVES_TO_REDO_ERROR:
            printf("Error: no moves to redo!\n");
        	break;
        case TMP_ERROR:
            printf("Error: temporary error!\n");
            break;
        case FIXED_CELL_ERROR: /*TODO: implement all these*/
        	printf("Error: you cannot set a fixed cell in solve mode!\n");
            break;
        case GENERATE_NOT_ENOUGH_CELLS:
            break;
        case FAILED_TO_GENERATE:
            break;
        case GUESS_ERRONEOUS_BOARD:
        	/*@Omer perhaps you should delete this error and use COMMAND_UNAVAILABLE_WITH_ERRONEOUS_BOARD instead*/
            break;
    }
}

