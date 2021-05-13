#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/**
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 */

char brd[64], boardF[8][8];
char* move, **moveList, **illegalMoveList;
int idx, cnt, jumpRule, moveListIdx, illgalMoveListIdx, numberOfMoves = EOF;
char jump[20], capture[20], pTurn[20];
int plTurn = 0;
char beforeBoard[8][8];
int forRank = 0;

/**
 * This function takes in a pointer to a char array as a parameter and runs through it to see if it contains "RULES:".
 * Returns 1 if true, else 0.
 */
int rulesCheck(char *fileLine){
    size_t strLength = strlen(fileLine);
    char str[6] = "RULES:";
    int searchLength = 6;
    int pos = 0;
    for(int i = 0; i < (int) strLength; i++){
        if(fileLine[i] == '#'){
            break;
        }
        if(fileLine[i] == str[pos]){
            pos++;
            if(pos == searchLength){
                return 1;
            }
        }else{
            pos = 0;
        }
    }
    return 0;
}

/**
 * This function takes in a pointer to a char array as a parameter and runs through it to see if it contains "TURN:".
 * Returns 1 if ture, else 0.
 */
int turnCheck(char *fileLine){
    size_t strLength = strlen(fileLine);
    char str[5] = "TURN:";
    int pos = 0;
    for(int i = 0; i < (int) strLength; i++){
        if(fileLine[i] == '#'){
            break;
        }
        if(fileLine[i] == str[pos]){
            pos++;
            if(pos == 5){
                return 1;
            }
        }else{
            pos = 0;
        }
    }
    return 0;
}

/**
 * This function takes in a pointer to a char array as a parameter and runs through it to see if is
 * important for evaluating the turn. Returns 0 if false, 1 if reds turn and 2 if blacks turn.
 */
int turn(char *fileLine){
    size_t strLength = strlen(fileLine);
    char red[3] = "red";
    char black[5] = "black";
    int pos = 0;
    for(int i = 0; i < (int) strLength; i++){
        if(fileLine[i] == '#'){
            break;
        }
        if(fileLine[i] == black[pos]){
            pos++;
            if(pos == 5){
		        plTurn = 1;
                return 2;
            }
        }else if(fileLine[i] == red[pos]){
            pos++;
            if(pos == 3){
		        plTurn = 0;
                return 1;
            }
        }else{
            pos = 0;
        }
    }
    return 0;
}

/**
 * This function takes in a pointer to a char array as a parameter and runs through it to see if it contains "BOARD:".
 * Returns 1 if true, else 0.
 */
int checkBoard(char *fileLine){
    size_t strLength = strlen(fileLine);
    char str[6] = "BOARD:";
    int pos = 0;
    for(int i = 0; i < (int) strLength; i++){
        if(fileLine[i] == '#'){
            break;
        }
        if(fileLine[i] == str[pos]){
            pos++;
            if(pos == 6){
                return 1;
            }
        }else{
            pos = 0;
        }
    }
    return 0;
}

/**
 * This function takes in a pointer to a char array as a parameter and runs through it to see if it contains all
 * elements needed for the board to be valid. Returns 1 if valid, else 0.
 */
int boardValidation(char *board){
    int blacks = 0;
    int reds = 0;
    char c;
    if(board[0] != '"' || board[63] != '"' || board[16] != '"' || board[32] != '"' || board[48] != '"' ||
       board[15] != '"' || board[31] != '"' || board[47] != '"'){
        return 0;
    }
    for(int i = 0; i < 64; i++){
        c = board[i];
        if(i == 63){
            if(board[i]== '"'){
                break;
            }
        }
        if(c == '"' && i != 15 && i != 31 && i != 47){
            if(board[i + 1] == '"'){
                return 0;
            }
        }else if(c == '.' && i != 7 && i != 23 && i != 39 && i != 55){
            if(board[i + 1] != '"'){
                return 0;
            }
        }else if((c == 'b' || c == 'q')  && i != 7 && i != 23 && i != 39 && i != 55){
            if(board[i + 1] != '"'){
                return 0;
            }
        }else if((c == 'r' || c == 'n')  && i != 7 && i != 23 && i != 39 && i != 55){
            if(board[i + 1] != '"'){
                return 0;
            }
        }
        if(c == 'b' || c == 'q'){
            blacks++;
        }
        if(c == 'r' || c == 'n'){
            reds++;
        }
    }
    if(blacks > 12 || reds > 12){
        return 0;
    }else{
        return 1;
    }
}

