/*
 * solver.c
 *
 *      Author: roee
 */
#include "board.h"
#include <stdio.h>
#include <stdlib.h>
#include "solver.h"

int chooseWithThreshold(double *scoresArr, int *valuesArr, double threshold, int len);

ERROR solveLP(board *pBoard, VAR **resultVars, double **solValues, int *varCount);

/**
 * Copy board and solve ILP on it.
 * Copies back to original board afterwards
 * Autofills board first to prevent wastefulness
 * @param pBoard
 * @return appropriate error
 */
ERROR solveILP(board* pBoard){
    board* cpBoard;
    ERROR error;
    cpBoard = createBoard(pBoard->rows, pBoard->columns);
    copyBoard(cpBoard, pBoard);
    simpleAutofill(cpBoard);

    error = setUpGurobi(cpBoard, 1, NULL, NULL, NULL);
    if (error == NO_ERROR)
        copyBoard(pBoard, cpBoard);
    else if (DEBUG) /*TODO debugPrint*/
        printf("Error in solveILP\n");
    destroyBoard(cpBoard);
    return error;
}

/**
 *
 * @param pBoard game board. Read-only
 * @param i, 0-indexed
 * @param j, 0-indexed
 * @param cellValues, preallocated array for possible values of cell.
 * @param cellScores pre-allocated array for scores of values.
 * @return
 */
ERROR solveLPForTargetCell(board *pBoard, int i, int j, int *cellValues, double *cellScores, int *numOfValuesInCell) {
    /*TODO @Omer go over this*/
    board* cpBoard;
    int counter, varCount, index;
    ERROR error;
    VAR * resultVars;
    double * solValues;
    resultVars = NULL;
    solValues = NULL;
    error = solveLP(pBoard, &resultVars, &solValues, &varCount);
    if (error != NO_ERROR){
        free(resultVars);
        free(solValues);
        return error;
    }
    cpBoard = createBoard(pBoard->rows, pBoard->columns);
    copyBoard(cpBoard, pBoard);
    simpleAutofill(cpBoard);
    if (DEBUG && (resultVars == NULL || solValues == NULL))
        if (DEBUG) printf("Your arrays are NULL!\n"); /*TODO debugPrint*/
    *numOfValuesInCell = 0;
    index = 0;
    for (counter = 0; counter < varCount; counter++) {
        if ((resultVars + counter)->row == i && (resultVars + counter)->col == j) {
            cellScores[index] = solValues[counter];
            cellValues[index] = (resultVars + counter)->val;
            index++;
            (*numOfValuesInCell) += 1;
        }
    }
    destroyBoard(cpBoard);
    return NO_ERROR; /*TODO change this func not done*/
}

/**
 * finds LP solution, while only considering result values above a certain threshold.
 * Writes solution to the board. resultVars and solValues will be allocated in solveLP.
 * @param pBoard the board to solve
 * @param threshold double between 0 and 1 for which values to keep
 * @return appropriate ERROR
 */
ERROR solveLPWithThreshold(board *pBoard, double threshold){
    ERROR error;
    VAR * resultVars;
    double * solValues;
    int varCount;
    double *cellScores;
    board *cpBoard;
    int N, i, j, index, row, col, numOfValuesInCell;
    int *cellValues;
    int resultForCell;
    error = solveLP(pBoard, &resultVars, &solValues, &varCount);
    if (error != NO_ERROR){
        free(resultVars);
        free(solValues);
        return error;
    }
    cpBoard = createBoard(pBoard->rows, pBoard->columns);
    copyBoard(cpBoard, pBoard);
    N = pBoard->squareSideSize;
    cellScores = (double *) malloc(N * sizeof(double));
    if (cellScores == NULL)
        return MALLOC_ERROR;
    cellValues = (int *) malloc(N * sizeof(double));
    if (cellValues == NULL)
        return MALLOC_ERROR;
    if (DEBUG && (resultVars == NULL || solValues == NULL))
        if (DEBUG) printf("Your arrays are NULL!\n"); /*TODO debugPrint*/
    for (i = 0; i < varCount;) {
        numOfValuesInCell = 0;
        index = 0;
        row = (resultVars + i)->row;
        col = (resultVars + i)->col;
        for (;i < varCount && (resultVars + i)->row == row && (resultVars + i)->col == col; i++) { /*Since variables are in order of row and col*/
            cellScores[index] = solValues[i];
            cellValues[index] = (resultVars + i)->val;
            index++;
            numOfValuesInCell++;
        }
        for (j = 0; j < numOfValuesInCell; j++) {
            if (!validAsignment(cpBoard, cellValues[j], row, col))
                cellScores[j] = 0;
        }
       resultForCell = chooseWithThreshold(cellScores, cellValues, threshold, numOfValuesInCell); /*Only valid cells*/
       setCell(cpBoard, row, col, resultForCell); /*Will not be erroneous because of validAssignment check 4 rows above*/
    }
    copyBoard(pBoard,cpBoard);
    free(resultVars);
    free(solValues);
    free(cellScores);
    free(cellValues);
    destroyBoard(cpBoard);
    return NO_ERROR;
}
/**
 * The internal method, used for solving LP, filling in the appropriate arrays as necessary
 * the arrays should NOT be pre-allocated. setUpGurobi does allocation.
 * @param pBoard the board to solve
 * @param resultVars pointer to the array of resulting VARs for the problem
 * @param solValues pointer to the values of the VARs in the optimal solution
 * @param varCount the length of the above 2 arrays
 * @return appropriate error.
 */
