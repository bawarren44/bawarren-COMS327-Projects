#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "term.h"
#include <stdarg.h>
#include <termbox.h>

/**
 *Author: Brad Warren
 *Id Number:184030907
 *Email: bawarren@iastate.edu
 */

int boardCountX = 1;
int boardCountY = 0;
int changed = 0;
int pageEnd = 0;
int movesPerPage = 0;
int curPage = 1;

/**
 * Sets up the Red tiles of the board display
 */
void makeRedBoard(){
	struct tb_cell* redSpace;
	redSpace = malloc(sizeof(struct tb_cell));
	redSpace->bg = TB_RED;
	redSpace->fg = TB_WHITE;
	uint32_t uni;
	redSpace->ch = ' ';

	int right = tb_width()/2;
	int down = 1;
	int holder = right;
	int q = 0;

	//Prints to each red sqaure
	for(int i = 0; i<32; i++){
		if(right == (holder+35)){
			down = down + 3;
			right = (tb_width()/2) + 5;
		}else if(right == (holder+40)){
			down = down + 3;
			right = tb_width()/2;
		}else if(q != 0){
			right = right +5;
		}
		int x = down;
		int y = right;
		q++;
		//Fills the red cell
		for(;;){
			if(down == x + 3){
				down = x;
				right++;
			}
			if(right == y + 5){
				break;
			}
			tb_put_cell(right,down,redSpace);
			down++;
		}
	}
}

/**
 * Helper method for printf_tb
 */
void print_tb(const char *str, int x, int y, uint16_t fg, uint16_t bg){
	while(*str){
		uint32_t uni;
		str += tb_utf8_char_to_unicode(&uni,str);
		tb_change_cell(x,y,uni,fg,bg);
		x++;
	}
}

/**
 * Used to print into a cell with any string input
 */
void printf_tb(int x, int y, uint16_t fg, uint16_t bg, const char *fmt,...){
	char buf[4096];
	va_list vl;
	va_start(vl,fmt);
	vsnprintf(buf, sizeof(buf), fmt, vl);
	va_end(vl);
	print_tb(buf, x,y,fg,bg);
}

/**
 * Prints the moves onto the screen in order that they come in
 * @param t players turn
 * @param i index of moveList
 */
void printMoves(char t, int i){
	int x = 0;
	int y = 6;
	int countNextLine = 0;
	//Determines the value of x by player turn
	if(t == 'r'){
		x = 0;
	}else{
		x = 1;
	}

	//Prints each move until either the moveList index is done or we hit the display height
	for(i; i < moveListIdx; i++){
		if(countNextLine % 2 == 0){
			y++;
		}
		if(y == tb_height() -2){
			pageEnd = y;
			break;
		}else{

		    //Prints to reds side
		if(x % 2 == 0){
			if(here[i]->good == 0){
				printf_tb(((tb_width()/2)-7),y,TB_GREEN,TB_DEFAULT,(*(moveList + i)));
			}else if(here[i]->good == 1){
				printf_tb(((tb_width()/2)-7),y,TB_RED,TB_DEFAULT,(*(moveList + i)));
			}else{
				printf_tb(((tb_width()/2)-7),y,TB_WHITE,TB_DEFAULT,(*(moveList + i)));
			}
			countNextLine++;
		}else{
		    //Prints to black side
			if(here[i]->good == 0){
				printf_tb(0,y,TB_GREEN,TB_DEFAULT,(*(moveList + i)));
			}else if(here[i]->good == 1){
				printf_tb(0,y,TB_RED,TB_DEFAULT,(*(moveList + i)));
			}else{
				printf_tb(0,y,TB_WHITE,TB_DEFAULT,(*(moveList + i)));
			}
			countNextLine++;
		}
		}
		x++;
	}

	//Removes the left over unused cells from last page
	if(y != tb_height()-2 && curPage != 1){
		char b[7];
		for(int i = 0; i < 7; i++){
			b[i] = ' ';
		}
		if(x %2 == 0){
			if(t == 'b'){
				printf_tb(tb_width()/2-7,y,TB_DEFAULT,TB_DEFAULT, b);
			}
			y++;
			for(;;){
				if(y == tb_height()-2){
					break;
				}
				printf_tb(tb_width()/2-7,y,TB_DEFAULT,TB_DEFAULT,b);
				printf_tb(0,y,TB_DEFAULT,TB_DEFAULT,b);
				y++;
			}
		}else{
			if(t == 'r'){
				printf_tb(0,y,TB_DEFAULT,TB_DEFAULT,b);	
			}
			y++;
			for(;;){
				if(y == tb_height()-2){
					break;
				}	
				printf_tb(0,y,TB_DEFAULT,TB_DEFAULT, b);
				printf_tb(tb_width()/2-7,y,TB_DEFAULT,TB_DEFAULT, b);
				y++;
			}
		}
	}
}