/**
 * This function takes in a pointer to a char array as a parameter and runs through it to see if it contains "MOVES:".
 * Returns 1 if true, else 0.
 */
int checkMoves(char *fileLine){
    size_t strLength = strlen(fileLine);
    char str[6] = "MOVES:";
    int pos = 0;
    for(int i = 0; i < (int) strLength; i++){
        if(fileLine[i] == '#'){
            break;
        }
        if(fileLine[i] == str[pos]){
            pos++;
            if(pos == 6){
                return 1;
            }
        }else{
            pos = 0;
        }
    }
    return 0;
}


/**
 * This function takes in a char for determining whether a invalid statement gets printed to standard error and an
 * integer to determine what line the error occurs.
 */
int validation(char c, int line){
    if(c == 't'){
        fprintf(stderr, "Error near line ");
        fprintf(stderr, "%d", line);
        fprintf(stderr, ": expecting 'TURN:' \n");
        return 0;
    }
    if(c == 'p'){
        fprintf(stderr, "Error near line ");
        fprintf(stderr, "%d", line);
        fprintf(stderr, ": expecting 'red' or 'black' \n");
        return 0;
    }
    if (c == 'n') {
        fprintf(stderr, "Error near line ");
        fprintf(stderr, "%d", line);
        fprintf(stderr, ": expecting 'single jumps' or 'multiple jumps' \n");
        return 0;
    }
    if (c == 'g') {
        fprintf(stderr, "Error near line ");
        fprintf(stderr, "%d", line);
        fprintf(stderr, ": expecting 'capture' or 'no capture' \n");
        return 0;
    }
    if(c == 'e'){
        fprintf(stderr, "Error near line ");
        fprintf(stderr, "%d", line);
        fprintf(stderr, ": unexpected end of file \n");
        return 0;
    }
    if(c == 'i'){
        fprintf(stderr, "Error near line ");
        fprintf(stderr, "%d", line);
        fprintf(stderr, ": illegal move \n");
        return 0;
    }
    return 1;
}

/**
 * Gives row number from letter input
 */
int boardCol(char letter){
    if(letter == 'a'){
        return 0;
    }else if(letter == 'b'){
        return 1;
    }else if(letter == 'c'){
        return 2;
    }else if(letter == 'd'){
        return 3;
    }else if(letter == 'e'){
        return 4;
    }else if(letter == 'f'){
        return 5;
    }else if(letter == 'g'){
        return 6;
    }else if(letter == 'h'){
        return 7;
    }else{
        return 8;
    }
}

/**
 * Gives row number from letter input
 */
int boardRow(char number){
    if(number == '8'){
        return 0;
    }else if(number == '7'){
        return 1;
    }else if(number == '6'){
        return 2;
    }else if(number == '5'){
        return 3;
    }else if(number == '4'){
        return 4;
    }else if(number == '3'){
        return 5;
    }else if(number == '2'){
        return 6;
    }else if(number == '1'){
        return 7;
    }else{
        return 8;
    }
}

/**
 * This function takes in a pointer to a char array as a parameter and runs through it to see if the rules for capture are present.
 * This is called if ruleCheck returns 1. Returns 0 if rule is not found, 1 if capture and 2 if no capture.
 */
int captureRules(char *fileLine){
    int answer = 0;
    char ans[10];
    size_t strLength = strlen(fileLine);
    char str[7] = "capture";
    int searchLength = 7;
    int pos = 0;
    int index;
    for(int i = 0; i < (int) strLength; i++){
        if(fileLine[i] == '#'){
            break;
        }
        if(fileLine[i] == str[pos]){
            pos++;
            if(pos == searchLength){
                strcpy(ans, "gottem");
                index = i;
                break;
            }
        }else{
            pos = 0;
        }
    }

    char no[2] = "no";
    index = index + 2;
    if(ans[0] == 'g'){
        if(fileLine[index] == no[0] && fileLine[index + 1] == no[1]){
            return 0;
        }
    }
    index = index - 11;
    if( ans[0] == 'g'){
        if(fileLine[index] == no[0] && fileLine[index + 1] == no[1]) {
            answer = 2;
        }else{
            answer = 1;
        }
    }else{
        answer = 0;
    }
    if(answer == 2){
        strcpy(capture, "no capture");
    }else if(answer == 1){
        strcpy(capture, "capture");
    }
    return answer;
}

