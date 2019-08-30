/*
 * changeNode.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#ifndef CHANGENODE_H_
#define CHANGENODE_H_

typedef struct ChangeNode{
	int i;
	int j;
	int prevVal;
	struct ChangeNode *next;

}changeNode;
changeNode* createChangeNode(int i,int j,int prevVal);
void destroyChangeNode(changeNode* change);
void destroyAllChangeNodesStartingFrom(changeNode* start);
void printChangesStartingFrom(changeNode *start);

#endif /* CHANGENODE_H_ */
