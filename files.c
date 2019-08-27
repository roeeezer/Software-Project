/*
 * files.c
 *
 *      Author: roee
 */
#include "files.h"



ERROR saveGame(board* b,board* bTypes,char* path,int gameMode){
	FILE* fo;
	int s,i,j;
	s = b->squareSideSize;
	if(!boardIsValid(b)){
		/*TODO: deal with the error*/
		return TMP_ERROR;
	}
	if(erroneousBoard(bTypes)){
		/*TODO: deal with the error*/
		return TMP_ERROR;
	}
	fo = fopen(path,"w+");
	if(fo==NULL){
		/*TODO: deal with the error*/
		return FOPEN_ERROR;
	}
	fprintf(fo,"%d %d\n",b->rows,b->columns);
	for(i=0;i<s;i++){
		for(j=0;j<s;j++){
			fprintf(fo,"%d",getCell(b,i,j));
			if(getCell(bTypes,i,j)==FIXED||gameMode==EDIT){
				fprintf(fo,".");
			}
			if(j!=s-1){
				fprintf(fo," ");
			}
			if(j==s-1){
				fprintf(fo,"\n");
			}
		}
	}
	if(fclose(fo)==-1){/*fclose returns EOF when it fails*/
		return FCLOSE_ERROR;
	}
	return NO_ERROR;
}
/*b,bTypes should NOT be the real boards of the current game, they should be 2 new
 * empty board pointers so we can restore the old game in case the load command has failed*/
ERROR loadGame(board* b,board* bTypes,char* path){
	int n,m,N,i,j,val;
	char ch;
	FILE* f;
	f = fopen(path,"r");
	if(f==NULL){
		/*TODO: deal with the error*/
		return FOPEN_ERROR;
	}
	if(fscanf(f,"%d",&n)<=0){
		return INVALID_FILE_FORMAT;
	}
	if(fscanf(f,"%d",&m)<=0){
		return INVALID_FILE_FORMAT;
	}
	N=n*m;
	resetBoard(bTypes,REGULAR);
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			fscanf(f,"%d",&val);
			setCell(b,i,j,val);
			fscanf(f,"%c",&ch);
			/*if this scan could not scan a char (but a number) then the pointer in the file will not
			 * progress so we won't have to change the process of the next iteration*/
			if(ch=='.'){
				setCell(bTypes,i,j,FIXED);}
		}

	}
	if(fclose(f)==-1){/*fclose returns EOF when it fails*/
		return FCLOSE_ERROR;
	}
	return NO_ERROR;
}
