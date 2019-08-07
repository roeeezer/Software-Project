/*
 * parser.c
 *
 *      Author: roee
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "command.h"
#include "game.h"

#define MAX_INPUT_SIZE 1024

int readInteger(char *token, int *pInt);

int getIntializationInput(){
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

/*
 * Reads the command from user, updates to an allocated command
 * returns NULL if unable to init command*/
void readCommand(command *pCommand) {
	char input[MAX_INPUT_SIZE];
	char* res;
	int x=-1,y=-1,z=-1, success=1;
	char* token;
	commandName name;
    char *delim = " \t\r\n";
    pCommand->j = -1;
    pCommand->i = -1;
    pCommand->z = -1;
    res = fgets(input, MAX_INPUT_SIZE, stdin);
	if (res == NULL){
        pCommand->name = EX;
        return;

	}
	while (strcmp(res, "\n") == 0){
	    res = fgets(input, MAX_INPUT_SIZE, stdin);
        if (res == NULL){
            pCommand->name = EX;
            return;
        }
	}

    token = strtok(input, delim);
	if (token == NULL){
	    pCommand->name = EX;
	    return;
	}
	if (strcmp(token, "validate") == 0)
	    name = VALIDATE;
	else if (strcmp(token, "restart") == 0)
	    name = RESTART;
	else if (strcmp(token, "exit") == 0)
	    name = EX;
	else if (strcmp(token, "set") == 0)
	    name = SET;
	else if (strcmp(token, "hint") == 0)
	    name = HINT;
	else {
	    name = NONE; /* Possibly we should return a command with the name being NONE*/
	}
	if (name == HINT || name == SET){
	    token = strtok(NULL, delim);
	    success *= readInteger(token, &x);
        token = strtok(NULL, delim);
        success *= readInteger(token, &y);
	}
	if (name == SET){
	    token = strtok(NULL, delim);
	    success *= readInteger(token, &z);
	}
	if (!success)
	    name = NONE;
	pCommand->name = name;
	pCommand->j = x;
	pCommand->i = y;
	pCommand->z = z;
}

/*
 * Reads an integer from the token to a given integer point
 * Returns 1 if the token is a str representing a integer (succeeded in conversion)
 * Returns 0 otherwise*/
int readInteger(char *token, int *pInt) {
    if (token == NULL)
        return 0;
    *pInt = atoi(token); /*Per specifications, assumes valid integer*/
    return 1;
}

