#include <stdio.h>
#include "change.h"

/**
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 */
int score[2], deep, haveWeChecked = 0, oppDepth = 0, verbose = 0, final = 0;

/**
 * List of moves
 */
struct moves{
    char **moves;
};

struct move{
	char *move;
	char board[8][8];
	int good;
	int x;
	int y;
};

/**
 * Prints a move of a piece in verbose form
 * @param depth - determines indentation
 * @param m - move
 * @param turn - players turn
 */
void printVerboseForm(int depth, char* m, int turn){
    int count = 0;
    while(count < depth){
        printf("\t");
        count++;
    }
    char pt[10];
    //Sets pt to be either "red" or "black"
    if(turn % 2 == 0){
        strcpy(pt, "red");
    }else{
        strcpy(pt, "black");
    }
    printf("? %c%c%c%c%c%c for %s:\n",m[0],m[1],m[2],m[3],m[4],m[5],pt);
}

/**
 * Prints the score of a certain pieces move
 * @param depth - determines the indentation
 * @param m - move
 * @param turn - players turn
 * @param goal - score
 */
void printMoveScore(int depth, char* m, int turn, int goal){
    int count = 0;
    while(count < depth){
        printf("\t");
        count++;
    }
    char pt[10];
    //For move during reds turn
    if(turn % 2 == 0){
        //Print in verbose form
        if(verbose == 1){
            strcpy(pt, "red");
            printf(". %c%c%c%c%c%c for %s: score %d\n",m[0],m[1],m[2],m[3],m[4],m[5],pt, goal);
        }else{ //Print in non-verbose form
            strcpy(pt, "red");
            printf("%c%c%c%c%c%c for %s: score %d\n",m[0],m[1],m[2],m[3],m[4],m[5],pt, goal);
        }
    //For move during blacks turn
    }else{
        //Print in verbose form
        if(verbose == 1){
            strcpy(pt, "black");
            printf(". %c%c%c%c%c%c for %s: score %d\n",m[0],m[1],m[2],m[3],m[4],m[5],pt, goal);
        }else{ //Print in non-verbose form
            strcpy(pt, "black");
            printf("%c%c%c%c%c%c for %s: score %d\n",m[0],m[1],m[2],m[3],m[4],m[5],pt, goal);
        }
    }
}

/**
 * Reverts the board to its original state before last move
 * @param b4 - board at previous state
 */
void revertBoard(char b4[8][8]){
    for(int i = 0; i < 8; i++){
        strcpy(boardF[i], b4[i]);
    }
    memset(beforeBoard, 0, sizeof(char[8][8]));
}

/**
 * Gives the letter from a number
 * @param i - number to be determined
 * @return letter of the move
 */
char getMoveLetter(int i){
    if(i == 0){
        return 'a';
    }else if (i == 1){
        return 'b';
    }else if (i == 2){
        return 'c';
    }else if (i == 3){
        return 'd';
    }else if (i == 4){
        return 'e';
    }else if (i == 5){
        return 'f';
    }else if (i == 6){
        return 'g';
    }else if (i == 7){
        return 'h';
    }else{
        return '0';
    }
}

/**
 * Gives the number from a given number
 * @param i - number to be determined
 * @return number of given move
 */
char getMoveNumber(int i){
    if(i == 0){
        return '8';
    }else if (i == 1){
        return '7';
    }else if (i == 2){
        return '6';
    }else if (i == 3){
        return '5';
    }else if (i == 4){
        return '4';
    }else if (i == 5){
        return '3';
    }else if (i == 6){
        return '2';
    }else if (i == 7){
        return '1';
    }else{
        return '0';
    }
}

/**
 * Gets the score of the board
 * @param turn - determines whose turn it is
 * @return the score according to the turn given
 */
