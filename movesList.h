/*
 *
 *      Author: roee
 */
#include "moveNode.h"
#ifndef COMMANDLIST_H_
#define COMMANDLIST_H_
typedef struct{
	moveNode *first;
	moveNode *curr;
	moveNode *last;
}movesList;

movesList* createMovesList();
void destroyMovesList();
void addMove(movesList *l,moveNode *m);
void promoteCurrPointer(movesList *l);
#endif /* COMMANDLIST_H_ */
