/*
 *
 *      Author: roee
 */

#ifndef COMMANDLIST_H_
#define COMMANDLIST_H_

#include "moveNode.h"
#define STANDART_STATE 0
#define NO_MOVES_TO_UNDO_STATE 1
#define EMPTY_LIST_STATE 3
/*NO_MOVES_TO_UNDO_STATE is need because in the way that the curr pointer and demoteCurrPointer()
 * are operating, if list->curr==list->first we need a way to determine whether or not the move
 *  list->first was already undone  */


typedef struct{
	moveNode *first;
	moveNode *curr;
	/*curr points to the move we want to undo in the next undo command
	 * curr->next if exist is the move we will redo in the next redo command*/
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
void printMovesList(movesList *l);
#endif /* COMMANDLIST_H_ */
