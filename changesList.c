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
void printChangesList(changesList* list){
	if(emptyChangesList(list)){
		printf("No changes were made\n");
		return;
	}
	printChangesStartingFrom(list->first);

}

