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
int emptyMovesList(movesList *l);
void destroyMovesList();
void addMove(movesList *l,moveNode *m);
/*important! if the curr node is the last node in the list then the function does nothing*/
void promoteCurrPointer(movesList *l);
/*important! if the curr node is the first node in the list then the function does nothing*/
void demoteCurrPointer(movesList *l);
#endif /* COMMANDLIST_H_ */
