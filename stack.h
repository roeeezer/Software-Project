
/*
 * stack.h
 *
 *  Created on: 3 баев„ 2019
 *      Author: roee
 */

#ifndef STACK_H_
#define STACK_H_
#include "stackNode.h"
typedef struct{
	stackNode* topNode;


}stack;
stack* createStack(int cellIndex,int fromVal);
void push(stack* s,int cellIndex,int fromVal);
int emptyStack(stack* s);
stackNode* top(stack* s);
void deleteTop(stack* s);
void destroyStack(stack* Pstack);
#endif /* STACK_H_ */