int getScore(int turn){
    //Reds score
    score[0] = 0;
    //Blacks score
    score[1] = 0;
    int redsP = 0, redsK = 0, blacksP = 0, blacksK = 0;
    int i = 0;
    //Gets the count of all the pieces
    for(i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(boardF[i][j] == 'r'){
                redsP++;
            }else if(boardF[i][j] == 'R'){
                redsK++;
            }else if(boardF[i][j] == 'b'){
                blacksP++;
            }else if(boardF[i][j] == 'B'){
                blacksK++;
            }
        }
    }
    //Determines if game is over
    if(blacksK == 0 && blacksP == 0){
        score[0] = 99;
        score[1] = -99;
    }else if(redsK == 0 && redsP == 0) {
        score[0] = -99;
        score[1] = 99;
    }else{ // Calculates score if game isnt over
        score[0] = (redsP + 2 * redsK) - (blacksP + 2 * blacksK);
        score[1] = (blacksP + 2 * blacksK) - (redsP + 2 * redsK);
    }
    //Return red score
    if(turn %2 == 0){
        return score[0];
    }else{ //Return black score
        return score[1];
    }
}

/**
 * Simulates all possible moves from a given location on the board.
 * @param i - row
 * @param j - column
 * @param list - list of moves to add the move to
 * @return 0
 */
