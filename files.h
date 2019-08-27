/*
 * files.h
 *
 *      Author: roee
 */

#ifndef FILES_H_
#define FILES_H_
#include "board.h"
#include "error.h"
#define INIT 0
#define EDIT 1
#define SOLVING 2

ERROR saveGame(board* b,board* bTypes,char* path,int gameMode);
ERROR loadGame(board* b,board* bTypes,char* path);
#endif /* FILES_H_ */
