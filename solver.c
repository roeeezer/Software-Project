/*
 * solver.c
 *
 *      Author: roee
 */
#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

int DeterministicBackTracingRec(board* b,board* bSol,int startInd){
	int s,cellsInBoard,v,nextEmptyCell;
	int indices[2];
	s=b->squareSideSize;
	cellsInBoard=s*s;
	if(startInd==cellsInBoard){
		/*the entire board was solved successfully therefore the recursion
		 * call called a cell which is beyond the last cell(cell[81] in a regular
		 * 3X3 sudoku*/
		return 1;
	}
	oneDto2Dindices(b,indices,startInd);
	nextEmptyCell=findNextEmptyCell(b,startInd+1);
	for(v=1;v<=s;v++){
		/*squareSideSize is the maximal possible value of a single digit in the board*/
		if(validAsignment(bSol,v,indices[0],indices[1])){
			setCell(bSol,indices[0],indices[1],v);
			if(DeterministicBackTracingRec(b,bSol,nextEmptyCell)){
				return 1;
			}
		}
		setCell(bSol,indices[0],indices[1],0);
	}
	return 0;
}
int exhaustiveBackTracingWithStack(board* b,board* bSol){
	int s,cellsInBoard,currInd,nextEmptyInd,returnVal=0,addReturnVal,skipToNextStackNode=0;
	int indices[2];stack* Pstack;stackNode* currNode;
	s=b->squareSideSize;
	cellsInBoard=s*s;
	currInd = findNextEmptyCell(b,0);
	Pstack = createStack(currInd,0);
	while(!emptyStack(Pstack)){
		skipToNextStackNode=0;
		currNode = top(Pstack);
		currInd = currNode->cellIndex;
		if(addReturnVal&&!skipToNextStackNode){
			currNode->counter+=returnVal;
			addReturnVal=0;
			}
		if(currInd==cellsInBoard&&!skipToNextStackNode){
			addReturnVal=1;
			returnVal= 1;
			deleteTop(Pstack);
			skipToNextStackNode=1;}
		oneDto2Dindices(b,indices,currInd);
		nextEmptyInd=findNextEmptyCell(b,currInd+1);
		for(;currNode->fromVal<=s&&!skipToNextStackNode;currNode->fromVal++){
		if(validAsignment(bSol,currNode->fromVal,indices[0],indices[1])){
			setCell(bSol,indices[0],indices[1],currNode->fromVal);
			push(Pstack,nextEmptyInd,0);
			skipToNextStackNode=1;
	}
		}
		if(currNode->fromVal>= s && !skipToNextStackNode){
			setCell(bSol,indices[0],indices[1],0);
			addReturnVal=1;
			returnVal= currNode->counter;
			deleteTop(Pstack);
			skipToNextStackNode=1;}
		}
	/*tmp*/
	if(!addReturnVal){
		printf("error");
	}
	destroyStack(Pstack);
	return returnVal;
}
/*this is temporary just to compare with exhaustiveBackTracingWithStack */
		int exhaustiveBackTracingRec(board* b,board* bSol,int startInd){
			int s,cellsInBoard,v,nextEmptyCell,counter=0;
			int indices[2];
			s=b->squareSideSize;
			cellsInBoard=s*s;
			if(startInd==cellsInBoard){
				/*the entire board was solved successfully therefore the recursion
				 * call called a cell which is beyond the last cell(cell[81] in a regular
				 * 3X3 sudoku*/
				/*printf("startInd=%d \t res=%d\n",startInd,1);*/
				return 1;
			}
			oneDto2Dindices(b,indices,startInd);
			nextEmptyCell=findNextEmptyCell(b,startInd+1);
			for(v=1;v<=s;v++){
				/*squareSideSize is the maximal possible value of a single digit in the board*/
				if(validAsignment(bSol,v,indices[0],indices[1])){
					setCell(bSol,indices[0],indices[1],v);
					counter+= exhaustiveBackTracingRec(b,bSol,nextEmptyCell);

				}

			}
			setCell(bSol,indices[0],indices[1],0);
			/*printf("startInd=%d \t counter=%d\n",startInd,counter);*/
			return counter;
		}
int createValidValuesList(int* valuesList,board* bSol,int i,int j){
	int v,curr;
	curr=0;
	for(v=1;v<=bSol->squareSideSize;v++){
		/*squareSideSize is the maximal possible value of a single digit in the board*/
		if(validAsignment(bSol,v,i,j)){
			valuesList[curr]=v;
			curr++;
		}

	}
	return curr;

}
void deleteIndexFromList(int* valuesList,int ind,int size){
	int i;
	for(i=ind;i<size-1;i++){
		valuesList[i]=valuesList[i+1];
	}


}
int RandomBackTracingRec(board* b,board* bSol,int startInd){
		int s,cellsInBoard,v,nextEmptyCell,listSize,r;
		int indices[2],* valuesList;
		valuesList = malloc(sizeof(int)*b->squareSideSize);
		if (valuesList == NULL){
            printf("Error: malloc has failed\n");
            exit(-42);
		}
		/*squareSideSize is the maximal possible value of a single digit in the board*/
		s=b->squareSideSize;
		cellsInBoard=s*s;
		if(startInd==cellsInBoard){
			/*the entire board was solved successfully therefore the recursion
			 * call called a cell which is beyond the last cell(cell[81] in a regular
			 * 3X3 soduko*/
			free(valuesList);
			return 1;
		}
		oneDto2Dindices(b,indices,startInd);
		nextEmptyCell=findNextEmptyCell(b,startInd+1);
		listSize=createValidValuesList(valuesList,bSol,indices[0],indices[1]);
		while(0<listSize){
			if(listSize==1){
				r=0;
			}
			else{
				r = rand()%listSize;
				}
			v = valuesList[r];
			setCell(bSol,indices[0],indices[1],v);
			if(RandomBackTracingRec(b,bSol,nextEmptyCell)){
				free(valuesList);

				return 1;
			}

			setCell(bSol,indices[0],indices[1],0);
			deleteIndexFromList(valuesList,r,listSize);
			listSize--;
		}
		free(valuesList);
		return 0;
}

int findDeterministicSolution(board* Pboard,board* PboardSol){
	int firstEmptyCell;
	board* pSol = createBoard(Pboard->rows, Pboard->columns);
	copyBoard(pSol,Pboard);
	firstEmptyCell = findNextEmptyCell(Pboard,0);
	if (DeterministicBackTracingRec(Pboard,pSol,firstEmptyCell)){
        copyBoard(PboardSol, pSol);
        destroyBoard(pSol);
        return 1;
	}
    destroyBoard(pSol);
	return 0;
}
/*
int findNumberOfSolutions(board* Pboard,board* PboardSol){
	int res;
	board* pSol = createBoard(Pboard->rows, Pboard->columns);
	copyBoard(pSol,Pboard);
	res=exhaustiveBackTracingWithStack(Pboard,pSol);

    destroyBoard(pSol);
    return res;

}
*/
int findRandomSolution(board* Pboard,board* PboardSol){
	int firstEmptyCell;
	copyBoard(PboardSol,Pboard);
	firstEmptyCell = findNextEmptyCell(Pboard,0);
	return RandomBackTracingRec(Pboard,PboardSol,firstEmptyCell);
}
