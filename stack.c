/*
 * stack.c
 *
 *  Created on: 3 баев„ 2019
 *      Author: roee
 */
#include "stack.h"
#include <stdlib.h>
stack* createStack(int cellIndex,int fromVal){
	stack* res;
	res = (stack*) malloc(sizeof(stack));
	res->topNode = createStackNode(cellIndex,fromVal);
	return res;

}
void destroyStack(stack* Pstack){
	destroyAllLowerNodes(Pstack->topNode);
	free(Pstack);

}
void push(stack* s,int cellIndex,int fromVal){
	stackNode* newTop = createStackNode(cellIndex,fromVal);
	newTop->lower = s->topNode;
	s->topNode = newTop;
}
int emptyStack(stack* s){
	if(s->topNode==NULL){
		return 1;
	}
	return 0;
}
stackNode* top(stack* s){
	return s->topNode;
}
void deleteTop(stack* s){
	stackNode* prevTop;
	if(emptyStack(s)){
		return;
	}
	prevTop = s->topNode;
	s->topNode = prevTop->lower;
	destroyStackNode(prevTop);
}
