/*
 * moveNode.h
 * the Node object that wraps every move in the movesList of the game
 *
 */
#include "command.h"
#include <stdio.h>
#include <stdlib.h>
#include "changesList.h"

#ifndef MOVENODE_H_
#define MOVENODE_H_
typedef struct MoveNode{
	command* command;
	struct MoveNode *prev;
	struct MoveNode *next;
	changesList *changes;

}moveNode;
moveNode* createMoveNode(command* c);
void destroyMoveNode(moveNode* m);
void destroyAllMoveNodesStartingFrom(moveNode* start);
#endif /* MOVENODE_H_ */
