/*
 * error.h
 *
 *  Created on: 3 баев„ 2019
 *      Author: Omer
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include "command.h"

typedef enum {
    NO_ERROR,
    INPUT_TOO_LONG,
    INCORRECT_COMMAND_NAME,
    INCORRECT_NUMBER_OF_PARAMS,
    PARAM_OUT_OF_RANGE, /*Param is of illegal type or format, or not in legal range*/
    INPUT_ERROR,
    MALLOC_ERROR,
    EXIT_MESSAGE,
    INVALID_GAME_MODE_SOLVE_EDIT,
    INVALID_GAME_MODE_SOLVE,
    INVALID_GAME_MODE_EDIT,
	FOPEN_ERROR,
	FCLOSE_ERROR,
	INVALID_FILE_FORMAT,
	TMP_ERROR,/*just for compilation reasons*/
    GUROBI_ERROR,
    UNKNOWN_ERROR /*An unknown error occurred. For checking if we reached unexpected situations*/
} ERROR;
void printErrorMessage(ERROR error, command *pCommand);

#endif /* ERROR_H_ */