/**
 * This function takes in a pointer to a char array as a parameter and runs through it to see if the
 * rules for jump are present. Returns 0 is not, 1 if single jumps and 2 if multiple jumps.
 */
int jumpRules(char *fileLine){
    int answer = 0;
    char ans[10];
    size_t strLength = strlen(fileLine);
    char str[5] = "jumps";
    int pos = 0;
    int index;
    for(int i = 0; i < (int) strLength; i++){
        if(fileLine[i] == '#'){
            break;
        }
        if(fileLine[i] == str[pos]){
            pos++;
            if(pos == 5){
                strcpy(ans, "gottem");
                index = i;
                break;
            }
        }else{
            pos = 0;
        }
    }

    if(ans[0] == 'g'){
        char single[10];
        char multiple[10];
        strcpy(single, "single");
        strcpy(multiple, "multiple");
        index = index - 11;
        if(fileLine[index] == single[0] && fileLine[index + 1] == single[1]) {
            answer = 1;
        }else if(fileLine[index - 2] == multiple[0] && fileLine[index - 1] == multiple[1]){
            answer = 2;
        }
    }else{
        answer = 0;
    }
    if(answer == 1){
        strcpy(jump, "single jumps");
    }else if(answer == 2){
        strcpy(jump, "multiple jumps");
    }
    return answer;
}

/**
 * This function takes in a pointer to a char array as a parameter and runs through it and finds "->".
 * After finding this, it determines if a move has numbers and letters between 1-8 and A-H.
 */
int moves(char *fileLine){
    if(moveListIdx == 0){
        int count = 0;
        illegalMoveList = malloc(sizeof(char)*6*150);
        illgalMoveListIdx = 0;
        moveList = malloc(sizeof(char)*6*150);
        while(count < 150){
            *(moveList + count) = malloc(sizeof(char)*6);
            *(illegalMoveList + count) = malloc(sizeof(char)*6);
            count++;
        }
    }
    move = malloc(sizeof(char)*30);
    int pos = 0;
    int answer = 1;
    int index;
    size_t stringLength = strlen(fileLine);
    char letters[8] = "abcdefgh";
    char numbers[8] = "12345678";
    char str[2] = "->";
    for(int i = 0; i < (int) stringLength; i++){
        if(fileLine[i] == '#'){
            break;
        }else if(fileLine[i] == str[pos]){
            pos++;
            if(pos == 2){
                *(move + 0) = fileLine[i-3]; *(move + 1) = fileLine[i-2]; *(move + 2) = fileLine[i-1];
                *(move + 3) = fileLine[i]; *(move + 4) = fileLine[i+1]; *(move + 5) = fileLine[i+2];
                int count = 0;
                //UPDATES THE MOVES LIST WITH CURRENT MOVE
                while(count < 6){
                    *(*(moveList + moveListIdx) + count) = *(move + count);
                    count++;
                }
                moveListIdx++;
                index = i - 3;
                if((fileLine[index] != letters[0]) && (fileLine[index] != letters[1]) && (fileLine[index] != letters[2]) && (fileLine[index] != letters[3]) &&
                   (fileLine[index] != letters[4]) && (fileLine[index] != letters[5]) && (fileLine[index] != letters[6]) && (fileLine[index] != letters[7]))  {
                    return 0;
                }else if((fileLine[index + 1] != numbers[0]) && (fileLine[index + 1] != numbers[1]) && (fileLine[index + 1] != numbers[2]) && (fileLine[index + 1] != numbers[3]) &&
                         (fileLine[index + 1] != numbers[4]) && (fileLine[index + 1] != numbers[5]) && (fileLine[index + 1] != numbers[6]) && (fileLine[index + 1] != numbers[7])){
                    return  0;
                }
                index = i + 1;
                if((fileLine[index] != letters[0]) && (fileLine[index] != letters[1]) && (fileLine[index] != letters[2]) && (fileLine[index] != letters[3]) &&
                   (fileLine[index] != letters[4]) && (fileLine[index] != letters[5]) && (fileLine[index] != letters[6]) && (fileLine[index] != letters[7]))  {
                    return 0;
                }else if((fileLine[index + 1] != numbers[0]) && (fileLine[index + 1] != numbers[1]) && (fileLine[index + 1] != numbers[2]) && (fileLine[index + 1] != numbers[3]) &&
                         (fileLine[index + 1] != numbers[4]) && (fileLine[index + 1] != numbers[5]) && (fileLine[index + 1] != numbers[6]) && (fileLine[index + 1] != numbers[7])){
                    return  0;
                }
            }
        }else{
            pos = 0;
        }
    }
    return answer;
}

