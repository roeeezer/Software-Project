/*
 * files.h
 *
 *      Author: roee
 */

#ifndef FILES_H_
#define FILES_H_
#include "board.h"
#include "error.h"


ERROR saveGame(board* b,board* bTypes,char* path,int gameMode);
ERROR loadGame(board **b,board **bTypes,char* path,int *n,int *m);
#endif /* FILES_H_ */
