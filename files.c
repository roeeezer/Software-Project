/*
 * files.c
 *
 *      Author: roee
 */
#include "files.h"
void emptyFunction5(){}

void saveGame(board* b,board* bTypes,char* path){
	FILE* fo;
	int s,i,j;
	fo = fopen(path,"w+");
	fprintf(fo,"%d %d\n",b->rows,b->columns);
	s = b->squareSideSize;
	i=0;
	j=0;
	for(i=0;i<s;i++){
		for(j=0;j<s;j++){
			fprintf(fo,"%d",getCell(b,i,j));
			if(getCell(bTypes,i,j)==FIXED){
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
	fclose(fo);
}
ERROR* loadBoard(board* b,board* bTypes,char* path){
	printf("%d%d%s",b,bTypes,path);
	return NULL;
}