/**
 * Gives x and y location (cell) for each value in the here struct
 * @param c
 */
void giveCells(char c){
    //If it is reds turn first
	if(c == 'r'){
		int count = 0;
		int x;
		int xw = (tb_width()/2)-7;
		int xs = 0;
		int y = 7;
		//Give x and y for first index of here
		here[0]->x = xw;
		here[0]->y = y;
		count++;
		//Give x and y to remaining here values
		for(int i = 1; i< hereIdx+1; i++){
			if(count %2 == 0){
				x = xw;
				y++;
				here[i]->x = x;
				here[i]->y  = y;
			}else{
				x = xs;
				here[i]->x = x;
				here[i]->y = y;
			}
			count++;
		}
	}else{ //If it is blacks turn first
		int count = 0;
		int x;
		int xw = (tb_width()/2)-7;
		int xs = 0;
		int y = 7;
		//Gives x and y to first index of here
		here[0]->x = xs;
		here[0]->y = y;
		count++;
		//Gives x and y to remaining values
		for(int i =1; i<hereIdx+1; i++){
			if(count%2 == 0){
				x = xs;
				y++;
				here[i]->x = x;
				here[i]->y = y;
			}else{
				x = xw;
				here[i]->x =x;
				here[i]->y =y;
			}
			count++;
		}
	}
}

/**
 * Gives the cell input to the specific board value
 * @param c board location
 * @return
 */
struct tb_cell* getCell(char c){
	struct tb_cell* cell;
	cell = malloc(sizeof(struct tb_cell));
	cell-> bg = TB_WHITE;
	if(c == 'r'){
		cell-> fg = TB_RED;
		cell-> ch = 'O';
	}else if(c == 'R'){
		cell-> fg = TB_RED;
		cell-> ch = '@';
	}else if(c == 'B'){
		cell-> fg = TB_BLACK;
		cell->ch = '@';
	}else if(c == 'b'){
		cell-> fg = TB_BLACK;
		cell-> ch = 'O';
	}else{
		cell-> fg = TB_DEFAULT;
		cell-> ch = ' ';
	}
	return cell;
}

/**
 * Increments the counter of boardCountx and boardCounty
 * @param y
 * @param x
 */
void incCounter(int y, int x){
	if(y %2 == 0){
		if(x == 7){
			boardCountY = y+ 1;
			boardCountX = 0;
		}else{
			boardCountY = y;
			boardCountX = x+2;
		}
	}else{
		if(x == 6){
			boardCountY = y+ 1;
			boardCountX = 1;
		}else{
			boardCountY = y;
			boardCountX = x+2;
		}
	}
}

/**
 * Sets the board white spaces from the given board input
 * @param b board input
 */
void populateBoard(char b[8][8]){
	int right = tb_width()/2 +5;
	int down = 1;
	int holder = tb_width()/2;
	int q = 0;

	//Cell to be places for empty spots
	struct tb_cell* whiteSpace;
	whiteSpace = malloc(sizeof(struct tb_cell));
	whiteSpace-> bg = TB_WHITE;
	whiteSpace-> fg = TB_WHITE;
	whiteSpace-> ch = ' ';

	//Prints to each of the 32 squares
	for(int i = 0; i<32; i++){
		if(right == (holder + 35)){
			down = down + 3;
			right = (tb_width()/2)+5;
		}else if(right == (holder+40)){
			down = down + 3;
			right = tb_width()/2;
		}else if(q != 0){
			right = right + 5;
		}
		int x = down;
		int y = right;
		q++;
		//Fills the cells of the given location
		for(;;){
			if(down == x + 3){
				down = x;
				right++;
			}
			if(right == y + 5){
				break;
			}
			if(down == x + 1 && right == y + 2){
				tb_put_cell(right,down,getCell(b[boardCountY][boardCountX]));
				incCounter(boardCountY,boardCountX);					down++;
			}else{
				tb_put_cell(right,down,whiteSpace);
				down++;
			}
		}
	}
	changed = 1;
	boardCountY= 0;
	boardCountX = 1;
}

