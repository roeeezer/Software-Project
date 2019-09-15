/*
 *
 *      Author: roee
 *      each game has such a list that contains all the moves (represented by
 *      moveNode) that the user has done so far
 */

#ifndef COMMANDLIST_H_
#define COMMANDLIST_H_

#include "moveNode.h"
#define STANDART_STATE 0
#define NO_MOVES_TO_UNDO_STATE 1
#define EMPTY_LIST_STATE 3
/*NO_MOVES_TO_UNDO_STATE is needed because in the way that the curr pointer and demoteCurrPointer()
 * are operating, if list->curr==list->first we need a way to determine whether or not the move
 *  list->first was already undone  */


typedef struct{
	moveNode *first;
	moveNode *curr;
	/*curr points to the the last move we executed - the move we want to undo in the next undo command
	 * curr->next if exist is the move we will redo in the next redo command*/
	moveNode *last;
	int currPointerState;
	/*currPointerState contains one of the values defined by the macros:
	 * STANDART_STATE,NO_MOVES_TO_UNDO_STATE,EMPTY_LIST_STATE*/
}movesList;

movesList* createMovesList();
int emptyMovesList(movesList *l);
void destroyMovesList();
void addMove(movesList *l,moveNode *m);
void promoteCurrPointer(movesList *l);
/* */
void demoteCurrPointer(movesList *l);
void printMovesList(movesList *l);
/*nodeIsStartSentinel(l,l->curr)==1 only if we undone all the commands*/
int nodeIsStartSentinel(movesList *l,moveNode* node);
/*deletes from the list all the moves that are after node
 * this function is used when we undo some commands and then execute a new command without
 * redoing all the commands we have undone  - all the old moves we undone should be deleted*/
void makeMoveTheLastInTheList(movesList* list,moveNode* node);
#endif /* COMMANDLIST_H_ */
