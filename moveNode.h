/*
 * moveNode.h
 *
 */
#include "command.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef MOVENODE_H_
#define MOVENODE_H_
typedef struct MoveNode{
	command* command;
	struct MoveNode *prev;
	struct MoveNode *next;

}moveNode;
moveNode* createMoveNode(command* c);
void destroyMoveNode(moveNode* m);

#endif /* MOVENODE_H_ */