/**
 * Switches the board from one board to another
 * @param lastBoard last board input
 * @param board current board input
 */
void switchBoard(char lastBoard[8][8], char board[8][8]){
	int right = tb_width()/2 + 5;
	int down = 1;
	int holder = tb_width()/2;
	int q = 0;

	//Prints new board from old
	for(int i = 0; i < 32; i++){
		if(right == (holder + 35)){
			down = down + 3;
			right = (tb_width()/2)+5;
		}else if(right == (holder+40)){
			down = down + 3;
			right = tb_width()/2;
		}else if(q != 0){
			right = right + 5;
		}
		int x = down;
		int y = right;
		q++;
		//Fills the board spaces
		for(;;){
			if(down == x + 3){
				down = x;
				right++;
			}
			if(right == y+5){
				break;
			}
			if(down == x + 1 && right == y+2){//If the middle space print a value
				if(lastBoard[boardCountY][boardCountX] != board[boardCountY][boardCountX]){
					char c = board[boardCountY][boardCountX];
					if(c == 'r'){
						tb_change_cell(right,down,'O',TB_RED,TB_WHITE);
					}else if(c == 'R'){
						tb_change_cell(right,down,'@',TB_RED,TB_WHITE);
					}else if(c =='B'){
						tb_change_cell(right,down,'@',TB_BLACK,TB_WHITE);
					}else if(c == 'b'){
						tb_change_cell(right,down,'O',TB_BLACK,TB_WHITE);
					}else {
						tb_change_cell(right,down,' ', TB_WHITE,TB_WHITE);
					}
					down++;
					incCounter(boardCountY,boardCountX);

				}else{
					down++;
					incCounter(boardCountY,boardCountX);
				}
			}else{
				down++;
			}
		}
	}
	boardCountY = 0;
	boardCountX = 1;
}

/**
 * Prints the button display onto the display
 * @param t
 */
void printButtons(char t){
    if(t == 'r'){
        printf_tb(0,1,TB_WHITE,TB_DEFAULT,"RIGHT ARROW: move to previous move");
        printf_tb(0,2,TB_WHITE,TB_DEFAULT,"LEFT ARROW: move to next move");
    }else{
        printf_tb(0,1,TB_WHITE,TB_DEFAULT,"RIGHT ARROW: move to previous move");
        printf_tb(0,2,TB_WHITE,TB_DEFAULT,"LEFT ARROW: move to last move");
    }
    printf_tb(0,3,TB_WHITE,TB_DEFAULT,"ECS: quit");
    printf_tb(0,5,TB_WHITE, TB_DEFAULT, "Black");
    printf_tb(0,6,TB_WHITE,TB_DEFAULT, ":");
    printf_tb(((tb_width())/2)-3,5,TB_WHITE,TB_DEFAULT,"Red");
    printf_tb(((tb_width())/2)-1,6,TB_WHITE,TB_DEFAULT,":");
}

/**
 * Prints the page numbers at the bottom of the display
 * @param x current page
 * @param h total number of pages
 */
void printPage(int x, int h){
	char p[3] = {0};
	p[0] = x + '0';
	p[1] = '/';
	p[2] = h + '0';
	printf_tb(((tb_width()/2)/2),tb_height()-1,TB_WHITE,TB_DEFAULT,p);
}

/**
 * Prints the next page display
 */
void nextPage(int spot, char t){
	printMoves(t,spot+1);
}

/**
 * Prints the last page display
 * @param spot
 * @param t
 */
void lastPage(int spot, char t){
	int num = spot % movesPerPage;
	num = (spot-movesPerPage)-num;
	printMoves(t,num);
}

/**
 * Colors the cursor according to if the current move in the spot is valid == 0, invalid ==1, unknown == 2.
 * @param x x coordinate
 * @param y y coordinate
 * @param spot move index
 */
