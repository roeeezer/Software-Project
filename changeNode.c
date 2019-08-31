/*
 * changeNode.c
 *
 */
#include "changeNode.h"


changeNode* createChangeNode(int i,int j,int prevVal){
	changeNode* res = (changeNode*)malloc(sizeof(changeNode));
	res->i = i;
	res->j = j;
	res->prevVal = prevVal;
	res->next=NULL;
	return res;

}
void destroyChangeNode(changeNode* change){
	free(change);

}
void destroyAllChangeNodesStartingFrom(changeNode* start){
	if(start->next==NULL){
		destroyChangeNode(start);
	}
	destroyAllChangeNodesStartingFrom(start->next);
	destroyChangeNode(start);
}
void printChange(changeNode* change){
	printf("Cell (%d,%d) value changed back to %d\n",change->j+1,change->i+1,change->prevVal);
}
void printChangesStartingFrom(changeNode* start){
	if(start->next==NULL){
		printChange(start);
		return;
	}
	printChangesStartingFrom(start->next);
	printChange(start);

}


