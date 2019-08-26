/*
 * board.c
 *
 *      Author: roee
 */
#include <stdlib.h>
#include "board.h"

board* createBoard(int blockRows,int blockColumns){
	board* res;
	res= (board*) malloc(sizeof(board));
	if (res == NULL){
        printf("Error: malloc has failed\n");
        exit(-42);
	}
	res->rows = blockRows;
	res->columns = blockColumns;
	res->squareSideSize= (res->rows)*(res->columns);
	res->boardArr = malloc((res->squareSideSize*res->squareSideSize)*sizeof(int));
	if (res->boardArr == NULL){
        printf("Error: malloc has failed\n");
        exit(-42);
	}
    resetBoard(res, 0);
	return res;
}
void destroyBoard(board* board){
	free(board->boardArr);

	free(board);

}

int getCell(board* Pboard,int i,int j){
	int s;
	s= Pboard->squareSideSize;
	return Pboard->boardArr[i*s+j];
}
void setCell(board* Pboard,int i,int j,int val){
	int s;
    s= Pboard->squareSideSize;
	Pboard->boardArr[i*s+j]=val;
}
int emptyCell(board* Pboard,int i,int j){
	return getCell(Pboard, i, j)==0;
}
void resetBoard(board* b,int v){
	int s,i,j;
	s = b->squareSideSize;
	for(i=0;i<s;i++){
		for(j=0;j<s;j++){

			setCell(b,i,j,v);
		}
	}
}

int boardSolved(board * pBoard){
    int s = pBoard->squareSideSize;
    s = s * s;
    return (findNextEmptyCell(pBoard, 0) == s);
}
int findNextEmptyCell(board* b,int fromInd){
	int s,cellsInBoard,i;
	int indices[2];
	s=b->squareSideSize;
	cellsInBoard=s*s;
	for(i=fromInd;i<cellsInBoard;i++){
		oneDto2Dindices(b,indices,i);
		if(emptyCell(b,indices[0],indices[1])){
			return i;
		}
	}
	return i;
}
int validAsignmentRow(board* b,int v,int i){
	int boardColumns,k;
	boardColumns = b->squareSideSize;
	for(k=0;k<boardColumns;k++){
		if(getCell(b,i,k)==v){
			return 0;
		}
	}
	return 1;

}
int validAsignmentColumn(board* b,int v,int j){
	int boardRows,k;
	boardRows = b->squareSideSize;
		for(k=0;k<boardRows;k++){
			if(getCell(b,k,j)==v){
				return 0;
			}
		}
		return 1;

}
void findCellBlockIndices(board* b,int i,int j,int* blockIndices){
	int rowsOfBlocks,columnsOfBlocks;
	rowsOfBlocks = b->columns;
	columnsOfBlocks = b->rows;
	blockIndices[0] = i/rowsOfBlocks;
	blockIndices[1] = j/columnsOfBlocks;
}
int validAsignmentBlock(board* b,int v,int i,int j){
	int blockIndices[2],k,r;

	findCellBlockIndices(b,i,j,blockIndices);

	for(k=blockIndices[0]*b->rows;k<(blockIndices[0]+1)*b->rows;k++){
		for(r=blockIndices[1]*b->columns;r<(blockIndices[1]+1)*b->columns;r++){
			if(getCell(b,k,r)==v){
				return 0;
					}
		}
	}
	return 1;
}
int validAsignment(board* b,int v,int i,int j){
	return validAsignmentRow(b,v,i)&&validAsignmentColumn(b,v,j)&&validAsignmentBlock(b,v,i,j);

}


void oneDto2Dindices(board* b,int* indices,int oneDind){
	int s;
	s= b->squareSideSize;
	indices[0]=oneDind/s;
	indices[1]=oneDind%s;

}
void copyBoard(board* copy,board* orig){
	int s,cellsInBoard,i,v;
	int indices[2];
	if(copy->rows!=orig->rows||copy->columns!=orig->columns){
		printf("unmatched boards in copyBoard(board* copy,board* orig)");
		exit(0);
	}
	s = copy->squareSideSize;
	cellsInBoard=s*s;
	for(i=0;i<cellsInBoard;i++){
		oneDto2Dindices(orig,indices,i);
		v=getCell(orig,indices[0],indices[1]);
		setCell(copy,indices[0],indices[1],v);
	}

}
void printDashRow(board* b){
	int amount = 4*b->squareSideSize+b->rows+1,i;
	for(i=0;i<amount;i++){
		printf("-");
	}
	printf("\n");

}
int printBoard(board* b,board* bTypes){
	int blockRow,rowsOfBlocksInTheBoard;
	rowsOfBlocksInTheBoard=b->columns;
	printDashRow(b);
	for(blockRow=0;blockRow<rowsOfBlocksInTheBoard;blockRow++){
		printRowOfBlocks(b,blockRow,bTypes);
		printDashRow(b);
	}
	return 1;
}
void printRowOfBlocks(board* b,int blockRow,board* bTypes){
	int boardRow;
	for(boardRow=0;boardRow<b->rows;boardRow++){
		printOneRowInRowsOfBlocks(b,blockRow,boardRow,bTypes);
	}


}
void printOneRowInRowsOfBlocks(board* b,int blocksRow,int boardRow,board* bTypes){
	int blockComlumnInSingleRow,columnsOfBlocksInBoard;
	columnsOfBlocksInBoard=b->rows;
	printf("|");
	for(blockComlumnInSingleRow=0;blockComlumnInSingleRow<columnsOfBlocksInBoard;blockComlumnInSingleRow++){
		printSingleRowInSingleBlock(b,blocksRow,boardRow,blockComlumnInSingleRow,bTypes);
		/*printf(" |");*/
		printf("|");
	}
	printf("\n");
}
void printSingleRowInSingleBlock(board* b,int blocksRow,int boardRow,
		int blockComlumnInSingleRow,board* bTypes){
	int i,j,blockRowLength,blockColumnLength,firstColumn;
	blockRowLength = b->rows;
	blockColumnLength = b->columns;
	i=blockRowLength*blocksRow+boardRow;
	firstColumn=blockComlumnInSingleRow*blockColumnLength;
	for(j=firstColumn;j<firstColumn+b->columns;j++){
		/*printf(" ");*/
		printCellValue(b,i,j,bTypes);
	}

}
void printCellValue(board* b,int i,int j,board* bTypes){
	int val,type;
	val = getCell(b,i,j);
	type = getCell(bTypes,i,j);
	printf(" ");
	if(val==0){
			printf("  ");
		}
	else if(type==REGULAR){
		printf(" %d",val);
	}

	else if(type==FIXED){
		printf("%d.",val);

		}
	else if(type==ERR){
		printf("%d*",val);

			}
	printf(" ");
}
int boardIsValid(board* b){
	/*TODO: Omer return true if it has a solution*/
	printf("%d",b);
	return 1;
}
int erroneousBoard(board* bTypes){
	int i,j,s = bTypes->squareSideSize;
	for(i=0;i<s;i++){
		for(j=0;j<s;j++){
			if(getCell(bTypes,i,j)==ERR){
				return 1;
			}
		}
	}
	return 0;
}

