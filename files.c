/*
 * files.c
 *
 *      Author: roee
 */
#include "files.h"
#include "solver.h"


/*saves the given board as a file in the required format located in path*/
ERROR saveGame(board* b,board* bTypes,char* path,int gameMode){
	FILE* fo;
	int s,i,j;
	s = b->squareSideSize;
	if(gameMode==EDIT_MODE){
		if(erroneousBoard(bTypes)){
			return SAVE_ERRONEOUS_CELLS_IN_EDIT_MODE;
		}
		if(!boardHasASolution(b)){
			return SAVE_UNSOLVABLE_BOARD_IN_EDIT_MODE;
		}
	}

	fo = fopen(path,"w+");
	if(fo==NULL){
		/*TODO: deal with the error*/
		return FOPEN_ERROR;
	}
	if(fprintf(fo,"%d %d\n",b->rows,b->columns)<0){
		return FPRINTF_ERROR;
	}
	for(i=0;i<s;i++){
		for(j=0;j<s;j++){
			if(fprintf(fo,"%d",getCell(b,i,j))<0){
				return FPRINTF_ERROR;
			}
			if((getCell(bTypes,i,j) == FIXED_CELL || gameMode == EDIT_MODE) && getCell(b, i, j) != 0){
				if(fprintf(fo,".")<0){
					return FPRINTF_ERROR;
				}
			}
			if(j!=s-1){
				if(fprintf(fo," ")<0){
					return FPRINTF_ERROR;
				}
			}
			if(j==s-1){
				if(fprintf(fo,"\n")<0){
					return FPRINTF_ERROR;
				}
			}
		}
	}
	if(fclose(fo)==-1){/*fclose returns EOF when it fails*/
		return FCLOSE_ERROR;
	}
	printf("The board was saved successfully!\n");
	return NO_ERROR;
}

/*returns a binary answers to the question: can a saved board file in the required format
 * contains the char c*/
int validChar(char c){
	if(((int)c)>=48 &&((int)c)<=57){/*c is a digit 0-9*/
		return 1;
	}
	if(c=='.'||c==' '){
		return 1;
	}
	if(((int)c)==10){/*I think ch gets this value when the scan was invalid*/
		return 1;
	}
	if(((int)c)==13){/*when txt files are copied to the nova somehow they contain this char in the nova server*/
		return 1;
	}
	return 0;
}

/*given a file that is located in path and contains the data of a sudoku-board in the required format this function
 * creates the board described in the file and assign the pointers b,bTypes to point to the newly
 * created board
 * b,bTypes should NOT be the real boards of the current game, they should be 2 new
 * empty board pointers so we can restore the old game in case the load command fails*/
ERROR loadGame(board** b,board** bTypes,char* path,int *n,int *m,int gameMode){
	int N,i,j,val;
	char ch;
	FILE* f;
	f = fopen(path,"r");
	if(f==NULL){
		return FOPEN_ERROR;
	}
	if(fscanf(f,"%d",n)<=0){
		return INVALID_FILE_FORMAT;
	}
	if(fscanf(f,"%d",m)<=0){
		return INVALID_FILE_FORMAT;
	}
	if(*n<1 ||*m<1){
		return INVALID_FILE_FORMAT;
	}
	N=(*n)*(*m);
	if(N>=100){/*we do not support 3 digits numbers in cells*/
		return INVALID_FILE_FORMAT;
	}
	*b = createBoard(*n,*m);
	*bTypes = createBoard(*n,*m);
	resetBoard(*bTypes, REGULAR_CELL);
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			if(fscanf(f,"%d",&val)<=0){
				return INVALID_FILE_FORMAT_BOARDS_WERE_CREATED;
			}
			if(val<0||val>N){
				return INVALID_FILE_FORMAT_BOARDS_WERE_CREATED;
			}
			setCell(*b,i,j,val);
			fscanf(f,"%c",&ch);
			/*if this scan could not scan a char (but a number) then the pointer in the file will not
			 * progress so we won't have to change the process of the next iteration*/
			if(!validChar(ch)){
				return INVALID_FILE_FORMAT_BOARDS_WERE_CREATED;
			}
			if(ch=='.'){
				if(val==0){/*fixed cells with value 0*/
					return INVALID_FILE_FORMAT_BOARDS_WERE_CREATED;
				}
				setCell(*bTypes, i, j, FIXED_CELL);}
		}

	}
	if(fscanf(f,"%d",&val)>0){/*the file contains too many numbers for the give n,m*/
		return INVALID_FILE_FORMAT_BOARDS_WERE_CREATED;
	}
	if(boardContainsFixedErroneousCells(*b,*bTypes)){
		return FIXED_ERRONEOUS_CELLS_IN_FILE;
	}
	if(fclose(f)==-1){/*fclose returns EOF when it fails*/
		return FCLOSE_ERROR;
	}
	markAllErroneousCellsInBoard( *b,*bTypes,gameMode);
	return NO_ERROR;
}
