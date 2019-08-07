/*
 * parser.h
 *
 *      Author: roee
 * parser module contains all the functions that interacts with the user.
 */

#ifndef PARSER_H_
#define PARSER_H_
#include "command.h"
#include "game.h"
/*getIntializationInput
 * returns the number of fixed cells (cells o fill)
 *  the user wants at the beginning*/
int getIntializationInput();

/*Reads the command from the user
 * returns a valid command if possible. Otherwise,
 * returns a command with name equal to NONE */
void *readCommand(command *pCommand);


#endif /* PARSER_H_ */
