/*
 * changesList.c
 *
 */
#include "changesList.h"

changesList* createChangesList(){
	changesList* res = (changesList*)malloc(sizeof(changesList));
	res->first = NULL;
	return res;
}

int emptyChangesList(changesList* list){
	return list->first==NULL;
}
void destroyChangesList(changesList* l){
	if(l==NULL){
		return;
	}
	if(!emptyChangesList(l)){
		destroyAllChangeNodesStartingFrom(l->first);}
	free(l);
}
/*Inserts "change" at the beginning of the changesList "list"*/
void InsertFirst(changesList* list,changeNode* change){
	if(emptyChangesList(list)){
		list->first=change;
		return;
	}
	change->next = list->first;
	list->first = change;

}
/*this function is called when the user call for undo/redo command
 * and we want to print all the changes of the relevant move*/
void printChangesList(changesList* list,int ind){
	if(emptyChangesList(list)){

		 /* this might happen in autofill move that didn't fill any cells  */
		return;
	}
	printChangesStartingFrom(list->first,ind);

}


