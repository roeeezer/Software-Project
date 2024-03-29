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
	res = (moveNode*)malloc(sizeof(moveNode));
    if (res == NULL) {
        printf("Error: malloc has failed!\n");
        exit(-42);
    }
	res->command = c;
	res->changes = createChangesList();
	res->next=NULL;
	res->prev=NULL;
	return res;


}
void destroyMoveNode(moveNode* m){
	destroyCommand(m->command);
	destroyChangesList(m->changes);
	free(m);
}
/*destroys the node "start" and all the moveNodes that comes next to it*/
void destroyAllMoveNodesStartingFrom(moveNode* start){
	if(start==NULL){
		return;
	}
	if(start->next==NULL){
		destroyMoveNode(start);
		return;
	}
	destroyAllMoveNodesStartingFrom(start->next);
	destroyMoveNode(start);
}



