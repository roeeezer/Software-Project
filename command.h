/*
 * command.h
 *
 * we created this module because we needed a convenient way to store and transfer
 *  the data that is relevant to each command (transfer from parser module to the other modules)
 *  that is typed by the user. data such as the command type and the indices
 */

#ifndef COMMAND_H_
#define COMMAND_H_
typedef enum{
	SET,HINT,VALIDATE,RESTART,EX, NONE
}commandName;
typedef struct{
	commandName name; /*what type of command is it*/
	int j, i, z; /*The column, row, and, if necessary, value of the command*/
}command;
command* createCommand();
void destroyCommand(command* PcurrCommand);

#endif /* COMMAND_H_ */
