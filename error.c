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
            printf("Thanks for playing Roee and Omer's sudoku! We hope to see you again!\n");
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
            /*TODO:@Omer choose a message*/
            printf("Error: file could not be opened!\n");
            break;
        case FCLOSE_ERROR:
            /*TODO:@Omer choose a message. we need to decide whether to continue with the loaded board
             * with the previous board in that case*/
            printf("Error: fclose error!\n");
            break;
        case INVALID_FILE_FORMAT:
            printf("Error: invalid file format!\n");
            break;
        case FPRINTF_ERROR:
            printf("Error: failure to write to the specified file!\n");
            break;
        case TMP_ERROR:
            /*TODO:@Omer choose a message*/
            printf("Error: temporary error!\n");
            break;
    }
}

