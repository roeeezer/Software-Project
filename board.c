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
    res->emptyCellsCounter=(res->squareSideSize)*(res->squareSideSize);
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
	int s,prevVal;
    s= Pboard->squareSideSize;
	prevVal = Pboard->boardArr[i*s+j];
	Pboard->boardArr[i*s+j]=val;
	if(val==0){
		if(prevVal!=0){
			Pboard->emptyCellsCounter++;

		}
	}
	else{
		if(prevVal==0){
			Pboard->emptyCellsCounter--;
		}
	}
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

void findCellBlockIndices(board* b,int i,int j,int* blockIndices){
	int rowsOfBlocks,columnsOfBlocks;
	rowsOfBlocks = b->columns;
	columnsOfBlocks = b->rows;
	blockIndices[0] = i/rowsOfBlocks;
	blockIndices[1] = j/columnsOfBlocks;
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

	else if(type==ERRONEOUS){
		printf("%d*",val);

			}
	if(val<10){
		printf(" ");}
}
int boardHasASolution(board* b){
	/*TODO: Omer return true if it has a solution*/
	printf("",b);
	return 1;
}
int erroneousBoard(board* bTypes){
	int i,j,s = bTypes->squareSideSize;
	for(i=0;i<s;i++){
		for(j=0;j<s;j++){
			if(getCell(bTypes,i,j)==ERRONEOUS){
				return 1;
			}
		}
	}
	return 0;
}
int setCausesErroneousCellInRow(board* b,board* bTypes,int i,int v,int ind){
	int boardColumns,k,res=0;
	boardColumns = b->squareSideSize;
	for(k=0;k<boardColumns;k++){
		if(getCell(b,i,k)==v){
			if(ind==2&&getCell(bTypes,i,k)==FIXED){
				return 1;
			}
			if(ind==0){
				return 1;
			}
			if(ind==1){
				res=1;
				setCell(bTypes,i,k,ERRONEOUS);}

		}
	}
	return res;

}
int setCausesErroneousCellInColumn(board* b,board* bTypes,int j,int v,int ind){
	int boardRows,k,res=0;
	boardRows = b->squareSideSize;
		for(k=0;k<boardRows;k++){
			if(getCell(b,k,j)==v){
				if(ind==2&&getCell(bTypes,k,j)==FIXED){
					return 1;
				}
				if(ind==0){
					return 1;
				}
				if(ind==1){
					res=1;
					setCell(bTypes,k,j,ERRONEOUS);}
			}
		}
		return res;

}
int setCausesErroneousCellInBlock(board* b,board* bTypes,int i,int j,int v,int ind){
	int blockIndices[2],k,r,res=0;

	findCellBlockIndices(b,i,j,blockIndices);

	for(k=blockIndices[0]*b->rows;k<(blockIndices[0]+1)*b->rows;k++){
		for(r=blockIndices[1]*b->columns;r<(blockIndices[1]+1)*b->columns;r++){
			if(getCell(b,k,r)==v){
				if(ind==2&&getCell(bTypes,k,r)==FIXED){
					return 1;
				}
				if(ind==0){
					return 1;
				}
				if(ind==1){
					res=1;
					setCell(bTypes,k,r,ERRONEOUS);}
					}
		}
	}
	return res;
}

/*ind=0 for checking if this set command causes erroneous cells
 * ind=1 for checking if this set command causes erroneous cells and marking the erroneous cells in bTypes
 * ind=2 if we want to set a fixed cell to (i,j) and to check if the set will cause a collision with
 * other fixed cells - used in function boardContainsFixedErroneousCells
 * @pre: cell(i,j) is empty, namely =0
 * @POST: $RET=1 iff the answer to the question determined by ind is YES*/

int setCausesErroneousCell(board* b,board* bTypes,int i,int j,int v,int ind){
	int b1,b2,b3,res;
	b1 = setCausesErroneousCellInRow(b,bTypes,i,v,ind);
	if(b1&&!ind){
		return 1;
	}
	b2 = setCausesErroneousCellInColumn(b,bTypes,j,v,ind);
	if(b2&&!ind){
		return 1;
	}
	b3 = setCausesErroneousCellInBlock(b,bTypes,i,j,v,ind);
	if(b3&&!ind){
		return 1;
	}
	res= b1||b2||b3;
	return res;
}
int validAsignment(board* b,int v,int i,int j){
	return !setCausesErroneousCell( b,NULL,i, j, v, 0);

}
void setCellAndMarkErroneous(board* b,board* bTypes,int i,int j,int val){
	int erroneous;
	erroneous=setCausesErroneousCell( b,bTypes,i,j,val,1);
	setCell(b,i,j,val);
	if(erroneous){
		setCell(bTypes,i,j,ERRONEOUS);
	}
}

void setCellAndUpdate(board* b,board* bTypes,moveNode* move,int i,int j,int val){
	int erroneous;
	printf("",move);
	erroneous=setCausesErroneousCell( b,bTypes,i,j,val,1);
	setCell(b,i,j,val);
	if(erroneous){
		setCell(bTypes,i,j,ERRONEOUS);
	}
}
int boardContainsFixedErroneousCells(board *b,board *bTypes){
	int N,i,j,v;
	N = b->squareSideSize;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(getCell(bTypes,i,j)==FIXED){
				v=getCell(b,i,j);
				setCell(b,i,j,0);
				if(setCausesErroneousCell(b,bTypes, i, j, v, 2)){
					return 1;
				}
				setCell(b,i,j,v);

			}
		}
	}

	return 0;
}
void markAllErroneousCellsInBoard(board* b,board* bt){
	int i,j,v,erroneous,N=b->squareSideSize;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(getCell(bt,i,j)==REGULAR){
				v = getCell(b,i,j);
				setCell(b,i,j,0);
				erroneous=setCausesErroneousCell( b,bt,i,j,v,1);
				setCell(b,i,j,v);
				if(erroneous){
					setCell(bt,i,j,ERRONEOUS);
				}
			}

		}}
}


