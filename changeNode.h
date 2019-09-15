/*
 * changeNode.h
 * contains all the information needed for the undo\redo commands about a single change
 * in a single move
 *
 */
#include <stdio.h>
#include <stdlib.h>
#ifndef CHANGENODE_H_
#define CHANGENODE_H_


#define UNDO_CHANGE_IND 0
#define REDO_CHANGE_IND 1
/*these indications are for the functions of undo\redo changes and printing changes*/

typedef struct ChangeNode{
	int i;
	int j;
	int prevVal;
	int newVal;
	struct ChangeNode *next;

}changeNode;
changeNode* createChangeNode(int i,int j,int prevVal,int newVal);
void destroyChangeNode(changeNode* change);
void destroyAllChangeNodesStartingFrom(changeNode* start);
void printChangesStartingFrom(changeNode* start,int ind);
void printChange(changeNode* change,int ind);

#endif /* CHANGENODE_H_ */
