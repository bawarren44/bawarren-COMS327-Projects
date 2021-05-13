#include "verbose.h"


struct move **here;
int hereIdx = 0;
int moveListIdxb = 0;

/**
 * Used to contruct a list of moves into structs.
 * If move->good == 1 then illegal,2 follows an illegal, 0 legal.
 * Copies board to here index
 */
void constructMoves(int x, int first){
	here[hereIdx]->move = malloc(sizeof(char)*7);
	strcpy(here[hereIdx]->move, (*(moveList + moveListIdxb)));//Copy move to struct here at index
	for(int i = 0; i < 8; i++){
		for(int j = 0; j < 8; j++){
			here[hereIdx]->board[i][j] = '0';
		}
	}
	//Copies board to here board at index
	for(int i = 0; i < 8; i++){
		strcpy(here[hereIdx]->board[i],boardF[i]);
	}

	//Sets if move is legal, illegal or follows an illegal
	if(x == 1 && first == 0){
		here[hereIdx]->good = 2;
	}else{		
		here[hereIdx]->good = x;
	}
	hereIdx++;
}
