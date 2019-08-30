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
void destroyMovesList(){
	/*TODO*/

}
int EmptyList(movesList *l){
	return l->first==NULL;
}
void addMove(movesList *l,moveNode *m){
	if(EmptyList(l)){
		l->first = m;
		l->curr = m;
		l->last = m;
		return;
	}
	m->prev=l->last;
	l->last->next = m;

}
void promoteCurrPointer(movesList *l){
	l->curr = l->curr->next;
}
