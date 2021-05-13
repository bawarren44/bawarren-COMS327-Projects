#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "verbose.h"

/**
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 */

/**
 * Main method that is used to create the change executable. Processes moves as specified from the command line arguments.
 */
int main(int argc, char** argv) {
    char fileLine[150];
    char *buff = fileLine;
    int size = 100;
    int lineCount = 0;
    int flag = 0;
    int index = 0;
    char rule[20];
    char playerTurn[20];
    char ruleCheck[20];
    int redPawns= 0;
    int redKings= 0;
    int blackPawns= 0;
    int blackKings= 0;
    int count = 0;
    char *ptr;
    int D = 0;
    int fileThere = 0;
    FILE* file;


    for(int i = 1; i < argc; i++){
        if(strcmp(argv[i], "-v")==0){
            verbose = 1;
        }
        if(strcmp(argv[i],"-d") == 0) {
            D = (int) strtol(argv[i + 1],&ptr,10);
        }
	if(fileThere == 0){
	    file = fopen(argv[i], "r");
	    if(file != NULL){
		fileThere = 1;
	    }
	}
    }

    //Delete later
//    FILE* file;
//    file = fopen("C:\\Users\\sup3r\\coms327\\coms327\\5.txt", "r");

    if(fileThere == 0){
	file = stdin;
    }

    //Get each line one by one and validate them
    char validator[20] = "1";

    while(fgets(buff, size, file) != NULL) {
        lineCount++;

        //Rule capture or no capture validation
        if (rulesCheck(fileLine) == 1) {
            strcpy(validator, "get capture");
            flag = lineCount;
        }

        //Determines the capture rules
        if (validator[0] == 'g') { // if validator was updated to "get capture" after calling rulesCheck(fileLine)
            if (captureRules(fileLine) == 2) {
                strcpy(rule, "no capture");
                strcpy(validator, "now jumps");
                flag = lineCount;
            } else if (captureRules(fileLine) == 1) {
                strcpy(validator, "now jumps");
                strcpy(rule, "capture");
                flag = lineCount;
            } else if(captureRules(fileLine) == 0){
                strcpy(rule, "invalid");
            }
        }

        //Rule single jumps or multiple jumps validation
        if (validator[0] == 'n') { // if validator was updated to "now jumps" after calling captureRules(fileLine)
            if (jumpRules(fileLine) == 2) {
                strcpy(rule, "multiple jumps");
                strcpy(validator, "to turn");
                flag = lineCount + 1;
            } else if (jumpRules(fileLine) == 1) {
                strcpy(validator, "to turn");
                strcpy(rule, "single jumps");
                flag = lineCount + 1;
            }
        }

        //Checks if 'TURN:' is present
        if (validator[0] == 't') { // if validator was updated to "to turn" after calling jumpRules(fileLine)
            if (turnCheck(fileLine) == 1) {
                strcpy(validator, "pick turn");
                flag = lineCount;
            }
        }

        //Checks if turn gets decided
        if (validator[0] == 'p') { // if validator was updated to "pick turn" after calling turnCheck(fileLine)
            if (turn(fileLine) == 1) {
                strcpy(playerTurn, "red");
                strcpy(validator, "board");
            } else if (turn(fileLine) == 2) {
                strcpy(validator, "board");
                strcpy(playerTurn, "black");
            }
        }

        //Check for 'BOARD:'
        if(validator[0] == 'b'){ // if validator was updated to "board" after calling turn(fileLine)
            if(checkBoard(fileLine) == 1){
                strcpy(validator, "define board");
                flag = lineCount;
            }
        }

        //Check format of board
        if(validator[0] == 'd' || validator[0] == 'a'){ // if validator was updated to "define board" after calling checkBoard(fileLine) or

            boardCopy(fileLine, index, count);
            index = idx;
            count = cnt;

            //Determines if there are unnecessary content between board elements
            if((validator[0] == 'a' && index ==  0)){
                strcpy(validator, "error");
                flag = lineCount;
                break;
            }else if(index == 64){ //Determines if board is completely full or not
                if(boardValidation(brd) == 0){ //if returned 0 the board has an error in format
                    strcpy(validator, "error");
                    flag = lineCount;
                    break;
                }else{
                    for(int i = 0; i< 64; i++){ //Gets number of pawns or kings in each color by traversing through the array
                        if(brd[i] == 'b'){
                            blackPawns++;
                        }else if(brd[i] == 'B'){
                            blackKings++;
                        }else if(brd[i] == 'r'){
                            redPawns++;
                        }else if(brd[i] == 'R'){
                            redKings++;
                        }
                    }
                    strcpy(validator, "move next");
                }
            }else if(index > 0){
                strcpy(validator, "array continue");
            }
        }

        if(validator[0] == 'm' && count < 63){//if we move to check move and number of elements in board is less than 63 we have an error
            strcpy(validator, "error");
            flag = lineCount + 1;
        }

        //Check for 'MOVES:'
        if(validator[0] == 'm'){
            if(checkMoves(fileLine) == 1){
                strcpy(validator, "offense");
            }
        }
        if(validator[0] == 'o' || validator[0] == 'r'){// if validator was updated to "offense" after calling checkMoves(fileLine) or
            // if validator was updated to "ready" after calling moves(fileLine)
            if(moves(fileLine) == 1){
                strcpy(validator, "ready");
            }else if((moves(fileLine) == 0) && (validator[0] == 'r')){
                strcpy(validator, "illegal");
                flag = lineCount;
            }
        }

    }

    if(validator[0] == 'a' && brd[63] != '"'){
        strcpy(validator, "error");
        flag = lineCount + 1;
    }

    //Prints error message if rules is not present at the end of reading the file
    if(validator[0] == '1'){
        fprintf(stderr, "Error near line ");
        fprintf(stderr, "%d", lineCount + 1);
        fprintf(stderr, ": expecting 'RULES:' \n");
    }

    //Copies the board into an 8x8 format
    boardFormat();

    //Ends program with status 1 if an unimplemented rule is present
    if(strcmp(capture, "capture") == 0){
        fprintf(stderr, "Sorry, capture is not implemented\n");
        exit(1);
    }
    if(strcmp(jump, "multiple jumps") == 0){
        fprintf(stderr, "Sorry, multiple jumps are not implemented\n");
        exit(1);
    }

    //PART 3 START
//    D = 1;
    deep = D;
//    verbose = 1;

    //Starts recusive god method to process everything for red player turn
    if(playerTurn[0] == 'r'){
        recursiveGodMethod(0,D,0,boardF);
    }else{     //Starts recusive god method to process everything for black player turn
        recursiveGodMethod(0,D,1,boardF);
    }
    return 0;
}
