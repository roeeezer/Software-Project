/*
 * parser.c
 *
 *      Author: roee
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vss.h>
#include "command.h"
#include "game.h"
#include "error.h"

int correctNumberOfParams(commandName name, int numOfParams);
int readInteger(char *token, int *pInt);
int checkValidFloat(char* );

ERROR checkMarkErrorsParam(char *param1);

ERROR checkSetParams(char *param1, char *param2, char *param3);

ERROR checkGuessParam(char *param);

ERROR checkCommandValidForMode(commandName name, GAME_MODE mode);

typedef enum {
    X, Y, Z
} SETPARAMS;

int getInitializationInput(){
	int res,validScan;
	printf("Please enter the number of cells to fill [0-80]:\n");
	validScan=scanf("%d",&res);
	while(validScan==1){
		if(0<=res&&res<=80){
			return res;
		}
		else {
		    printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
            printf("Please enter the number of cells to fill [0-80]:\n");
		    validScan = scanf("%d", &res);
		}
	}
	/*assuming by project specifications that EOF*/
	return -1;
}

/**
 * Helper method to match the text of the command name and assign the correct commandName enum.
 * @param token the text the user input at the beginning of the line
 * @param name pointer to the commandName to be assigned
 * @return NO_ERROR if matched successfully, INCORRECT_PARAM_NAME otherwise.
 */
ERROR matchCommandName(const char *token, commandName *name) {
    ERROR returnCode = NO_ERROR;
    if (strcmp(token, "solve") == 0)
        *name = SOLVE;
    else if (strcmp(token, "edit") == 0)
        *name = EDIT;
    else if (strcmp(token, "mark_errors") == 0)
        *name = MARK_ERRORS;
    else if (strcmp(token, "print_board") == 0)
        *name = PRINT_BOARD;
    else if (strcmp(token, "set") == 0)
        *name = SET;
    else if (strcmp(token, "validate") == 0)
        *name = VALIDATE;
    else if (strcmp(token, "guess") == 0)
        *name = GUESS;
    else if (strcmp(token, "generate") == 0)
        *name = GENERATE;
    else if (strcmp(token, "undo") == 0)
        *name = UNDO;
    else if (strcmp(token, "redo") == 0)
        *name = REDO;
    else if (strcmp(token, "save") == 0)
        *name = SAVE;
    else if (strcmp(token, "hint") == 0)
        *name = HINT;
    else if (strcmp(token, "guess_hint") == 0)
        *name = GUESS_HINT;
    else if (strcmp(token, "num_solutions") == 0)
        *name = NUM_SOLUTIONS;
    else if (strcmp(token, "autofill") == 0)
        *name = AUTOFILL;
    else if (strcmp(token, "reset") == 0)
        *name = RESET;
    else if (strcmp(token, "exit") == 0)
        *name = EXIT;
    else {
        *name = NONE;
        returnCode = INCORRECT_COMMAND_NAME;
    }
    return returnCode;
}

/**
 * Checks that param values are legal. Assumes we already checked if number of params is appropriate
 * @param name
 * @param param1
 * @param param2
 * @param param3
 * @return the appropriate ERROR, or NO_ERROR if all params are legal
 */

ERROR checkLegalParam(command* pCommand, game* pGame) {
    char * param1, *param2, *param3;
    param1 = pCommand->param1;
    param2 = pCommand->param2;
    param3 = pCommand->param3;
    switch (pCommand->name){
        /*Assumes all text (i.e. path) or 0 param commands are by definition valid.
         * If path is incorrect the save/load will fail.*/
        case SOLVE:
        case EDIT:
        case PRINT_BOARD:
        case VALIDATE:
        case UNDO:
        case REDO:
        case SAVE:
        case NUM_SOLUTIONS:
        case AUTOFILL:
        case RESET:
        case EXIT:
            return NO_ERROR;
        case MARK_ERRORS:
            return checkMarkErrorsParam(param1);
        case SET:
            return checkSetParams(param1, param2, param3);
        case GUESS:
            return checkGuessParam(param1);
        case GENERATE:
            return checkGenerateParams(param1, param2);
        case GUESS_HINT:
            return checkGuessHintParams(param1, param2);
        case HINT:
            return checkHintParams(param1, param2);
        case NONE:
        default:
            break;
        /*TODO: implement these checks*/
    }

    return UNKNOWN_ERROR;
}


ERROR checkGuessParam(char *param) {
    if (checkValidFloat(param) && atof(param)>= 0.0 && atof(param) <= 1.0)
        return NO_ERROR;
    return PARAM_OUT_OF_RANGE;
}

ERROR checkSetParams(char *param1, char *param2, char *param3) {
    ERROR error1, error2, error3;
    error1 = checkSetParam(param1, X);
    error2 = checkSetParam(param2, Y);
    error3 = checkSetParam(param3, Z);
    if (error1 != NO_ERROR)
        return error1;
    if error2
    return NO_ERROR;
}

ERROR checkMarkErrorsParam(char *param1) {

    if (strcmp(param1, "0") == 0 || strcmp(param1, "1") == 0)
        return NO_ERROR;
    return PARAM_OUT_OF_RANGE;
}

