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
    param1 = (char *) (calloc(MAX_INPUT_SIZE, sizeof(char))); /*Calloc to make sure all chars are '\0' */
    param2 = (char *) (calloc(MAX_INPUT_SIZE, sizeof(char)));
    param3 = (char *) (calloc(MAX_INPUT_SIZE, sizeof(char)));
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
    return -1;

}
void printCommandName(command *c) {

    switch (c->name) {
        case EDIT:
        	printf("EDIT\n");
        	break;
        case PRINT_BOARD:
        	printf("PRINT_BOARD");
        	break;
        case VALIDATE:
        	printf("VALIDATE");
        	break;
        case UNDO:
        	printf("UNDO");
        	break;
        case REDO:
        	printf("REDO");
        	break;
        case NUM_SOLUTIONS:
        	printf("NUM_SOLUTIONS");
        	break;
        case AUTOFILL:
        	printf("AUTOFILL");
        	break;
        case RESET:
        	printf("RESET");
        	break;
        case EXIT:
        	printf("EXIT");
        	break;
        case SOLVE:
        	printf("SOLVE");
        	break;
        case MARK_ERRORS:
        	printf("MARK_ERRORS");
        	break;
        case GUESS:
        	printf("GUESS");
        	break;
        case SAVE:
        	printf("SAVE");
        	break;
        case GENERATE:
        	printf("GENERATE");
        	break;
        case HINT:
        	printf("HINT");
        	break;
        case GUESS_HINT:
        	printf("GUESS_HINT");
        	break;
        case SET:
        	printf("SET cell(%d,%d)=%d",atoi(c->param1), atoi(c->param2), atoi(c->param3));
        	break;
        case NONE:
        	printf("NONE");
        	break;
    }


}
/*follows the definition of "move" in page 3*/
int commandIsAMove(command* c){
	commandName name = c->name;
	if(name==SET||name==AUTOFILL||name==GENERATE||name==GUESS){
		return 1;
	}
	else{
		return 0;
	}

}