void colorCursor(int x, int y,int spot){
//Checks if a move is present
    if(hereIdx != 0){
       		if(x == 0){ //If black move
	    		char b[tb_width()/2/2-1]; // Value to be printed into x and y location
     			for(int i = 0; i < 6; i++){
				b[i] = '0';
				b[i] = moveList[spot][i];
    			}
    			for(int i = 6; i < tb_width()/2/2-1; i++){
				b[i] = '0';
				b[i] = ' ';
	    		}
    			//Sets correct color and location of cursor
	    		if(here[spot]->good == 0){
				printf_tb(0,y,TB_BLACK,TB_GREEN,b);
		    	}else if(here[spot]->good == 1){
				printf_tb(0,y,TB_BLACK,TB_RED,b);
		       	}else{
				printf_tb(0,y,TB_BLACK,TB_WHITE,b);
			}
		}else {//If red move
                char n[tb_width() / 2 / 2 - 1]; //Value to be printed into x and y location
                for (int i = 0; i < (tb_width() / 2 / 2 - 1) - 6; i++) {
                    n[i] = '0';
                    n[i] = ' ';
                }
                for (int i = 0; i < 6; i++) {
                    n[((tb_width() / 2 / 2 - 1) - 6) + i] = '0';
                    n[((tb_width() / 2 / 2 - 1) - 6) + i] = moveList[spot][i];
                }
                //Set current color and location
                if (here[spot]->good == 0) {
                    printf_tb(tb_width() / 2 / 2, y, TB_BLACK, TB_GREEN, n);
                } else if (here[spot]->good == 1) {
                    printf_tb(tb_width() / 2 / 2, y, TB_BLACK, TB_RED, n);
                } else {
                    printf_tb(tb_width() / 2 / 2, y, TB_BLACK, TB_WHITE, n);
                }
            }
    }
}

/**
 * Resets the cursor color
 * @param x x coordinate
 * @param y y coordinate
 * @param spot index location of move
 */
void resetCursor(int x, int y, int spot){
		if(x == 0){//Black move
			char b[tb_width()/2/2-1];//Value to be printed into x and y location
			for(int i = 0; i < 6; i++){
				b[i] = '0';
           			b[i] = moveList[spot][i];
        		}
  		        for(int i = 6; i < tb_width()/2/2-1; i++) {
				b[i] = '0';
           			b[i] = ' ';
     			}
  		        //Revert color at location
			if(here[spot]->good == 0){
        			printf_tb(0,y,TB_GREEN,TB_DEFAULT,b);
			}else if(here[spot]->good == 1){
				printf_tb(0,y,TB_RED,TB_DEFAULT,b);
			}else{
				printf_tb(0,y,TB_WHITE,TB_DEFAULT,b);
			}
   		 }else {//Red move
            char n[tb_width() / 2 / 2 - 1]; //Value to be printed into x and y location
            for (int i = 0; i < (tb_width() / 2 / 2 - 1) - 6; i++) {
                n[i] = '0';
                n[i] = ' ';
            }
            for (int i = 0; i < 6; i++) {
                n[((tb_width() / 2 / 2 - 1) - 6) + i] = '0';
                n[((tb_width() / 2 / 2 - 1) - 6) + i] = moveList[spot][i];
            }
            //Revert color at location
            if (here[spot]->good == 0) {
                printf_tb(tb_width() / 2 / 2, y, TB_GREEN, TB_DEFAULT, n);
            } else if (here[spot]->good == 1) {
                printf_tb(tb_width() / 2 / 2, y, TB_RED, TB_DEFAULT, n);
            } else {
                printf_tb(tb_width() / 2 / 2, y, TB_WHITE, TB_DEFAULT, n);
            }
        }
}

