/*
 * changeNode.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include"board.h"
#ifndef CHANGENODE_H_
#define CHANGENODE_H_
typedef struct ChangeNode{
	board* b;
	int i;
	int j;
	int prevVal;
	struct ChangeNode *next;

}changeNode;


#endif /* CHANGENODE_H_ */
