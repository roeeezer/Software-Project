/*
 * command.c
 *
 *      Author: roee
 */
#include "command.h"
#include <stdlib.h>
#include <stdio.h>

command* createCommand(){
    char* param1, *param2,*param3;
    command* res;
    param1 = (char *) (malloc(sizeof(char) * MAX_INPUT_SIZE));
    param2 = (char *) (malloc(sizeof(char) * MAX_INPUT_SIZE));
    param3 = (char *) (malloc(sizeof(char) * MAX_INPUT_SIZE));
	res = (command*) malloc(sizeof(command));
	if (res == NULL || param1 == NULL || param2 == NULL || param3 == NULL){
        printf("Error: malloc has failed\n");
        exit(-42);
	}
	res->param1 = param1;
	res->param2 = param2;
    res->param3 = param3;
	return res;
}
void destroyCommand(command* PcurrCommand){
    free(PcurrCommand->param1);
    free(PcurrCommand->param2);
    free(PcurrCommand->param3);
	free(PcurrCommand);
}
/**
 *
 * @param name
 * @return the number of params needed for this command. Returns 0 for EDIT.
 */
int numberOfParams(commandName name) {
    switch (name) {
        case EDIT:
        case PRINT_BOARD:
        case VALIDATE:
        case UNDO:
        case REDO:
        case NUM_SOLUTIONS:
        case AUTOFILL:
        case RESET:
        case EXIT:
            return 0;
        case SOLVE:
        case MARK_ERRORS:
        case GUESS:
        case SAVE:
            return 1;
        case GENERATE:
        case HINT:
        case GUESS_HINT:
            return 2;
        case SET:
            return 3;
        case NONE:
            return -1;
    }
    return 0;
}