ERROR solveLP(board *pBoard, VAR **resultVars, double **solValues, int *varCount) {
    ERROR error;
    board* cpBoard;
    (*resultVars) = NULL;
    (*solValues) = NULL;
    cpBoard = createBoard(pBoard->rows, pBoard->columns);
    copyBoard(cpBoard, pBoard);
    simpleAutofill(cpBoard);
    error = setUpGurobi(cpBoard, 0, resultVars, solValues, varCount);
    destroyBoard(cpBoard);
    return error;
}

/**
 *  function for choosing from weighted random. The score of the value at valuesArr[i] is scoresArr[i]
 * @param scoresArr the scores of each value (by index, not by actual value)
 * @param valuesArr array of possible cell values
 * @param threshold for choice, don't choose cells of score< threshold
 * @param len length of the arrays
 * @return the value to set for the cell. Algorithm's correctness implies that a cell with score 0 will never be chosen
 */
int chooseWithThreshold(double *scoresArr, int *valuesArr, double threshold, int len) {
    int resIndex, i;
    double sum, randChoice;
    sum=0;
    resIndex=-1;
    for (i = 0; i < len; i++) {
        scoresArr[i] = scoresArr[i] * (scoresArr[i] >= threshold);
    }
    for (i = 0; i < len; i++) {
        sum += scoresArr[i]; /*Sum used for normalizing*/
    }
    if (sum == 0.0){
        return 0; /*no value is over threshold, cell remains empty*/
    }
    for (i = 0; i < len; i++) {
        scoresArr[i] = scoresArr[i] / sum; /*Normalize total sum of scores to 1*/
    }
    randChoice = (double)rand() / RAND_MAX;
    for (i = 0; i < len; i++) {
        /** cumulative sum weighted random choice algorithm:
         * when the cumulative sum surpasses randChoice we return this value*/
        randChoice -= scoresArr[i];
        if (randChoice <= 0) {
            resIndex = i;
            break;
        }
    }
    if ((resIndex == -1) && DEBUG) printf("Problem with chooseWithThreshold!\n"); /*TODO debugPrint*/
    return valuesArr[resIndex];
}

