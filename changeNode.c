/*
 * changeNode.c
 *
 */
#include "changeNode.h"


changeNode* createChangeNode(int i,int j,int prevVal,int newVal){
	changeNode* res = (changeNode*)malloc(sizeof(changeNode));
	res->i = i;
	res->j = j;
	res->prevVal = prevVal;
	res->newVal = newVal;
	res->next=NULL;
	return res;

}
void destroyChangeNode(changeNode* change){
	free(change);

}
/*destroys the node "start" and all the changeNodes that comes next to it*/
void destroyAllChangeNodesStartingFrom(changeNode* start){
	if(start==NULL){
		return;
	}
	if(start->next==NULL){
		destroyChangeNode(start);
		return;
	}
	destroyAllChangeNodesStartingFrom(start->next);
	destroyChangeNode(start);
}
void printChange(changeNode* change,int ind){
	if(ind==UNDO_CHANGE_IND){
		printf("Cell (%d,%d) value was set to %d\n",change->j+1,change->i+1,change->prevVal);
		}
	if(ind==REDO_CHANGE_IND){
		printf("Cell (%d,%d) value was set to %d\n",change->j+1,change->i+1,change->newVal);
		}
}
/*prints the node "start" and all the changeNodes that comes next to it*/
void printChangesStartingFrom(changeNode* start,int ind){
	if(start->next==NULL){
		printChange(start,ind);
		return;
	}
	printChangesStartingFrom(start->next,ind);
	printChange(start,ind);

}


