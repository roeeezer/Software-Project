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
#include "error.h"
/*getInitializationInput
 * returns the number of fixed cells (cells o fill)
 *  the user wants at the beginning*/
int getInitializationInput();
/**
 * Checks that the command is legal for the current game state.
 * i.e., checks for type 3 and 4 errors in project specifications, page 8.
 * @param pCommand the
 * @param pGame
 * @return the appropriate error, or NO_ERROR if command is legal.
 */
ERROR checkLegalParam(command* pCommand, game* pGame);

/*Reads the command from the user
 * returns a valid command if possible. Otherwise,
 * returns a command with name equal to NONE */
ERROR readCommand(command *pCommand, game* pGame);
void printWelcomeMessage();
void askForCommand(game* g);

#endif /* PARSER_H_ */
