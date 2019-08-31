/*
 * command.h
 *
 * we created this module because we needed a convenient way to store and transfer
 *  the data that is relevant to each command (transfer from parser module to the other modules)
 *  that is typed by the user. data such as the command type and the indices
 */

#ifndef COMMAND_H_
#define COMMAND_H_

#define MAX_INPUT_SIZE 1024
typedef enum{
	SOLVE, EDIT, MARK_ERRORS, PRINT_BOARD, GUESS,
	GENERATE, UNDO, REDO, SAVE, GUESS_HINT,
	NUM_SOLUTIONS, AUTOFILL, RESET, SET,HINT,VALIDATE,EXIT, NONE
}commandName;
typedef struct {
	commandName name; /*what type of command is it*/
	char* param1, *param2, *param3; /*At most 3 commands*/
}command;
command* createCommand();
void destroyCommand(command* PcurrCommand);
int numberOfParams(commandName name);
/*follows the definition of "move" in page 3*/
int commandIsAMove(command* c);
void printCommandName(command *c);
#endif /* COMMAND_H_ */
