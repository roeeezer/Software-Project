/*
 * stackNode.h
 *
 *  Created on: 23 баев„ 2019
 *      Author: roee
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