ERROR simpleAutofill(board *pBoard){
    int i,j, N, size;
    int * valuesList;
    valuesList = malloc(sizeof(int) * pBoard->squareSideSize);
    if (valuesList == NULL){
        return MALLOC_ERROR;
    }
    N = pBoard->squareSideSize;
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            if(getCell(pBoard,i,j)==0){
                size=createValidValuesList(valuesList,pBoard, i, j);
                if(size==1){
                    setCell(pBoard,i,j,valuesList[0]);
                }
            }
        }
    }
    free(valuesList);
    return NO_ERROR;
}
int exhaustiveBackTracingWithStack(board* b,board* tmpCopy){
	int s,cellsInBoard,currInd,nextEmptyInd,returnVal=0,addReturnVal=0,skipToNextStackNode=0;
	int indices[2];stack* Pstack;stackNode* currNode;
	s=b->squareSideSize;
	cellsInBoard=s*s;
	currInd = findNextEmptyCell(b,0);
	Pstack = createStack(currInd,1);
	while(!emptyStack(Pstack)){
        skipToNextStackNode=0;/*when skipToNextStackNode==1 the program will skip to the next while loop iteration*/
		currNode = top(Pstack);
		currInd = currNode->cellIndex;
        if (!skipToNextStackNode && addReturnVal) {
            /*simulates adding the value from the last recursive call to the counter*/
            currNode->counter += returnVal;
            addReturnVal = 0;
        }
		if(!skipToNextStackNode && currInd==cellsInBoard){
			/*the stopping condition in the recursion*/
			addReturnVal=1;
			returnVal= 1;
			deleteTop(Pstack);
			skipToNextStackNode=1;}
		oneDto2Dindices(b,indices,currInd);
		nextEmptyInd=findNextEmptyCell(b,currInd+1);
		for(;!skipToNextStackNode && currNode->fromVal<=s;currNode->fromVal++){
            if(validAsignment(tmpCopy,currNode->fromVal,indices[0],indices[1])){
                setCell(tmpCopy,indices[0],indices[1],currNode->fromVal);
                /*simulates a recursive call*/
                push(Pstack,nextEmptyInd,1);
                skipToNextStackNode=1;
            }
		}
		if(!skipToNextStackNode && currNode->fromVal>= s){
			setCell(tmpCopy,indices[0],indices[1],0);
			addReturnVal=1;
			returnVal= currNode->counter;
			deleteTop(Pstack);
			skipToNextStackNode=1;}
		}
	/*tmp*/
	if(!addReturnVal){
		printf("Error in exhaustive backtracking");
	}
	destroyStack(Pstack);
	return returnVal;
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

ERROR autofillBoard(board* b,board* bt,moveNode* move,int gameMode,int printInd){
	int* valuesList;
	int i,j,size,v,N=b->squareSideSize;
	board* cellsToFillBoard;

	cellsToFillBoard = createBoard(b->rows,b->columns);
	resetBoard(cellsToFillBoard,0);
	valuesList = malloc(sizeof(int)*b->squareSideSize);
	if (valuesList == NULL){
		return MALLOC_ERROR;

	}
	/*goal: cellsToFillBoard(i,j)=v iff the only possible value for b(i,j) is v
	 * if b(i,j) has multiple possible values then cellsToFillBoard(i,j)=0*/
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(getCell(b,i,j)==0){
				size=createValidValuesList(valuesList,b, i, j);
				if(size==1){
					setCell(cellsToFillBoard,i,j,valuesList[0]);
			}
		}
		}
	}
	/*now we shall set the cells with "obvious" values to the board b*/
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			v=getCell(cellsToFillBoard,i,j);
			if(v!=0){

				setCellUpdateErroneousAndMove(b,bt,move, i, j,v,gameMode,printInd);

			}
		}}
	free(valuesList);
	return NO_ERROR;


}

/**
 * Fills X empty cells in board with valid values. Assumes there ARE at least x empty cells
 * @param pBoard the board
 * @param x how many cells to fill
 * @return 1 if succeeded, 0 if at any point a cell had no valid options
 */
int fillXRandomCells(board* pBoard, int x){
    int i, j, counter, N, numValuesForCell, chosenValue;
    int* valuesList;
    N = pBoard->squareSideSize;
    valuesList = (int *) malloc(N * sizeof(int));
    if (valuesList == NULL){
        printf("Error: malloc has failed!\n");
        exit(-42);
    }
    for (counter = 0; counter < x;) {
        i = rand() % N;
        j = rand() % N;
        while (!emptyCell(pBoard, i, j)){
            i = rand() % N;
            j = rand() % N;
        }
        numValuesForCell = createValidValuesList(valuesList, pBoard,i, j);
        if (numValuesForCell == 0){
            free(valuesList);
            return 0;
        }
        chosenValue = valuesList[rand() % numValuesForCell];
        setCell(pBoard, i, j, chosenValue);
        counter++;
    }
    free(valuesList);
    return 1;
}

void clearRandomCell(board* pBoard){
    int i, j, N;
    N = pBoard->squareSideSize;
    while(1){
        i = rand() % N;
        j = rand() % N;
        if (getCell(pBoard, i, j)){
            clearCell(pBoard, i, j);
            return;
        }
    }
}

int boardHasASolution(board* pBoard){
	board * cpBoard;
	ERROR error;
	cpBoard = createBoard(pBoard->rows, pBoard->columns);
	copyBoard(cpBoard, pBoard);
    error = solveILP(cpBoard);
    destroyBoard(cpBoard);
    return error == NO_ERROR;
}
