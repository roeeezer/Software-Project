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
	else if(type == REGULAR_CELL){
		printf(" %d",val);
	}

	else if(type == FIXED_CELL){
		printf("%d.",val);

		}

	else if(type == ERRONEOUS_CELL){
		printf("%d*",val);

			}
	if(val<10){
		printf(" ");}
}
int boardHasASolution(board* b){
	/*TODO: Omer return true if it has a solution*/
	if (!b)
	    return -1;
	printf("\n");
	return 1;
}
int erroneousBoard(board* bTypes){
	int i,j,s = bTypes->squareSideSize;
	for(i=0;i<s;i++){
		for(j=0;j<s;j++){
			if(getCell(bTypes,i,j) == ERRONEOUS_CELL){
				return 1;
			}
		}
	}
	return 0;
}
int setCausesErroneousCellInRow(board* b,board* bTypes,int i,int j,int v,int ind){
	int boardColumns,k,oldV,res=0;
	oldV = getCell(b,i,j);
	boardColumns = b->squareSideSize;
	for(k=0;k<boardColumns;k++){
		if(k!=j&&getCell(b,i,k)==v&&v!=0){/*collision*/

			if(ind==0){
				return 1;
			}
			if(ind==1&&getCell(bTypes,i,k) != FIXED_CELL){
				/*Moshe explained in the moodle forum that fixed cells are never considered erroneous
				 *here we mark the cells that became erroneous*/
				res=1;
				setCell(bTypes, i, k, ERRONEOUS_CELL);}
			if(ind==2&& getCell(bTypes,i,k) == FIXED_CELL){
				return 1;
			}

		}
		if(ind==1&&getCell(b,i,k)==oldV&&getCell(bTypes,i,k)==ERRONEOUS_CELL){
			/*updating cells that are not erroneous thanks to this set*/
			setCell(b,i,j,v);
			if(!setCausesErroneousCell(b,bTypes, i, k, getCell(b,i,k),0)){
				setCell(bTypes,i,k,REGULAR_CELL);}
			setCell(b,i,j,oldV);
		}
	}
	return res;

}
int setCausesErroneousCellInColumn(board* b,board* bTypes,int i,int j,int v,int ind){
	int boardRows,k,oldV,res=0;
	oldV = getCell(b,i,j);
	boardRows = b->squareSideSize;
		for(k=0;k<boardRows;k++){
			if(k!=i&&getCell(b,k,j)==v&&v!=0){/*collision*/

				if(ind==0){
					return 1;
				}
				if(ind==1&&getCell(bTypes,k,j)!=FIXED_CELL){
					/*Moshe explained in the moodle forum that fixed cells are never considered erroneous
					 *here we mark the cells that became erroneous*/
					res=1;
					setCell(bTypes, k, j, ERRONEOUS_CELL);}
				if(ind==2&& getCell(bTypes,k,j) == FIXED_CELL){
					return 1;
				}
			}
			if(ind==1&&getCell(b,k, j)==oldV&&getCell(bTypes,k, j)==ERRONEOUS_CELL){
				/*updating cells that are not erroneous thanks to this set*/
				setCell(b,i,j,v);
				if(!setCausesErroneousCell(b,bTypes, k, j, getCell(b,k, j),0)){
					setCell(bTypes,k, j,REGULAR_CELL);}
				setCell(b,i,j,oldV);
			}
		}
		return res;

}
int setCausesErroneousCellInBlock(board* b,board* bTypes,int i,int j,int v,int ind){
	int blockIndices[2],k,r,oldV,res=0;
	oldV = getCell(b,i,j);
	findCellBlockIndices(b,i,j,blockIndices);

	for(k=blockIndices[0]*b->rows;k<(blockIndices[0]+1)*b->rows;k++){
		for(r=blockIndices[1]*b->columns;r<(blockIndices[1]+1)*b->columns;r++){
			if((r!=j||k!=i)&&getCell(b,k,r)==v&&v!=0){/*collision*/

				if(ind==0){
					return 1;
				}
				if(ind==1&&getCell(bTypes,k,r)!=FIXED_CELL){
					/*Moshe explained in the moodle forum that fixed cells are never considered erroneous
					 *here we mark the cells that became erroneous*/
					res=1;
					setCell(bTypes, k, r, ERRONEOUS_CELL);}
				if(ind==2&& getCell(bTypes,k,r) == FIXED_CELL){
					return 1;
				}
					}
			if(ind==1&&getCell(b,k, r)==oldV&&getCell(bTypes,k, r)==ERRONEOUS_CELL){
				/*updating cells that are not erroneous thanks to this set*/
				setCell(b,i,j,v);
				if(!setCausesErroneousCell(b,bTypes,k, r, getCell(b,k, r),0)){
					setCell(bTypes,k, r,REGULAR_CELL);}
				setCell(b,i,j,oldV);
			}
		}
	}
	return res;
}

int setCausesErroneousCell(board* b,board* bTypes,int i,int j,int v,int ind){
	int b1,b2,b3,res;
	if(getCell(b,i,j)==v){
		return 0;
	}
	if(v==0&&(ind==0||ind==2)){
		return 0;
	}
	b1 = setCausesErroneousCellInRow(b,bTypes,i,j,v,ind);
	if(b1&&!ind){
		return 1;
	}
	b2 = setCausesErroneousCellInColumn(b,bTypes,i,j,v,ind);
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
void setCellAndUpdateErroneous(board* b,board* bTypes,int i,int j,int val){
	int erroneous;
	if(getCell(b,i,j)==val){
		return ;
	}
	erroneous=setCausesErroneousCell( b,bTypes,i,j,val,1);
	setCell(b,i,j,val);
	if(erroneous&&getCell(bTypes, i, j)!=FIXED_CELL){
		setCell(bTypes, i, j, ERRONEOUS_CELL);
	}
	if(!erroneous&&getCell(bTypes, i, j)==ERRONEOUS_CELL){
		setCell(bTypes, i, j, REGULAR_CELL);
	}
}
void setCellUpdateMove(board* b,moveNode* move,int i,int j,int val){
	changeNode* change = createChangeNode(i,j,getCell(b,i,j));
	setCell(b,i,j,val);
	InsertFirst(move->changes,change);
}
void setCellUpdateErroneousAndMove(board* b,board* bTypes,moveNode* move,int i,int j,int val){
	int erroneous;
	erroneous=setCausesErroneousCell( b,bTypes,i,j,val,1);
	setCellUpdateMove(b,move,i,j,val);
	if(erroneous&&getCell(bTypes, i, j)!=FIXED_CELL){
		setCell(bTypes, i, j, ERRONEOUS_CELL);
	}
	if(!erroneous&&getCell(bTypes, i, j)==ERRONEOUS_CELL){
		setCell(bTypes, i, j, REGULAR_CELL);
	}
}

int boardContainsFixedErroneousCells(board *b,board *bTypes){
	int N,i,j,v;
	N = b->squareSideSize;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(getCell(bTypes,i,j) == FIXED_CELL){
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
			if(getCell(bt,i,j) == REGULAR_CELL){
				v = getCell(b,i,j);
				setCell(b,i,j,0);
				erroneous=setCausesErroneousCell( b,bt,i,j,v,1);
				setCell(b,i,j,v);
				if(erroneous){
					setCell(bt, i, j, ERRONEOUS_CELL);
				}
			}

		}}
}

void clearCell(board* pBoard, int i, int j){
    setCell(pBoard, i, j, 0);
}