int simulateMoves(int i, int j, struct moves *list){
    int c;
    //Creates room for moves in the list of moves
    if(list->moves == NULL){
        list->moves = malloc(sizeof(char)*7*8);
        for(int b = 0; b < 8; b++){
            *(list->moves + b) = malloc(sizeof(char)* 7);
        }
    }
    char m[2];
    char mv[7], mv1[7], mv2[7], mv3[7], mv4[7], mv5[7], mv6[7], mv7[7], temp[7];
    char hold[8][7];
    int m21, m22;
    //For when board position if red pawn
    if(boardF[i][j] == 'r'){
        //Sets each move to be a different possibility
        mv[0] = getMoveLetter(j);mv[1] = getMoveNumber(i);m21 = j - 1;m22 = i - 1;
        mv[4] = getMoveLetter(m21);mv[5] = getMoveNumber(m22);mv[2] = '-'; mv[3] = '>';mv[6] = '\0';
        mv1[0] = mv[0]; mv2[0] = mv[0]; mv3[0] = mv[0]; mv1[1] = mv[1]; mv2[1] = mv[1]; mv3[1] = mv[1];
        mv1[2] = '-'; mv1[3] = '>';mv1[6] = '\0'; mv2[2] = '-'; mv2[3] = '>';mv2[6] = '\0'; mv3[2] = '-'; mv3[3] = '>';mv3[6] = '\0';
        m21 = j + 1;m22 = i - 1;mv1[4] = getMoveLetter(m21);mv1[5] = getMoveNumber(m22);
        m21 = j + 2;m22 = i - 2;mv2[4] = getMoveLetter(m21);mv2[5] = getMoveNumber(m22);
        m21 = j - 2;m22 = i - 2;mv3[4] = getMoveLetter(m21);mv3[5] = getMoveNumber(m22);
        //Copies moves into temp list
        strcpy(hold[0], mv);
        strcpy(hold[1], mv1);
        strcpy(hold[2], mv2);
        strcpy(hold[3], mv3);

        //Orders the moves in alphabetical
        for(int p = 0; p< 4; p++) {
            for(int k = 0; k<4; k++) {
                if(strcmp(hold[p], hold[k]) < 0) {
                    strcpy(temp, hold[p]);
                    strcpy(hold[p], hold[k]);
                    strcpy(hold[k], temp);
                }
            }
        }
        //Copies the orders list into the result list
        strcpy(list->moves[0], hold[0]);
        strcpy(list->moves[1], hold[1]);
        strcpy(list->moves[2], hold[2]);
        strcpy(list->moves[3], hold[3]);
        c = 4;

    }else if(boardF[i][j] == 'b'){ //For when board position is black pawn
        //Sets each move to be a different possibility
        mv[0] = getMoveLetter(j);mv[1] = getMoveNumber(i);m21 = j + 1;m22 = i + 1;
        mv[4] = getMoveLetter(m21);mv[5] = getMoveNumber(m22);mv[2] = '-'; mv[3] = '>';mv[6] = '\0';
        mv1[0] = mv[0]; mv2[0] = mv[0]; mv3[0] = mv[0]; mv1[1] = mv[1]; mv2[1] = mv[1]; mv3[1] = mv[1];
        m21 = j - 1;m22 = i + 1;mv1[4] = getMoveLetter(m21);mv1[5] = getMoveNumber(m22);
        mv1[2] = '-'; mv1[3] = '>';mv1[6] = '\0';
        m21 = j - 2;m22 = i + 2;mv2[4] = getMoveLetter(m21);mv2[5] = getMoveNumber(m22);
        mv2[2] = '-'; mv2[3] = '>';mv2[6] = '\0';
        m21 = j + 2;m22 = i + 2;mv3[4] = getMoveLetter(m21);mv3[5] = getMoveNumber(m22);
        mv3[2] = '-'; mv3[3] = '>';mv3[6] = '\0';
        //Copies moves into temp list
        strcpy(hold[0], mv);
        strcpy(hold[1], mv1);
        strcpy(hold[2], mv2);
        strcpy(hold[3], mv3);

        //Orders the moves in alphabetical
        for(int p = 0; p< 4; p++) {
            for(int k = 0; k<4; k++) {
                if(strcmp(hold[p], hold[k]) < 0) {
                    strcpy(temp, hold[p]);
                    strcpy(hold[p], hold[k]);
                    strcpy(hold[k], temp);
                }
            }
        }
        //Copies the orders list into the result list
        strcpy(list->moves[0], hold[0]);
        strcpy(list->moves[1], hold[1]);
        strcpy(list->moves[2], hold[2]);
        strcpy(list->moves[3], hold[3]);
        c = 4;

    }else if(boardF[i][j] == 'R' || boardF[i][j] == 'B'){ //For when board position is a king
        //Sets each move to be a different possibility
        mv[0] = getMoveLetter(j);mv[1] = getMoveNumber(i);m21 = j - 1;m22 = i - 1;
        mv[4] = getMoveLetter(m21);mv[5] = getMoveNumber(m22);mv[2] = '-'; mv[3] = '>';mv[6] = '\0';
        mv1[0] = mv[0]; mv2[0] = mv[0]; mv3[0] = mv[0]; mv1[1] = mv[1]; mv2[1] = mv[1]; mv3[1] = mv[1];
        mv1[2] = '-'; mv1[3] = '>';mv1[6] = '\0'; mv2[2] = '-'; mv2[3] = '>';mv2[6] = '\0'; mv3[2] = '-'; mv3[3] = '>';mv3[6] = '\0';
        m21 = j + 1;m22 = i - 1;mv1[4] = getMoveLetter(m21);mv1[5] = getMoveNumber(m22);
        m21 = j + 2;m22 = i - 2;mv2[4] = getMoveLetter(m21);mv2[5] = getMoveNumber(m22);
        m21 = j - 2;m22 = i - 2;mv3[4] = getMoveLetter(m21);mv3[5] = getMoveNumber(m22);
        mv4[0] = getMoveLetter(j);mv4[1] = getMoveNumber(i);m21 = j + 1;m22 = i + 1;
        mv4[4] = getMoveLetter(m21);mv4[5] = getMoveNumber(m22);mv4[2] = '-'; mv4[3] = '>';mv4[6] = '\0';
        mv5[0] = mv[0]; mv6[0] = mv[0]; mv7[0] = mv[0]; mv5[1] = mv[1]; mv6[1] = mv[1]; mv7[1] = mv[1];
        m21 = j - 1;m22 = i + 1;mv5[4] = getMoveLetter(m21);mv5[5] = getMoveNumber(m22);
        mv5[2] = '-'; mv5[3] = '>';mv5[6] = '\0';
        m21 = j - 2;m22 = i + 2;mv6[4] = getMoveLetter(m21);mv6[5] = getMoveNumber(m22);
        mv6[2] = '-'; mv6[3] = '>';mv6[6] = '\0';
        m21 = j + 2;m22 = i + 2;mv7[4] = getMoveLetter(m21);mv7[5] = getMoveNumber(m22);
        mv7[2] = '-'; mv7[3] = '>';mv7[6] = '\0';
        //Copies moves into temp list
        strcpy(hold[0], mv);
        strcpy(hold[1], mv1);
        strcpy(hold[2], mv2);
        strcpy(hold[3], mv3);
        strcpy(hold[4], mv4);
        strcpy(hold[5], mv5);
        strcpy(hold[6], mv6);
        strcpy(hold[7], mv7);

        //Orders the moves in alphabetical
        for(int p = 0; p< 8; p++) {
            for(int k = 0; k<8; k++) {
                if(strcmp(hold[p], hold[k]) < 0) {
                    strcpy(temp, hold[p]);
                    strcpy(hold[p], hold[k]);
                    strcpy(hold[k], temp);
                }
            }
        }
        //Copies the orders list into the result list
        strcpy(list->moves[0], hold[0]);
        strcpy(list->moves[1], hold[1]);
        strcpy(list->moves[2], hold[2]);
        strcpy(list->moves[3], hold[3]);
        strcpy(list->moves[4], hold[4]);
        strcpy(list->moves[5], hold[5]);
        strcpy(list->moves[6], hold[6]);
        strcpy(list->moves[7], hold[7]);
        c = 8;

    }
    return c;
}