/**
 * Copies the board into a char array while also validating if the whole board is present of not
 */
void boardCopy(char* fileLine, int index, int count){
    int done = 0;
    // if validator was updated to "array continue" to show that the board is not yet full
    size_t strLength = strlen(fileLine);
    //Copies board elements from file string to the board array
    for(int i = 0; i < (int) strLength; i++){
        if(fileLine[i] == '#'){
            break;
        }else if(fileLine[i] == '"'){
                cnt++;
                brd[idx] = '"';
                idx++;
                done++;
        }else if(brd[0] == '"') {
            if(fileLine[i] == '"'){
                cnt++;
                brd[idx] = '"';
                idx++;
                done++;
            }else if(fileLine[i] == 'b'){
                cnt++;
                brd[idx] = 'b';
                idx++;
                done++;
            }else if(fileLine[i] == 'B'){
		        cnt++;
		        brd[idx] = 'B';
		        idx++;
		        done++;
	        }else if(fileLine[i] == '.'){
                cnt++;
                brd[idx]  = '.';
                idx++;
                done++;
            }else if(fileLine[i] == 'r'){
                cnt++;
                brd[idx]  = 'r';
                idx++;
                done++;
            }else if(fileLine[i] == 'R'){
		        cnt++;
		        brd[idx] = 'R';
		        idx++;
		        done++;
	        }else if(fileLine[i] == '-'){
            }else if(fileLine[i] == '+'){
            }else if(fileLine[i] == '|'){
            }else if(fileLine[i] == 32){
	        }else if(fileLine[i] == '\r'){
            }else if(fileLine[i] == '\n') {
            }else if(done == 8){
                break;
            }else if(brd[63] == '"'){
                break;
            }else{
                idx = 0;
                break;
            }
        }
    }
}

/**
 * Makes board easier to follow with an 8x8 format
 */
void boardFormat(){
    int count = 0;
    int i = 0;
    while(count != 64){
        for(int j = 0; j < 8; j++){
            boardF[i][j] = brd[count];
            count++;
        }
        i++;
    }
}

/**
 * Checks if move stays within the black spots of the board. Returns 0 if false, else returns 1.
 */
int staysOnBlack(char* m){
    if(((*(m + 0) == 'a' && ((*(m + 1) == '7') || (*(m + 1) == '5') || (*(m + 1) == '3') || (*(m + 1) == '1')))
        || (*(m + 0) == 'b' && ((*(m + 1) == '8') || (*(m + 1) == '6') || (*(m + 1) == '4') || (*(m + 1) == '2')))
        || (*(m + 0) == 'c' && ((*(m + 1) == '7') || (*(m + 1) == '5') || (*(m + 1) == '3') || (*(m + 1) == '1')))
        || (*(m + 0) == 'd' && ((*(m + 1) == '8') || (*(m + 1) == '6') || (*(m + 1) == '4') || (*(m + 1) == '2')))
        || (*(m + 0) == 'e' && ((*(m + 1) == '7') || (*(m + 1) == '5') || (*(m + 1) == '3') || (*(m + 1) == '1')))
        || (*(m + 0) == 'f' && ((*(m + 1) == '8') || (*(m + 1) == '6') || (*(m + 1) == '4') || (*(m + 1) == '2')))
        || (*(m + 0) == 'g' && ((*(m + 1) == '7') || (*(m + 1) == '5') || (*(m + 1) == '3') || (*(m + 1) == '1')))
        || (*(m + 0) == 'h' && ((*(m + 1) == '8') || (*(m + 1) == '6') || (*(m + 1) == '4') || (*(m + 1) == '2')))) &&
       ((*(m + 4) == 'a' && ((*(m + 5) == '7') || (*(m + 5) == '5') || (*(m + 5) == '3') || (*(m + 1) == '1')))
        || (*(m + 4) == 'b' && ((*(m + 5) == '8') || (*(m + 5) == '6') || (*(m + 5) == '4') || (*(m + 5) == '2')))
        || (*(m + 4) == 'c' && ((*(m + 5) == '7') || (*(m + 5) == '5') || (*(m + 5) == '3') || (*(m + 5) == '1')))
        || (*(m + 4) == 'd' && ((*(m + 5) == '8') || (*(m + 5) == '6') || (*(m + 5) == '4') || (*(m + 5) == '2')))
        || (*(m + 4) == 'e' && ((*(m + 5) == '7') || (*(m + 5) == '5') || (*(m + 5) == '3') || (*(m + 5) == '1')))
        || (*(m + 4) == 'f' && ((*(m + 5) == '8') || (*(m + 5) == '6') || (*(m + 5) == '4') || (*(m + 5) == '2')))
        || (*(m + 4) == 'g' && ((*(m + 5) == '7') || (*(m + 5) == '5') || (*(m + 5) == '3') || (*(m + 5) == '1')))
        || (*(m + 4) == 'h' && ((*(m + 5) == '8') || (*(m + 5) == '6') || (*(m + 5) == '4') || (*(m + 5) == '2'))))){
        return 1;
    }else{
        return 0;
    }
}

