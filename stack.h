
/*
 *      Author: roee
 * stack.h
 * Implementation of the stack data structure that replaces the recursion in
 * the exhaustive backtracking process.
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