/**
 * This method will end my pain. PLEASE MAY GOD HAVE MERCY ON ME, AND GIVE ME EXTREME RECURSION PROCESSING ABILITIES.
 * This method is a recursive method that processes the board from the end forward and prints out the score according to
 * the inputs of goal
 *
 * @param goal - last score
 * @param depth - depth of the move being processed
 * @param playerTurn - determines whos turn it is, and processes moves according to turn
 * @param board - playing board the move follows
 */
int recursiveGodMethod(int goal, int depth, int playerTurn, char board[8][8]){
    int prevScore = -goal;
    int scores[8] = {0};
    scores[0] = 0;scores[1] = 0;scores[2] = 0;scores[3] = 0;scores[4] = 0;scores[5] = 0;scores[6] = 0;scores[6] = 0;
    int scoreIdx = 0;
    int points[8];
    for(int i = 0; i < 8; i++){
        points[i] = 0;
    }
    int indx = 0;
    struct moves *list = malloc(sizeof(struct moves));
    char nextBoard[8][8] = {'0'};
    char b4[8][8] = {'0'};
    int highest = 0;
    int lowest = 0;
    int goalZero =0;

    //If depth is reached return goal
    if(depth == 0 && haveWeChecked != 0){
        return goal;
    }else{
        //For red players turn
        if(playerTurn %2 == 0){
            int count = 0;
            int fir = 8;
            int sec = 0;
            while(1){
                if(count == 8){
                    break;
                }
                if(fir == 0){
                    fir = 8;
                    sec++;
                    count++;
                }
                //Checks board until we get a red pawn of king
                if(boardF[fir][sec] == 'r' || boardF[fir][sec] == 'R'){
                    //Simulates the moves of the piece
                    int n = simulateMoves(fir,sec, list);
                    //Runs all the moves to determine if valid
                    for(int k = 0; k< n; k++){
                        int c = (int) strlen(*(list->moves));
                        if(list->moves[k][0] != '\0'){
                            forRank = 1;
                            if(moveListCheck(list->moves[k]) == 0){
                            }else{ //If move is valid then process score
                                for(int i = 0; i < 8; i++){
                                    strcpy(nextBoard[i], boardF[i]);
                                    strcpy(b4[i], beforeBoard[i]);
                                }
                                //Process score
                                goal = getScore(0);
                                if(goal == 99 && depth != 0){
                                    final = 1;
                                }
                                scores[scoreIdx] = goal;
                                //If verbose is active print in verbose form
                                if(verbose == 1){
                                    if(depth != 0){
                                        printVerboseForm(oppDepth, list->moves[k], 0);
                                    }
                                }
                                //Process until depth is reached and game isnt over
                                if(depth != 0 && goal != 99){
                                    oppDepth++;
                                    //Recursive call to method to determine opposing score from current move
                                    goal = recursiveGodMethod(goal, depth-1, playerTurn+1, nextBoard);
                                    goal = -goal;
                                    scores[scoreIdx] = goal;
                                    oppDepth--;
                                }
                                //Prints in verbose if verbose is active
                                if(verbose == 1){
                                    printMoveScore(deep-depth, list->moves[k], 0, goal);
                                }else if(verbose == 0 && depth == deep){ //Prints in normal form
                                    printMoveScore(deep-depth, list->moves[k], 0, goal);
                                }
                                //Reverts players turn
                                plTurn--;
                                //Reverts the board back to last state
                                revertBoard(b4);
                                scoreIdx++;
                            }
                        }
                    }
                }
                fir--;
            }
        }else{
            //For black players turn
            int count = 0;
            int fir = 8;
            int sec = 0;
            while(1){
                if(count == 8){
                    break;
                }
                if(fir == 0){
                    fir = 8;
                    sec++;
                    count++;
                }
                 //Checks board until we get a black pawn of king
                 if(boardF[fir][sec] == 'b' || boardF[fir][sec] == 'B'){
                     //Simulates the moves of the piece
                     int n = simulateMoves(fir,sec, list);
                     //Runs all the moves to determine if valid
                     for(int k = 0; k< n; k++){
                         if(list->moves[k][0] != '\0'){
                             forRank = 1;
                             if(moveListCheck(list->moves[k]) == 0){
                             }else{ //If move is valid then process score
                                 for(int i = 0; i < 8; i++){
                                     strcpy(nextBoard[i], boardF[i]);
                                     strcpy(b4[i], beforeBoard[i]);
                                 }
                                 //Process score
                                 goal = getScore(1);
                                 if(goal == 99 && depth != 0){
                                     final = 1;
                                 }
                                 scores[scoreIdx] = goal;
                                 //If verbose is active print in verbose form
                                 if(verbose == 1){
                                     if(depth != 0){
                                         printVerboseForm(oppDepth, list->moves[k], 1);
                                     }
                                 }
                                 //Process until depth is reached and game isnt over
                                 if(depth != 0 && goal != 99){
                                     oppDepth++;
                                     //Recursive call to method to determine opposing score from current move
                                     goal = recursiveGodMethod(goal, depth-1, playerTurn+1, nextBoard);
                                     goal = -goal;
                                     scores[scoreIdx] = goal;
                                     oppDepth--;
                                 }
                                 //Prints in verbose if verbose is active
                                 if(verbose == 1){
                                     printMoveScore(deep-depth, list->moves[k], 1, goal);
                                 }else if(verbose == 0 && depth == deep){
                                     printMoveScore(deep-depth, list->moves[k], 1, goal);
                                 }
                                 //Reverts players turn
                                 plTurn--;
                                 //Reverts the board back to last state
                                 revertBoard(b4);
                                 scoreIdx++;
                             }
                         }
                     }
                 }
                 fir--;
             }
        }
    }
    int result = -99;
    for(int i = 0; i < scoreIdx; i++){
        if(result < scores[i]){
            result = scores[i];
        }
    }
    return result;
}
