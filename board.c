/*
 * board.c
 *
 *      Author: roee
 */
#include <stdlib.h>
#include "board.h"
#include "solver.h"

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
	res->boardArr = calloc((res->squareSideSize*res->squareSideSize),sizeof(int)); /*calloc for valid reads*/
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

/*returns 1 if cell is empty*/
int emptyCell(board* Pboard,int i,int j){
	return getCell(Pboard, i, j)==0;
}

/*@post: all cells in the board contains the value: v*/
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

/*returns the next empty cell index in the board s.t $result and fromInd is 1D index
 * if cell[fromInd] is empty then res=fromInd
 * if the board is full res = MaxIndex+1 */
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
	rowsOfBlocks = b->rows;
	columnsOfBlocks = b->columns;
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
		if (DEBUG) printf("unmatched boards in copyBoard(board* copy,board* orig)");
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
void copyBoardAndUpdateMove(board* copy,board* orig,moveNode* move){
	int i,j,N,v,oldV;
	if(copy->rows!=orig->rows||copy->columns!=orig->columns){
		printf("unmatched boards in copyBoard(board* copy,board* orig)");
		exit(0);
	}
	N=orig->squareSideSize;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			v = getCell(orig,i,j);
			oldV = getCell(copy,i,j);
			if(v!=oldV){
				setCellUpdateMove(copy, move, i, j, v,0);}
		}
	}
}
void printDashRow(board* b){
	int amount = 4*b->squareSideSize+b->rows+1,i;
	for(i=0;i<amount;i++){
		printf("-");
	}
	printf("\n");

}
int printBoard(board* b,board* bTypes,int gameMode,int markErrors){
	int blockRow,rowsOfBlocksInTheBoard;
	rowsOfBlocksInTheBoard=b->columns;
	printDashRow(b);
	for(blockRow=0;blockRow<rowsOfBlocksInTheBoard;blockRow++){
		printRowOfBlocks(b,blockRow,bTypes, gameMode, markErrors);
		printDashRow(b);
	}
	return 1;
}
void printRowOfBlocks(board* b,int blockRow,board* bTypes,int gameMode,int markErrors){
	int boardRow;
	for(boardRow=0;boardRow<b->rows;boardRow++){
		printOneRowInRowsOfBlocks(b,blockRow,boardRow,bTypes,gameMode, markErrors);
	}


}
void printOneRowInRowsOfBlocks(board* b,int blocksRow,int boardRow,board* bTypes,int gameMode,int markErrors){
	int blockComlumnInSingleRow,columnsOfBlocksInBoard;
	columnsOfBlocksInBoard=b->rows;
	printf("|");
	for(blockComlumnInSingleRow=0;blockComlumnInSingleRow<columnsOfBlocksInBoard;blockComlumnInSingleRow++){
		printSingleRowInSingleBlock(b,blocksRow,boardRow,blockComlumnInSingleRow,bTypes, gameMode, markErrors);
		printf("|");
	}
	printf("\n");
}
void printSingleRowInSingleBlock(board* b,int blocksRow,int boardRow,
		int blockComlumnInSingleRow,board* bTypes,int gameMode,int markErrors){
	int i,j,blockRowLength,blockColumnLength,firstColumn;
	blockRowLength = b->rows;
	blockColumnLength = b->columns;
	i=blockRowLength*blocksRow+boardRow;
	firstColumn=blockComlumnInSingleRow*blockColumnLength;
	for(j=firstColumn;j<firstColumn+b->columns;j++){
		/*printf(" ");*/
		printCellValue(b,i,j,bTypes, gameMode, markErrors);
	}

}
void printCellValue(board* b,int i,int j,board* bTypes,int gameMode,int markErrors){
	int val,type;
	val = getCell(b,i,j);
	type = getCell(bTypes,i,j);
	printf(" ");
	if(val<10){
		printf(" ");}
	if(val==0){
			printf("  ");
		}

	else if(type == FIXED_CELL&& gameMode!=EDIT_MODE){
		printf("%d.",val);

		}

	else if(type == ERRONEOUS_CELL&&(gameMode==EDIT_MODE||markErrors)){
		printf("%d*",val);

			}
	else{
		printf("%d ",val);
	}



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
int setCausesErroneousCellInRow(board* b,board* bTypes,int i,int j,int v,int ind,int gameMode){
	int boardColumns,k,oldV,res=0;
	oldV = getCell(b,i,j);
	boardColumns = b->squareSideSize;
	for(k=0;k<boardColumns;k++){
		if(k!=j&&getCell(b,i,k)==v&&v!=0){/*collision*/
			if(ind!=CHECK_FOR_ERRONEOUS_COLLISION_OF_FIXED_CELLS_IND){
				res=1;
			}
			if(ind==CHECK_FOR_ERRONEOUS_CELLS_IND){
				return 1;
			}
			if(ind==CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND&&(getCell(bTypes,i,k) != FIXED_CELL||gameMode==EDIT_MODE)){
				/*Moshe explained in the moodle forum that fixed cells are never considered erroneous
				 *but in EDIT_MODE we do need to mark the fixed erroneous cells*/
				setCell(bTypes, i, k, ERRONEOUS_CELL);

			}
			if(ind==CHECK_FOR_ERRONEOUS_COLLISION_OF_FIXED_CELLS_IND&& getCell(bTypes,i,k) == FIXED_CELL){/*collision with a fixed cell*/
				return 1;
			}
		}
		if(ind==CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND&&getCell(b,i,k)==oldV&&getCell(bTypes,i,k)==ERRONEOUS_CELL){
			/*updating cells that are not erroneous thanks to this set*/
			setCell(b,i,j,v);
			if(!setCausesErroneousCell(b,bTypes, i, k, getCell(b,i,k),0,gameMode)){
				setCell(bTypes,i,k,REGULAR_CELL);}
			setCell(b,i,j,oldV);
		}
	}
	return res;

}
int setCausesErroneousCellInColumn(board* b,board* bTypes,int i,int j,int v,int ind,int gameMode){
	int boardRows,k,oldV,res=0;
	oldV = getCell(b,i,j);
	boardRows = b->squareSideSize;
		for(k=0;k<boardRows;k++){
			if(k!=i&&getCell(b,k,j)==v&&v!=0){/*collision*/
				if(ind!=CHECK_FOR_ERRONEOUS_COLLISION_OF_FIXED_CELLS_IND){
					res=1;
				}
				if(ind==CHECK_FOR_ERRONEOUS_CELLS_IND){
					return 1;
				}
				if(ind==CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND&&(getCell(bTypes,k,j)!=FIXED_CELL||gameMode==EDIT_MODE)){
					/*Moshe explained in the moodle forum that fixed cells are never considered erroneous
					 *here we mark the cells that became erroneous*/
					setCell(bTypes, k, j, ERRONEOUS_CELL);}
				if(ind==CHECK_FOR_ERRONEOUS_COLLISION_OF_FIXED_CELLS_IND&& getCell(bTypes,k,j) == FIXED_CELL){
					return 1;
				}
			}
			if(ind==CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND&&getCell(b,k, j)==oldV&&getCell(bTypes,k, j)==ERRONEOUS_CELL){
				/*updating cells that are not erroneous thanks to this set*/
				setCell(b,i,j,v);
				if(!setCausesErroneousCell(b,bTypes, k, j, getCell(b,k, j),0,gameMode)){
					setCell(bTypes,k, j,REGULAR_CELL);}
				setCell(b,i,j,oldV);
			}
		}
		return res;

}
int setCausesErroneousCellInBlock(board* b,board* bTypes,int i,int j,int v,int ind,int gameMode){
	int blockIndices[2],k,r,oldV,res=0;
	oldV = getCell(b,i,j);
	findCellBlockIndices(b,i,j,blockIndices);
	for(k=blockIndices[0]*b->rows;k<(blockIndices[0]+1)*b->rows;k++){
		for(r=blockIndices[1]*b->columns;r<(blockIndices[1]+1)*b->columns;r++){
			if((r!=j||k!=i)&&getCell(b,k,r)==v&&v!=0){/*collision*/
				if(ind!=CHECK_FOR_ERRONEOUS_COLLISION_OF_FIXED_CELLS_IND){
					res=1;
				}
				if(ind==CHECK_FOR_ERRONEOUS_CELLS_IND){
					return 1;
				}
				if(ind==CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND&&(getCell(bTypes,k,r)!=FIXED_CELL||gameMode==EDIT_MODE)){
					/*Moshe explained in the moodle forum that fixed cells are never considered erroneous
					 *here we mark the cells that became erroneous*/
					setCell(bTypes, k, r, ERRONEOUS_CELL);}
				if(ind==CHECK_FOR_ERRONEOUS_COLLISION_OF_FIXED_CELLS_IND&& getCell(bTypes,k,r) == FIXED_CELL){
					return 1;
				}
					}
			if(ind==CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND&&getCell(b,k, r)==oldV&&getCell(bTypes,k, r)==ERRONEOUS_CELL){
				/*updating cells that are not erroneous thanks to this set*/
				setCell(b,i,j,v);
				if(!setCausesErroneousCell(b,bTypes,k, r, getCell(b,k, r),CHECK_FOR_ERRONEOUS_CELLS_IND,gameMode)){
					setCell(bTypes,k, r,REGULAR_CELL);}
				setCell(b,i,j,oldV);
			}
		}
	}
	return res;
}

/*the function gives a binary answer (0=No, 1=Yes) for the question specified by ind such that:
 * ind=0 for checking if this set command causes erroneous cells
 * ind=1 for checking if this set command causes erroneous cells and updating
 * the erroneous cells in bTypes (adding and removing erroneous flags according to the value of the set)
 * ind=2 if we want to set a fixed cell (i,j) and to check if the set will cause a collision with
 * other fixed cells - this ind is used in the function boardContainsFixedErroneousCells
 * the current value , getCell(i,j) , does not affect this function!
 * @POST: $RET=1 iff the answer to the question determined by ind is YES*/
int setCausesErroneousCell(board* b,board* bTypes,int i,int j,int v,int ind,int gameMode){
	int b1,b2,b3,res;
	if(getCell(b,i,j)==v){
		return 0;
	}
	if(v==0&&(ind==CHECK_FOR_ERRONEOUS_CELLS_IND||ind==CHECK_FOR_ERRONEOUS_COLLISION_OF_FIXED_CELLS_IND)){
		return 0;
	}
	b1 = setCausesErroneousCellInRow(b,bTypes,i,j,v,ind, gameMode);
	if(b1&&!ind){
		return 1;
	}
	b2 = setCausesErroneousCellInColumn(b,bTypes,i,j,v,ind,gameMode);
	if(b2&&!ind){
		return 1;
	}
	b3 = setCausesErroneousCellInBlock(b,bTypes,i,j,v,ind,gameMode);
	if(b3&&!ind){
		return 1;
	}
	res= b1||b2||b3;
	return res;
}

/*@ret>1 if and only if the assignment of value v in the (i,j) cell follows the soduko rules*/
int validAsignment(board* b,int v,int i,int j){
	return !setCausesErroneousCell( b,NULL,i, j, v, CHECK_FOR_ERRONEOUS_CELLS_IND,7);/*7 is arbitrary, if ind==0 then the value of gameMode doesnt matter*/

}


void setCellAndUpdateErroneous(board* b,board* bTypes,int i,int j,int val,int gameMode){
	int erroneous,trivialChange;
	trivialChange= val==getCell(b,i,j);
	if(getCell(b,i,j)==val){
		return ;
	}
	erroneous=setCausesErroneousCell( b,bTypes,i,j,val,CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND,gameMode);/*update all the cell's neighbors cell types*/
	setCell(b,i,j,val);
	if(erroneous&&(getCell(bTypes, i, j)!=FIXED_CELL||gameMode==EDIT_MODE)){
		setCell(bTypes, i, j, ERRONEOUS_CELL);
	}
	if(!erroneous&&getCell(bTypes, i, j)==ERRONEOUS_CELL&&!trivialChange){
		setCell(bTypes, i, j, REGULAR_CELL);
	}
}
void setCellUpdateMove(board* b,moveNode* move,int i,int j,int val,int printInd){
	changeNode* change = createChangeNode(i,j,getCell(b,i,j),val);
	setCell(b,i,j,val);
	if(printInd){
		printf("Cell (%d,%d) value was set to %d\n",j+1,i+1,val);
	}
	InsertFirst(move->changes,change);
}
void setCellUpdateErroneousAndMove(board* b,board* bTypes,moveNode* move,int i,int j,int val,int gameMode,int printInd){
	int erroneous,trivialChange;
	trivialChange= val==getCell(b,i,j);
	erroneous=setCausesErroneousCell( b,bTypes,i,j,val,CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND,gameMode);
	setCellUpdateMove(b,move,i,j,val,printInd);
	if(erroneous&&(getCell(bTypes, i, j)!=FIXED_CELL||gameMode==EDIT_MODE)){
		setCell(bTypes, i, j, ERRONEOUS_CELL);
	}
	if(!erroneous&&getCell(bTypes, i, j)==ERRONEOUS_CELL&&!trivialChange){
		setCell(bTypes, i, j, REGULAR_CELL);
	}
}

/*this function is used to check the validity of a loaded board from file in loadGame*/
int boardContainsFixedErroneousCells(board *b,board *bTypes){
	int N,i,j,v;
	N = b->squareSideSize;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(getCell(bTypes,i,j) == FIXED_CELL){
				v=getCell(b,i,j);
				setCell(b,i,j,0);
				if(setCausesErroneousCell(b,bTypes, i, j, v, CHECK_FOR_ERRONEOUS_COLLISION_OF_FIXED_CELLS_IND,7)){/*7 is arbitrary, if ind==0 then the value of gameMode doesn't matter*/
					return 1;
				}
				setCell(b,i,j,v);

			}
		}
	}

	return 0;
}
void markAllErroneousCellsInBoard(board* b,board* bt,int gameMode){
	int i,j,v,erroneous,N=b->squareSideSize;
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(getCell(bt,i,j) == REGULAR_CELL||gameMode==EDIT_MODE){
				v = getCell(b,i,j);
				setCell(b,i,j,0);
				erroneous=setCausesErroneousCell( b,bt,i,j,v,CHECK_AND_UPDATE_ERRONEOUS_CELLS_IND,gameMode);
				setCell(b,i,j,v);
				if(erroneous){
					setCell(bt, i, j, ERRONEOUS_CELL);
				}			}
		}}
}

void clearCell(board* pBoard, int i, int j){
    setCell(pBoard, i, j, 0);
}
