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
	/*curr points to the the last move we executed - the move we want to undo in the next undo command
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
/*nodeIsStartSentinel(l,l->curr)==1 only if we undone all the commands*/
int nodeIsStartSentinel(movesList *l,moveNode* node);
/*deletes from the list all the moves that are after node
 * this function is used when we undo some commands and then execute a new command without
 * redoing all the commands we have undone  - all the old moves we undone should be deleted*/
void makeMoveTheLastInTheList(movesList* list,moveNode* node);
#endif /* COMMANDLIST_H_ */