/**
 * Reads command from stdin, and updates a pre-allocated command
 * Checks only errors of type 1 and 2 in page 8 of project specifications,
 * which can be checked regardless of game state.
 * @param pCommand pointer to an allocated command
 * @return the appropriate ERROR code, or NO_ERROR if a legal command was read successfully.
 */
ERROR readCommand(command *pCommand, game* pGame) {
    ERROR returnCode;
    int numberOfParams = 0;
    char *param1, *param2, *param3, *param4;
    char input[MAX_INPUT_SIZE];
    char* res; /*TODO: Make result an array of size MAX_INPUT_SIZE?*/
    char* token;
    commandName name;
    char *delim = " \t\r\n";
    res = fgets(input, MAX_INPUT_SIZE, stdin);
    if (strlen(input) >= 256) {
        return INPUT_TOO_LONG;
    }
    if (res == NULL){
        pCommand->name = EXIT;
        return NO_ERROR;
    }
    while (strcmp(res, "\n") == 0){ /*TODO: change it so blank lines count as empty commands and nothing happens*/
        res = fgets(input, MAX_INPUT_SIZE, stdin);
        if (res == NULL){
            pCommand->name = EXIT;
            return NO_ERROR;
        }
    }

    token = strtok(input, delim);
    if (token == NULL){
        pCommand->name = EXIT;
        return NO_ERROR;
    }
    returnCode = matchCommandName(token, &name);
    if (returnCode != NO_ERROR) {
        return returnCode;
    }
    returnCode = checkCommandValidForMode(name, pGame->currMode);
    if (returnCode != NO_ERROR)
        return returnCode;
    param1 = strtok(NULL, delim);
    param2 = strtok(NULL, delim);
    param3 = strtok(NULL, delim);
    param4 = strtok(NULL, delim); /* param4 only used to check if more text remains after 3rd param read*/
    numberOfParams += (param1 != NULL) + (param2 != NULL) + (param3 != NULL) + (param4 != NULL);
    if (!correctNumberOfParams(name, numberOfParams))
        return INCORRECT_NUMBER_OF_PARAMS;
    pCommand->name = name;
    strcpy(pCommand->param1, param1);
    strcpy(pCommand->param2, param2);
    strcpy(pCommand->param3, param3);
    return NO_ERROR;
}

ERROR checkCommandValidForMode(commandName name, GAME_MODE mode) {
    ERROR returnCode = NO_ERROR;
    switch (name){
        case SOLVE:
        case EDIT:
        case EXIT:
            break;
        case MARK_ERRORS:
        case GUESS:
        case HINT:
        case GUESS_HINT:
        case AUTOFILL:
            if (mode != SOLVE_MODE)
                returnCode = INVALID_GAME_MODE_SOLVE;
            break;
        case PRINT_BOARD:
        case SET:
        case VALIDATE:
        case UNDO:
        case REDO:
        case SAVE:
        case NUM_SOLUTIONS:
        case RESET:
            if (mode == INIT_MODE)
                returnCode = INVALID_GAME_MODE_SOLVE_EDIT;
            break;
        case GENERATE:
            if (mode != EDIT_MODE)
                returnCode = INVALID_GAME_MODE_EDIT;
            break;
        case NONE:
            returnCode = UNKNOWN_ERROR;
            break;
    }
    return returnCode;
}

/**
 * Verifies the correct number of params for the given commandName
 * @param name the commandName that was read
 * @param numOfParams number of params successfully read from user
 * @return 1 if number of params read matches required params for the command, 0 otherwise
 */
int correctNumberOfParams(commandName name, int numOfParams) {
    switch (numOfParams){
        case 0:
            if (name == EDIT || name == PRINT_BOARD ||
                name == VALIDATE || name == UNDO ||
                name == REDO || name == NUM_SOLUTIONS ||
                name == AUTOFILL || name == RESET ||
                name == EXIT)
                return 1;
            break;
        case 1:
            if (name == SOLVE || name == EDIT || name == MARK_ERRORS ||
                name == GUESS || name == SAVE)
                return 1;
            break;
        case 2:
            if (name == GENERATE || name == HINT || name == GUESS_HINT)
                return 1;
            break;
        case 3:
            if (name == SET)
                return 1;
            break;
        case 4:
        default: return 0;
    }
    return 0;
}

int checkValidFloat(char* str){/*TODO: complete this method*/
    int i, decimalSeen;
    decimalSeen = 0;
    for (i = 0; i < (int)strlen(str); i++){
        if (decimalSeen && str[i] == '.')
            return 0;
        if (str[i] == '.')
            decimalSeen = 1;
        else if (str[i] < '0' || str[i] > '9')
            return 0;
    }
    return 1;
}

/*
 * Reads an integer from the token to a given integer point
 * Returns 1 if the token is a str representing a integer (succeeded in conversion)
 * Returns 0 otherwise*/
int readInteger(char *token, int *pInt) {
    if (token == NULL)
        return 0;
    *pInt = atoi(token); /*Per specifications, assumes valid integer*/
    /*TODO: add checking to see if input is valid int*/
    return 1;
}

