/*
 *
 *      Author: roee
 */
#include "moveNode.h"
#ifndef COMMANDLIST_H_
#define COMMANDLIST_H_

#define STANDART_STATE 0
#define NO_MOVES_TO_UNDO_STATE 1
#define NO_MOVES_TO_REDO_STATE 2
#define EMPTY_LIST_STATE 3

typedef struct{
	moveNode *first;
	moveNode *curr;
	moveNode *last;
	int currPointerState;
}movesList;

movesList* createMovesList();
int emptyMovesList(movesList *l);
void destroyMovesList();
void addMove(movesList *l,moveNode *m);
/*important! if the curr node is the last node in the list then the function does not
 * changes the curr pointer - but changes currPointerState */
void promoteCurrPointer(movesList *l);
/*important! if the curr node is the first node in the list then the function does not
 * changes the curr pointer - but changes currPointerState */
void demoteCurrPointer(movesList *l);
#endif /* COMMANDLIST_H_ */
