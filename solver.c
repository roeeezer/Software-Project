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
		 * 3X3 soduko*/
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
int findRandomSolution(board* Pboard,board* PboardSol){
	int firstEmptyCell;
	copyBoard(PboardSol,Pboard);
	firstEmptyCell = findNextEmptyCell(Pboard,0);
	return RandomBackTracingRec(Pboard,PboardSol,firstEmptyCell);
}
