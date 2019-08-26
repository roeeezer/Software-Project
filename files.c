/*
 * files.c
 *
 *      Author: roee
 */
#include "files.h"
#define INIT 0
#define EDIT 1
#define SOLVE 2
void emptyFunction5(){}

ERROR saveGame(board* b,board* bTypes,char* path,int gameMode){
	FILE* fo;
	int s,i,j;
	s = b->squareSideSize;
	if(!boardIsValid(b)){
		/*TODO: deal with the error*/
		return NULL;
	}
	if(erroneousBoard(bTypes)){
		/*TODO: deal with the error*/
		return NULL;
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
	if(fclose(fo)==NULL){
		return FCLOSE_ERROR;
	}
	return NO_ERROR;
}
/*b,bTypes should NOT be the real boards of the current game, they should be 2 new
 * empty boards so we can restore the old game in case the load command has failed*/
ERROR* loadBoard(board* b,board* bTypes,char* path){
	int n,m;
	FILE* f;
	f = fopen(path,"r");
	if(f==NULL){
		/*TODO: deal with the error*/
		return FOPEN_ERROR;
	}
	if(fscanf(f,"%d",&n)<=0){
		return INVALID_FILE_FORMAT;
	}
	if(fscanf(f,"%d",&n)<=0){
		return INVALID_FILE_FORMAT;
	}
	if(fclose(f)==NULL){
		return FCLOSE_ERROR;
	}
	printf("n:%d, m:%d\n",n,m);
	printf("%d%d%s",b,bTypes,path);
	return NULL;
}