/**
 * Main method that is used to create the edit executable. Processes moves as specified from the command line arguments.
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
    int fileThere = 0;
    FILE* file;
    int fileIsThere = 0;
    char name[150];


    for(int i = 1; i < argc; i++){
        if(fileIsThere == 0){
            file = fopen(argv[i], "r");
            if(file != NULL){
                fileIsThere = 1;
                strcpy(name,argv[i]);
            }
        }
    }
    if(fileIsThere == 0){
        file = stdin;
    }

    //Delete later
//    file = fopen("C:\\Users\\sup3r\\coms327\\coms327\\3.txt", "r");

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

    here = malloc(sizeof(struct move)*150);
    for(int i = 0; i < 150; i++){
	here[i] = malloc(sizeof(struct move));
    }
    int illegalMove = 0;
   //Cases when numberOfMoves is not specified
   while(moveListIdxb != moveListIdx){
       if(moveListCheck((*(moveList +  moveListIdxb))) == 0){
	   constructMoves(1,1);
	   illegalMove = moveListIdxb;
           moveListIdxb++;
           //Copies remaining moves after an illegal move to the illegal move list
           while(moveListIdxb != moveListIdx){
		   constructMoves(1,0);
               moveToIllegal(*(moveList + moveListIdxb));
               moveListIdxb++;
           }
           break;
       }
       constructMoves(0,0);
       moveListIdxb++;
   }

    if(moveListIdxb != moveListIdx){
        while(moveListIdxb != moveListIdx){
		constructMoves(1,0);
            //Copies remaining moves after an illegal move to the illegal move list
            moveToIllegal(*(moveList + moveListIdxb));
            moveListIdxb++;
        }
    }

    //Starts termbox
	tb_init();

    //Cell struct
	struct tb_cell cell;
	cell.fg = TB_WHITE;
	cell.bg = TB_YELLOW;
	cell.ch = 'H';

	//Sets the top bar displaying name of file
	int cx = 0,cy = 0;
	printf_tb(0,0,TB_BLACK | TB_BOLD, TB_CYAN,name);
	int p = (int)strlen(name);
	//Fills in color following file name
	for(;;){
		struct tb_cell* c;
		c = malloc(sizeof(struct tb_cell));
		c-> fg = TB_BLACK;
		c-> bg = TB_CYAN;
		uint32_t uni;
		c->ch = uni;
		tb_put_cell(p,0,c);
		p++;
		if(p == tb_width()){
			break;
		}
	}

	//Gives cells to each move
	giveCells(playerTurn[0]);
	//Makes red squares in board
	makeRedBoard();
	//Prints button display
	printButtons(playerTurn[0]);
	if(playerTurn[0] == 'r'){
		cx = tb_width()/2 -7;
		cy = 7;
	}else{
		cx = 0;
		cy = 7;
	}
	//Prints moves
	printMoves(playerTurn[0], 0);
	int pageCount = 0;
	movesPerPage = (pageEnd - 7)*2;
	int poo = movesPerPage;
	pageCount++;
	//Gets number of pages
	for(;;){
		if(poo < moveListIdx){
			poo = poo + movesPerPage; 
			pageCount++;
		}else{
			break;
		}
	}
	//Prints page numbers to bottom of screen
	if(pageEnd == 0){
		printPage(1,1);
	}else{
		printPage(1,pageCount);
	}
	char first[8][8];
	int r = 0;
	int c = 0;
	int placement = 0;
	int spot = 0;

	//Fills first with initial board settings
	for(;;){
		if(c == 8){
			c = 0;
			r++;
		}
		if(r == 8){
			break;
		}
		first[r][c] = '0';
		first[r][c] = brd[placement];
		placement++;
		c++;
	}

	//Populates board with initial board settings
	populateBoard(first);
	//Loop that displays termbox
	for(;;){
	    //Sets cursor
	tb_set_cursor(cx,cy);
	//Sets color of cursor
	colorCursor(cx,cy,spot);
	//Hides actual cursor
	tb_set_cursor(TB_HIDE_CURSOR,TB_HIDE_CURSOR);
	tb_present();

	//Used to represent button presses
	struct tb_event event;
	tb_poll_event(&event);

	//If esc is pressed quit
	if(TB_KEY_ESC == event.key)break;
	if(TB_KEY_ARROW_RIGHT == event.key){ //If right arrow is pressed
		if(playerTurn[0] == 'r'){//Reds turn first
			if(cx == (tb_width()/2)-7){//Red move
				if(spot% movesPerPage == 0 && curPage != 1){//End of the page
					resetCursor(cx,cy,spot);//Reset cursor
					cx = 0;
					cy = tb_height()-3;
					switchBoard(here[spot-1]->board,here[spot-2]->board); //Switch board
					curPage--;
					lastPage(spot,playerTurn[0]); //Inc page number
					spot--;
					printPage(curPage,pageCount);
				}else{
					if((cy-1) != 6){//If cy not at top or moves
						resetCursor(cx,cy,spot);//Reset cursor
						cy--;
						cx = 0;
						switchBoard(here[spot-1]->board,here[spot-2]->board); //Switch board
						spot--;
					}
				}
				continue;
			}else{//Black move
				resetCursor(cx,cy,spot); //Reset cursor
				cx = tb_width()/2-7;
				if(cy == 7 && curPage == 1){//Top of moves
					switchBoard(here[spot]->board,first);//Switch board
					spot--;
					continue;
				}else{
					switchBoard(here[spot-1]->board,here[spot-2]->board);//Switch board
					spot--;
					continue;
				}
			}
		}else{//Black turn first
			if((((spot+1)%movesPerPage)+(movesPerPage/2))+6 == tb_height()-3 && cx == tb_width()/2-7){ //End of page
				resetCursor(cx,cy,spot);//Reset cursor
				cx = 0;
				cy = 7;
				switchBoard(here[spot-1]->board,here[spot]->board);//Switch board
				curPage++;
				nextPage(spot, playerTurn[0]);//Prints next page of moves
				spot++;
				printPage(curPage,pageCount);//inc page number
				continue;	
			}else{
				if(spot == hereIdx){//End of move list
					continue;
				}else{
					if(cx == 0){//black move
						resetCursor(cx,cy,spot);//Reset cursor
						cx = tb_width()/2-7;
						if(cy == 7 && curPage == 1){//Top of 1st page reset board to first instance
							switchBoard(first,here[0]->board);
							spot++;
						}else{
							switchBoard(here[spot-1]->board,here[spot]->board);//Switch board
							spot++;
						}
						continue;
					}else{
						resetCursor(cx,cy,spot);//Reset cursor
						cy++;
						cx = 0;
						switchBoard(here[spot-1]->board,here[spot]->board);//Switch board
						spot++;
						continue;
					}
				}
			}
		}
	}
	if(TB_KEY_ARROW_LEFT == event.key){//If left arrow key is pressed
		if(playerTurn[0] == 'r'){//Red turn first
			if((((spot+1)%movesPerPage)+(movesPerPage/2))+6 == tb_height()-3 && cx == 0){//At bottom of the page
				resetCursor(cx,cy,spot);//reset cursor
				cx = tb_width()/2 -7;
				cy = 7;
				switchBoard(here[spot-1]->board,here[spot]->board);//Switch board
				curPage++;
				nextPage(spot,playerTurn[0]);//Print next page of moves
				printPage(curPage,pageCount);//Inc page numbers
				spot++;
				continue;
			}else{//Not bottom of the page
				if(spot == hereIdx){//End of the move list
					continue;
				}else{
					if(cx == 0){//black move
						resetCursor(cx,cy,spot);//Reset cursor
						cx = tb_width()/2-7;
						cy++;
						switchBoard(here[spot-1]->board,here[spot]->board);//Switch board
						spot++;
						continue;
					}else{//red move
						resetCursor(cx,cy,spot);//Reset cursor
						cx = 0;
						if(cy == 7 && curPage == 1){//Top of the 1st page
							switchBoard(first,here[spot]->board);//Switch board
							spot++;
						}else{
							switchBoard(here[spot-1]->board,here[spot]->board);//Switch board
							spot++;
						}
						continue;
					}
				}
			}
		}else{//Black turn first
			if(cx == 0){//Black turn
				if(spot% movesPerPage == 0 && curPage != 1){//End of the page
					resetCursor(cx,cy,spot);//Reset cursor
					cx = tb_width()/2-7;
					cy = tb_height()-3;
					switchBoard(here[spot-1]->board,here[spot-2]->board);//Switch board
					curPage--;
					lastPage(spot, playerTurn[0]);//Prints last page of moves
					spot--;
					printPage(curPage,pageCount);//Dec page numbers
				}else{//Not end of the page
					if((cy-1)!= 6){//Not top of the page
						resetCursor(cx,cy,spot);//Reset cursor
						cy--;
						cx = tb_width()/2-7;
						switchBoard(here[spot-1]->board,here[spot-2]->board);//Switch board
						spot--;
					}
				}
				continue;
			}else{//Red turn
				resetCursor(cx,cy,spot);//Reset cursor
				cx =0;
				if(cy == 7 && curPage == 1){//Top of 1st page
					switchBoard(here[0]->board,first);//Switch board
					spot--;
				}else{
					switchBoard(here[spot-1]->board,here[spot-2]->board);//Switch board
					spot--;
				}
				continue;
			}
		}
	}

	cell.ch = event.ch;//Changes cell.ch to equal event.ch
	tb_put_cell(cx++, cy, &cell);//places the cell
	}
	printf("%d,%d",tb_width(),tb_height());
	tb_shutdown();//Ends termbox
    	return 0;
}
