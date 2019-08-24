/*
 * stackNode.c
 *
 *  Created on: 23 баев„ 2019
 *      Author: roee
 */
#include "stackNode.h"
#include <stdlib.h>
stackNode* createStackNode(int cellIndex,int fromVal){
	stackNode* res;
	res = (stackNode*) malloc(sizeof(stackNode));
	res->cellIndex=cellIndex;
	res->counter=0;
	res->fromVal=fromVal;
	res->lower=NULL;
	return res;
}
void destroyStackNode(stackNode* node){
	free(node);
}
void destroyAllLowerNodes(stackNode* node){
	if(node == NULL){
		return;
	}
	if(node->lower==NULL){
		destroyStackNode(node);
		return;
	}
	destroyAllLowerNodes(node->lower);
	destroyStackNode(node);

}
