/*
 * command.c
 *
 *      Author: roee
 */
#include "command.h"
#include <stdlib.h>
#include <stdio.h>

command* createCommand(){
	command* res = (command*) malloc(sizeof(command));
	if (res == NULL){
        printf("Error: malloc has failed\n");
        exit(-42);
	}
	return res;
}
void destroyCommand(command* PcurrCommand){
	free(PcurrCommand);
}
