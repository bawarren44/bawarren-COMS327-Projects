For the first part of my project I used arrays for my main source of data organization.

My main calls a series of functions to process the file input correctly. All functions are authored by Brad Warren.
Function discriptions below:

int rulesCheck(char *fileLine)
    - This function takes in a pointer to a char array as a parameter and runs through it to see if it contains "RULES:". Returns 1 if true, else 0.
    
int captureRules(char *fileLine)
    - This function takes in a pointer to a char array as a parameter and runs through it to see if the rules for capture are present. This is called if ruleCheck returns 1.
    Returns 0 if rule is not found, 1 if capture and 2 if no capture.
    
int jumpRules(char * fileLine)
    - This function takes in a pointer to a char array as a parameter and runs through it to see if the rules for jump are present. Returns 0 is not, 1 if single jumps and 2 if multiple jumps.

int turnCheck(char *fileLine)
    - This function takes in a pointer to a char array as a parameter and runs through it to see if it contains "TURN:". Returns 1 if ture, else 0.
    
int turn(char *fileLine)
    - This function takes in a pointer to a char array as a parameter and runs through it to see if is important for evaluating the turn. Returns 0 if false, 1 if reds turn and 2 if blacks turn.
    
int checkBoard(char *fileLine)    
    - This function takes in a pointer to a char array as a parameter and runs through it to see if it contains "BOARD:". Returns 1 if true, else 0.
    
int boardValidation(char *board)
    - This function takes in a pointer to a char array as a parameter and runs through it to see if it contains all elements needed for the board to be valid. Returns 1 if valid, else 0.
    
int checkMoves(char *fileLine)
    - This function takes in a pointer to a char array as a parameter and runs through it to see if it contains "MOVES:". Returns 1 if true, else 0.
    
int moves(char *fileLine)
    - This function takes in a pointer to a char array as a parameter and runs through it and finds "->". After finding this, it determines if a move has numbers and letters between 1-8 and A-H.
    
int validation(char c, int line)
    - This function takes in a char for determining whether a invalid statement gets printed to output and an integer to determine what line the error occurs.
    
For part 2 of my project I continued to use arrays for my main source of data organization. However, this time I used pointers. I moved a lot of the files from the main.c
into the header file change.h for easier implementation.

int boardCol(char letter)
    - This funtion returns the correct column number from the given letter input.
    
int boardRow(char number)
    - This function returns the correct row number from the given number input.
    
void boardFormat()
    - This function makes the board easier the read by copying the board into an 8x8 char array.
    
int staysOnBlack(char *m)
    - This function checks if the move stays on the black squares of the board. Returns 1 if true, else 0. Move indicated in parameter as m.
    
int regMove(char*m)
    -This function checks to see if the piece increments through the rank correctly. Returns 0 is false, else 1. Move indicated in parameter as m.
    
int playerJump(char*m, int turn)
    -This function performs a jump move. Returns 1 if move is legal, else 0. Move indicated in parameter as m.
    
void moveToIllegal(char*m)
    -This function moves a move to the illegal move list. Move indicated in parameter as m.
    
int moveListCheck(char* m)
    -This function performs the move. Runs that move through several methods to determine if the move is legal. If any method returns 0 
    moveListCheck returns 0 after first sending move to illegal move list using moveToIllegal method. Move indicated in parameter as m.
    
void setJumpRule(char* jumpRule, char* captureRule)
    -This function copies the rules into our designated rule string values
    
void outputFileH(char* filename)
    -This function writes to a file in human readable form. File name specified in the parameter as a char pointer.
    
void outputFileE(char* filename, char** commands)
    -This function writes to a file in exchange form. File name in the parameter as a pointer. Pointer to list of commands made for command print out purposes.
    
For part 3 of my project I continued to use arrays nested in a struct for my main source of data organization. I also added two new classes named verbose.h and rank.c to process all part 3.
    
struct moves - with a double pointer char array to contain moves
    - List that contains moves
    
void printVerboseForm(int depth, char* m, int turn)
    - This method prints a move of a piece in verbose form
    
void printMoveScore(int depth, char* m, int turn, int goal)
    - This method prints the score of a certain pieces move
    
void revertBoard(char b4[8][8])
    - This method reverts the board back to its original state before it processed a move
    
char getMoveLetter(int i)
    - This method gets the letter (column) associated with a number input
    
char getMoveNumber(int i)
    - This method gets the number (row) associated with a number input
    
int getScore(int turn)
    - This method gets the score of the current board standings, return based on whose turn it is
    
int simulateMoves(int i, int j, struct moves *list)
    - This method simulates all possible moves that can be made by a piece and populates the list with the moves
    
int recursiveGodMethod(int goal, int depth, int playerTurn, char board[8][8])
    - This mehod virtually does it all. It is a recursive method that processes the board from the end to the front and gets the score according to the player turn.
    Also calls to print methods to print according to stdin. Calls itself if the depth inputted into stdin is not reached yet, or game is over.
    
For part 4 of my projext I continued to use arrays nested inside of structs as my main source of data organization. I also have two new classes termboxx.c and term.h and I use
Termbox for this assignment.

struct move
    -Contains a struct moves (char array for a move), the current board instance, int good (if move is valid, invalid or unknown), int x (x coordinate of board), int y (y coordinate of board)

void constructMoves(int x, int first)
    - This method is used to construct the list of moves into structs. Copies board instance and sets int good to be valid, invalid or unknown.
    
void makeRedBoard()
    - Sets up the Red tiles of the board display
    
void print_tb(const char *str, int x, int y, uint16_t fg, uint16_t bg)
    - Helper method for printf_tb()
    
void printf_tb(int x, int y, uint16_t fg, uint16_t bg, const char *fmt,...)
    - Used to print into a cell with any string input    
    
void printMoves(char t, int i)
    - Prints the moves onto the screen in order that they come in, stops if reached the height of display - 2 or end of moves.
    
void giveCells(char c)
    - Gives x and y location (cell) for each value in the list of structs.
    
struct tb_cell* getCell(char c)
    - Gives the cell input to the specific board value.
    
void incCounter(int y, int x)
    - Increments the counter of boardCountx and boardCounty (our current cursor location).
    
void populateBoard(char b[8][8])
    - Sets the board white spaces of board display from the given board input.
    

void switchBoard(char lastBoard[8][8], char board[8][8])
    - Switches the board display from one board to another.
    
void printButtons(char t)
    - Prints the hotkey button display onto the display.
    
void printPage(int x, int h)
    - Prints the page numbers at the bottom of the display.
    
void nextPage(int spot, char t)
    - Prints the next page display.
    
void lastPage(int spot, char t)
    - Prints the last page display.
    
void colorCursor(int x, int y,int spot)
    - Colors the cursor according to if the current move in the spot is valid == 0, invalid ==1, unknown == 2.
    
void resetCursor(int x, int y, int spot)
    - Resets the cursor color.
    
int main(int argc, char** argv)
    - Main method that is used inside of termboxx.c to create the edit executable. Processes moves as specified from the command line arguments.
    