/*
 * Checks if piece is incrementing rank correctly for a regular move. Returns 0 if false, else returns 1.
 */
int regMove(char* m){
    int row = boardRow(*(m + 1));
    int col = boardCol(*(m + 0));
    int row2 = boardRow(*(m + 5));
    int col2 = boardCol(*(m + 4));

    if(plTurn %2 == 0){ //Case where it is reds turn

        if(boardF[row][col] != 'r'){ //Checks for correct piece
            if(boardF[row][col] != 'R'){
                return 0;
            }else{
                if(boardF[row2][col2] != '.'){
                    return 0;
                }else{
                    if((*(m + 0) == 'a' && *m + 4 == 'b') ||
                       (*(m + 0) == 'b' && ((*(m + 4) == 'a') || (*(m + 4) == 'c'))) ||
                       (*(m + 0) == 'c' && ((*(m + 4) == 'b') || (*(m + 4) == 'd'))) ||
                       (*(m + 0) == 'd' && ((*(m + 4) == 'c') || (*(m + 4) == 'e'))) ||
                       (*(m + 0) == 'e' && ((*(m + 4) == 'd') || (*(m + 4) == 'f'))) ||
                       (*(m + 0) == 'f' && ((*(m + 4) == 'e') || (*(m + 4) == 'g'))) ||
                       (*(m + 0) == 'g' && ((*(m + 4) == 'f') || (*(m + 4) == 'h'))) ||
                       (*(m + 0) == 'h' && ((*(m + 4) == 'g')))){
                        for(int i = 0; i < 8; i++){
                            strcpy(beforeBoard[i], boardF[i]);
                        }
                        boardF[row2][col2] = 'R';
                        boardF[row][col] = '.';
                        return 1;
                    }else{
                        return 0;
                    }
                }
            }
        }

        //Determines if the distance traveled is correct
        if(*(m + 1) < *(m + 5)){
            if((*(m + 0) == 'a' && *m + 4 == 'b') ||
               (*(m + 0) == 'b' && ((*(m + 4) == 'a') || (*(m + 4) == 'c'))) ||
               (*(m + 0) == 'c' && ((*(m + 4) == 'b') || (*(m + 4) == 'd'))) ||
               (*(m + 0) == 'd' && ((*(m + 4) == 'c') || (*(m + 4) == 'e'))) ||
               (*(m + 0) == 'e' && ((*(m + 4) == 'd') || (*(m + 4) == 'f'))) ||
               (*(m + 0) == 'f' && ((*(m + 4) == 'e') || (*(m + 4) == 'g'))) ||
               (*(m + 0) == 'g' && ((*(m + 4) == 'f') || (*(m + 4) == 'h'))) ||
               (*(m + 0) == 'h' && ((*(m + 4) == 'g')))){
                if(boardF[row2][col2] != '.'){ //Case where end spot isn't empty
                    return 0;
                }else{
                    if(row2 == 0){ //Case where pawn turns into king
                        for(int i = 0; i < 8; i++){
                            strcpy(beforeBoard[i], boardF[i]);
                        }
                        boardF[row2][col2] = 'R';
                        boardF[row][col] = '.';
                    }else{ //Regular move
                        for(int i = 0; i < 8; i++){
                            strcpy(beforeBoard[i], boardF[i]);
                        }
                        boardF[row2][col2] = 'r';
                        boardF[row][col] = '.';
                    }
                }
                return 1;
            }else{
                return 0;
            }
        }
    }else{ //Case where it is blacks turn
        if(boardF[row][col] != 'b'){
            if(boardF[row][col] != 'B'){
                return 0;
            }else{
                if(boardF[row2][col2] != '.'){
                    return 0;
                }else{
                    if((*(m + 0) == 'a' && *(m + 4) == 'b') ||
                       (*(m + 0) == 'b' && ((*(m + 4) == 'a') || (*(m + 4) == 'c'))) ||
                       (*(m + 0) == 'c' && ((*(m + 4) == 'b') || (*(m + 4) == 'd'))) ||
                       (*(m + 0) == 'd' && ((*(m + 4) == 'c') || (*(m + 4) == 'e'))) ||
                       (*(m + 0) == 'e' && ((*(m + 4) == 'd') || (*(m + 4) == 'f'))) ||
                       (*(m + 0) == 'f' && ((*(m + 4) == 'e') || (*(m + 4) == 'g'))) ||
                       (*(m + 0) == 'g' && ((*(m + 4) == 'f') || (*(m + 4) == 'h'))) ||
                       (*(m + 0) == 'h' && ((*(m + 4) == 'g')))){
                        for(int i = 0; i < 8; i++){
                            strcpy(beforeBoard[i], boardF[i]);
                        }
                        boardF[row2][col2] = 'B';
                        boardF[row][col] = '.';
                        return 1;
                    }else{
                        return 0;
                    }
                }
            }
        }

        //Determines if the distance traveled is correct
        if(*(m + 1) > *(m + 5)){
            if((*(m + 0) == 'a' && *(m + 4) == 'b') ||
               (*(m + 0) == 'b' && ((*(m + 4) == 'a') || (*(m + 4) == 'c'))) ||
               (*(m + 0) == 'c' && ((*(m + 4) == 'b') || (*(m + 4) == 'd'))) ||
               (*(m + 0) == 'd' && ((*(m + 4) == 'c') || (*(m + 4) == 'e'))) ||
               (*(m + 0) == 'e' && ((*(m + 4) == 'd') || (*(m + 4) == 'f'))) ||
               (*(m + 0) == 'f' && ((*(m + 4) == 'e') || (*(m + 4) == 'g'))) ||
               (*(m + 0) == 'g' && ((*(m + 4) == 'f') || (*(m + 4) == 'h'))) ||
               (*(m + 0) == 'h' && ((*(m + 4) == 'g')))){
                if(boardF[row2][col2] != '.'){ //Case where end spot isn't empty
                    return 0;
                }else{
                    if(row2 == 7){ //Case where pawn turns into king
                        for(int i = 0; i < 8; i++){
                            strcpy(beforeBoard[i], boardF[i]);
                        }
                        boardF[row2][col2] = 'B';
                        boardF[row][col] = '.';

                    }else{ //Regular move
                        for(int i = 0; i < 8; i++){
                            strcpy(beforeBoard[i], boardF[i]);
                        }
                        boardF[row2][col2] = 'b';
                        boardF[row][col] = '.';

                    }
                }
                return 1;
            }else{
                return 0;
            }
        }
    }
    return 0;
}

