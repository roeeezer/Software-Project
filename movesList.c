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
/*$ret>0 if and only if the list l is empty*/
int emptyMovesList(movesList *l){
	return l->first==NULL;
}
/*$ret>0 if and only if the list l contains a single node*/
int singleNodeList(movesList *l){
	return l->first==l->last;
}
void destroyMovesList(movesList* l){
	if(!emptyMovesList(l)){
		destroyAllMoveNodesStartingFrom(l->first);
	}
	free(l);
}
/*and the move m to the end of the list l*/
void addMove(movesList *l,moveNode *m){
	if(emptyMovesList(l)){
		l->first = m;
		l->curr = m;
		l->last = m;
		return;
	}
	m->prev=l->last;
	l->last->next = m;
	l->last= m;

}
int nodeIsStartSentinel(movesList *l,moveNode* node){
	return node==l->first&&l->currPointerState==NO_MOVES_TO_UNDO_STATE;
}
void promoteCurrPointer(movesList *l){
	if(nodeIsStartSentinel(l,l->curr)){
		if(singleNodeList(l)){
			l->currPointerState=STANDART_STATE;
			return;
		}
		else{
			l->currPointerState=STANDART_STATE;
		return;
		}

	}
	if(l->curr==l->last){
		return;
	}

	l->curr = l->curr->next;
}
/*important! if the curr node is the first node in the list then the function does not
 * changes the curr pointer - but might change currPointerState*/
void demoteCurrPointer(movesList *l){
	if(l->curr==l->first){
		l->currPointerState=NO_MOVES_TO_UNDO_STATE;
		return;
	}
	l->curr = l->curr->prev;
}
/*TMP: just for debuging*/
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
		if(nodeIsStartSentinel(l,currMove)){
			printf("<startSentinel>");
		}
		if(currMove==l->last){
			printf("(last)");
		}
		if(currMove!=l->last){
		printf("--");}
		currMove=currMove->next;
	}
}
/*deletes all the nodes that comes after "node" in "list"*/
void makeMoveTheLastInTheList(movesList* list,moveNode* node){
	moveNode* startDeletingFrom;
	if(node==list->last&&!nodeIsStartSentinel(list,node)){
		return;
	}
	if(nodeIsStartSentinel(list,node)){
		startDeletingFrom=node;
		list->first=NULL;
		list->curr=NULL;
		list->last=NULL;
	}
	else{
		startDeletingFrom=node->next;}
	node->next=NULL;
	destroyAllMoveNodesStartingFrom(startDeletingFrom);
	list->last=node;

}
