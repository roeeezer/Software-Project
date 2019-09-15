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
void InsertFirst(changesList* list,changeNode* change){
	if(emptyChangesList(list)){
		list->first=change;
		return;
	}
	change->next = list->first;
	list->first = change;

}
void printChangesList(changesList* list,int ind){
	if(emptyChangesList(list)){
		/*printf("No changes were made in this move\n");
		 * this might happen only in autofill move that didnt fill any cells  */
		return;
	}
	printChangesStartingFrom(list->first,ind);

}