/**
 * Performs a jump move. Returns 0 if move is illegal, else return 1.
 */
int playerJump(char* m, int turn){
    int row1 = boardRow(*(m + 1));
    int col1 = boardCol(*(m + 0));
    int row2 = boardRow(*(m + 5));
    int col2 = boardCol(*(m + 4));
    int middleRow;
    int middleCol;

    //If the move goes beyond the boarder of our playing board
    if((row1 > 7 || row1 < 0) || (row2 > 7 || row2 < 0) || (col1 > 7 || col1 < 0) || (col2 > 7 || col2 < 0)){
        return 0;
    }

    //Sets the value for middleRow
    if(row1 - 2 == row2){
        middleRow = row1 - 1;
    }else if(row1 + 2 == row2){
        middleRow = row1 + 1;
    }else{
        return 0;
    }

    //Sets the value for middleCol
    if(col1 - 2 == col2){
        middleCol = col1 - 1;
    }else if(col1 + 2 == col2){
        middleCol = col1 + 1;
    }else{
        return 0;
    }

    //Case where it is reds turn
    if(turn %2 == 0){

        //Case where piece is not a red piece
        if(boardF[row1][col1] != 'r' && boardF[row1][col1] != 'R'){
            return 0;
        }

        //Case where landing spot from jump is not empty
        if(boardF[row2][col2] != '.'){
            return 0;
        }

        //Case where there is no black piece in the middle spot
        if(boardF[middleRow][middleCol] != 'b' && boardF[middleRow][middleCol] != 'B'){
                return 0;
        }

        //Case where pawn goes the wrong way
	    if(boardF[row1][col1] == 'r' && row2 > row1){
	        return 0;
	    }

	    //Moving the piece
        if(boardF[row1][col1] == 'r'){
            for(int i = 0; i < 8; i++){
                strcpy(beforeBoard[i], boardF[i]);
            }
            boardF[row2][col2] = 'r';
            if(row2 == 0){ //Case where pawn turns into king
                boardF[row2][col2] = 'R';
            }
        }else{
            for(int i = 0; i < 8; i++){
                strcpy(beforeBoard[i], boardF[i]);
            }
            boardF[row2][col2] = 'R';
        }
        boardF[middleRow][middleCol] = '.';
        boardF[row1][col1] = '.';
    }else{ //Case where it is blacks turn

        //Case where piece is not black
        if(boardF[row1][col1] != 'b' && boardF[row1][col1] != 'B'){
            return 0;
        }

        //Case where landing spot is not empty
        if(boardF[row2][col2] != '.'){
            return 0;
        }

        //Case where middle spot has no red piece
        if(boardF[middleRow][middleCol] != 'r' && boardF[middleRow][middleCol] != 'R'){
                return 0;
        }

        //Case where pawn goes the wrong direction
	    if(boardF[row1][col1] == 'b' && row1 > row2){
	        return 0;
	    }

	    //Moving the piece
        if(boardF[row1][col1] == 'b'){
            for(int i = 0; i < 8; i++){
                strcpy(beforeBoard[i], boardF[i]);
            }
            boardF[row2][col2] = 'b';
            if(row2 == 7){ //Case where pawn turns into king
                boardF[row2][col2] = 'B';
            }
        }else{
            for(int i = 0; i < 8; i++){
                strcpy(beforeBoard[i], boardF[i]);
            }
            boardF[row2][col2]= 'B';
        }
        boardF[middleRow][middleCol] = '.';
        boardF[row1][col1] = '.';
    }
    return 1;
}

