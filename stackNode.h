/*
 * stackNode.h
 *
 *
 *      the stack is implemented as a linked list of stackNodes that
 *      contains all the information needed for the recursive calls
 *      simulation
 */

#ifndef STACKNODE_H_
#define STACKNODE_H_

typedef struct stackNode{
	int cellIndex;
	int counter;
	int fromVal;
	struct stackNode* lower;


}stackNode;
stackNode* createStackNode(int cellIndex,int fromVal);
void destroyStackNode(stackNode* node);
void destroyAllLowerNodes(stackNode* node);
#endif /* STACKNODE_H_ */
