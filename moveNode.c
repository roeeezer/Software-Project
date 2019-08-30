/*
 * moveNode.c
 *
 */
#include "moveNode.h"

moveNode* createMoveNode(command* c){
	moveNode* res;
	if(!commandIsAMove(c)){
		printf("ERROR! cannot envelope a command that is not a move in a moveNode");
		return NULL;
	}
	res = (moveNode*)malloc(sizeof(moveNode*));
	res->command = c;
	res->next=NULL;
	res->prev=NULL;
	return res;


}
void destroyMoveNode(moveNode* m){
	destroyCommand(m->command);
	free(m);
}

