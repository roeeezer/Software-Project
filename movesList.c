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
int singleNodeList(movesList *l){
	return l->first==l->last;
}
void destroyMovesList(movesList* l){
	if(!emptyMovesList(l)){
		destroyAllMoveNodesStartingFrom(l->first);
	}
	free(l);
}
void addMove(movesList *l,moveNode *m){
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
int nodeIsStartSentinel(movesList *l,moveNode* node){
	return node==l->first&&l->currPointerState==NO_MOVES_TO_UNDO_STATE;
}
int nodeIsEndSentinel(movesList *l,moveNode* node){
	return node==l->last&&l->currPointerState==NO_MOVES_TO_REDO_STATE;
}
void promoteCurrPointer(movesList *l){
	if(nodeIsStartSentinel(l,l->curr)){
		/*In this case we can image the curr pointer as if its pointing to and empty sentinel
		 * before undoList->first therefore we need to redo the undoList->first move*/
		if(singleNodeList(l)){
			l->currPointerState=NO_MOVES_TO_REDO_STATE;
			return;
		}
		else{
			l->currPointerState=STANDART_STATE;
		return;
		}

	}
	if(l->curr==l->last){
		l->currPointerState=NO_MOVES_TO_REDO_STATE;
		return;
	}

	l->curr = l->curr->next;
}
void demoteCurrPointer(movesList *l){
	if(nodeIsEndSentinel(l,l->curr)){
		if(singleNodeList(l)){
			l->currPointerState=NO_MOVES_TO_UNDO_STATE;
			return;
		}
		else{
			l->currPointerState=STANDART_STATE;
			l->curr = l->curr->prev;
			return;

		}
	}
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
		if(currMove==l->first){
			printf("(first)");
		}
		if(currMove==l->curr){
			printf("(curr)");
		}
		if(currMove==l->last){
			printf("(last)");
		}
		if(currMove!=l->last){
		printf("--");}
		currMove=currMove->next;
	}
}
