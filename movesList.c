/*
 *
 *      Author: roee
 */
#include <stdio.h>
#include <stdlib.h>
#include "movesList.h"
movesList* createMovesList(){
	movesList* res;
	res = (movesList*) malloc(sizeof(movesList));
	res->first=NULL;
	res->curr=NULL;
	res->last=NULL;
	res->currPointerState=EMPTY_LIST_STATE;

	return res;

}

int emptyMovesList(movesList *l){
	return l->first==NULL;
}
void destroyMovesList(movesList* l){
	if(!emptyMovesList(l)){
		destroyAllMoveNodesStartingFrom(l->first);
	}
	free(l);
}
void addMove(movesList *l,moveNode *m){
	l->currPointerState=NO_MOVES_TO_REDO_STATE;
	if(emptyMovesList(l)){
		l->first = m;
		l->curr = m;
		l->last = m;
		l->currPointerState=NO_MOVES_TO_REDO_STATE;
		return;
	}
	m->prev=l->last;
	l->last->next = m;
	l->last= m;

}
void promoteCurrPointer(movesList *l){
	if(l->curr==l->last){
		l->currPointerState=NO_MOVES_TO_REDO_STATE;
		return;
	}
	l->curr = l->curr->next;
}
void demoteCurrPointer(movesList *l){
	if(l->curr==l->first){
		l->currPointerState=NO_MOVES_TO_UNDO_STATE;
		return;
	}
	l->curr = l->curr->prev;
}
void printMovesList(movesList *l){
	moveNode *currMove= l->first;
	while(currMove!=NULL){
		printCommandName(currMove->command);
		printf("\t");
		currMove=currMove->next;
	}
}
