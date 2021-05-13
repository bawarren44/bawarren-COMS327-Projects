#include <stdio.h>
#include "verbose.h"

/**
 * ALL OF THIS IS USELESS USE MF RECURSION
 */

/**
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 */

/**
 * Struct that holds all information involved with a game piece on the board
 */
struct piece{
    struct moves *allMoves;
    int i;
    int j;
    int PorK;
};
//MAKE EACH MOVE INTO A STRUCT OF ITS OWN WITH SCORE AND SUBMOVES

struct move{
    int score;
    char m[7];
    char board[8][8];
};

struct lists{
    struct piece** blackList;
    struct piece** redList;
    int redListIdx, blackListIdx;
    int red, black;
};

/**
 * Made a list of mf lists
 * @param board
 * @param index
 */
void makeLists(char board[8][8], int index){
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j] == 'b'){
                primary[index]->black++;
            }else if(board[i][j] == 'B'){
                primary[index]->black++;
            }else if(board[i][j] == 'r'){
                primary[index]->red++;
            }else if(board[i][j] == 'R'){
                primary[index]->red++;
            }
        }
    }
    primary[index]->redList = malloc(sizeof(primary[index]->redList)*primary[index]->red);
    primary[index]->blackList = malloc(sizeof(primary[index]->blackList)*primary[index]->black);
    for(int i = 0; i < 8; i++){
         for(int j = 0; j < 8; j++){
             if(board[i][j] == 'r' || board[i][j] == 'R'){
                 primary[index]->redList[primary[index]->redListIdx] = (struct piece*) malloc(sizeof(struct piece));
                 primary[index]->redList[primary[index]->redListIdx]->i = i;
                 primary[index]->redList[primary[index]->redListIdx]->j = j;
                 if(board[i][j] == 'r'){
                     primary[index]->redList[primary[index]->redListIdx]->PorK = 0;
                 }else{
                     primary[index]->redList[primary[index]->redListIdx]->PorK = 1;
                 }
                 primary[index]->redListIdx++;
             }else if(board[i][j] == 'b' || board[i][j] == 'B'){
                 primary[index]->blackList[primary[index]->blackListIdx] = (struct piece*) malloc(sizeof(struct piece));
                 primary[index]->blackList[primary[index]->blackListIdx]->i = i;
                 primary[index]->blackList[primary[index]->blackListIdx]->j = j;
                 if(board[i][j] == 'b'){
                     primary[index]->blackList[primary[index]->blackListIdx]->PorK = 0;
                 }else{
                     primary[index]->blackList[primary[index]->blackListIdx]->PorK = 1;
                 }
                 primary[index]->blackListIdx++;
             }
         }
     }
 }




