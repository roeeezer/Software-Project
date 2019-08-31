/*
 *
 *      Author: roee
 */
#include <stdio.h>
#include <stdlib.h>
#include "movesList.h"
movesList* createMovesList(){
	movesList* res = (movesList*) malloc(sizeof(movesList));
	res->first=NULL;
	res->curr=NULL;
	res->last=NULL;

	return res;

}

int emptyMovesList(movesList *l){
	return l->first==NULL;
}
void destroyMovesList(movesList* l){
	if(!emptyMovesList(l)){
		destroyAllMoveNodesStartingFrom(l->first);}
	free(l);
}
void addMove(movesList *l,moveNode *m){
	if(emptyMovesList(l)){
		l->first = m;
		l->curr = m;
		l->last = m;
		return;
	}
	m->prev=l->last;
	l->last->next = m;

}
void promoteCurrPointer(movesList *l){
	if(l->curr->next==NULL){
		return;
	}
	l->curr = l->curr->next;
}
void demoteCurrPointer(movesList *l){
	if(l->curr->prev==NULL){
		return;
	}
	l->curr = l->curr->prev;
}
