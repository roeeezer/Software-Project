/*
 * changesList.h
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include "changeNode.h"
#ifndef CHANGESLIST_H_
#define CHANGESLIST_H_
typedef struct{
	changeNode *first;
}changesList;

changesList* createChangesList();
int emptyChangesList(changesList* list);
void destroyChangesList(changesList* l);
void InsertFirst(changesList* list,changeNode* change);
void printChangesList(changesList* list,int ind);

#endif /* CHANGESLIST_H_ */
