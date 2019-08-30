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
void destroyChangesList();


#endif /* CHANGESLIST_H_ */