/**
 * Moves a move from the moveList to the illegal move list.
 */
void moveToIllegal(char* m){
    int count = 0;
    while(count < 6){
        *(*(illegalMoveList + illgalMoveListIdx) + count) = *(m + count);
        count++;
    }
    illgalMoveListIdx++;
}

/**
 * Performs the move that is brought in as a parameter. Runs that move through several methods to determine if the move is legal. If any method returns 0
 * moveListCheck returns 0 after first sending move to illegal move list using moveToIllegal method.
 */
int moveListCheck(char* m){
    int row1 = boardRow(*(m + 1));
    int row2 = boardRow(*(m + 5));

    //Checks if piece stays on black
    if(staysOnBlack(m) == 0){
        if(forRank == 0){
            moveToIllegal(m);
        }else{
            forRank = 0;
        }
        return 0;
    }
    //Checks if regular move or jump
    if(row1 == row2+1 || row1 == row2-1){ //Case of a regular move
        if(regMove(m) == 0){
            if(forRank == 0){
                moveToIllegal(m);
            }else{
                forRank = 0;
            }
            return 0;
        }else{
            plTurn++;
            return 1;
        }
    }else if(row1 == row2+2 || row1 == row2-2){ //Case of a jump
        if(playerJump(m, plTurn) == 0){
            if(forRank == 0){
                moveToIllegal(m);
            }else{
                forRank = 0;
            }
            return 0;
        }else{
            plTurn++;
            return 1;
        }
    }
    //Moves move to illegal move list
    if(forRank == 0){
        moveToIllegal(m);
    }else{
        forRank = 0;
    }
    return 0;
}

/**
 * Method that copies the rules into our designated rule string values
 */
void setJumpRule(char* jumpRule, char* captureRule){
    strcpy(jump, jumpRule);
    strcpy(capture, captureRule);
}
