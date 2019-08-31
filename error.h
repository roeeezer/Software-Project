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
	FPRINTF_ERROR,
	SAVE_ERRONEOUS_CELLS_IN_EDIT_MODE,
	SAVE_UNSOLVABLE_BOARD_IN_EDIT_MODE,
	FIXED_ERRONEOUS_CELLS_IN_FILE,
	AUTOFILL_ERRONEOUS_BOARD,
	FIXED_CELL_ERROR,
	BOARD_SOLVED_ERRONEOUS,
	BOARD_SOLVED_CORRECTLY,
	GENERATE_NOT_ENOUGH_CELLS,
	FAILED_TO_GENERATE,
    GUESS_ERRONEOUS_BOARD,
	NO_CHANGES_TO_UNDO,
	TMP_ERROR,/*just for compilation reasons*/
    GUROBI_ERROR,
    UNKNOWN_ERROR /*An unknown error occurred. For checking if we reached unexpected situations*/
} ERROR;
void printErrorMessage(ERROR error, command *pCommand);

#endif /* ERROR_H_ */
