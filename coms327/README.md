# PART1
This project is the implementation of the board game checkers. For the first part of the project
located in the main.c, I implemented the reading from an input file to get the game configurations. I checked to see if
the input file had valid feilds for the game configurations. Those feilds being the "rules"
<"capture" or "no capture"> and <"single jumps" or "multiple jumps">, tracking whose turn it is 
<"red" or "black">, the board format and whether it was valid or invaid and finally the "moves" 
and whether they were valid or invalid moves (between a-h and 1-8).
I also displayed messages to standard output according to the game configurations that were read
from the file and whether they were valid or not. Displaying the appropriate message
for invalid and valid files.

# PART2
The project also processes moves according to the standard input. Multiple jumps and capture rules have not been implemented yet.
Correct error messages are made if unimplemented rules are present.
Also, if specified in the with commands -e, -h, prints to an output file in either human readable format(-h) or exchange format(-e) by the name that follows each command.
If -m is present, then that many moves are to be processed.

# PART3
The project also processes the current board situation and according to the turn indicated in the file. We get the score for the board standings.
If -v is present in the arguments then we print in verbose format. If -d (n) then we process the board n times starting with the current move. Good for standard boards, single jumps
and no capture rule.

# PART4
The project now can take from parts one and two and use them to take an input file from the command line and output the moves and board status to a display screen, made from
termbox.You can go through the display one move at a time using the left and right arrow key. This display updates the board display as you move through the moves. Good for
standard boards, single jump and no capture rule. Saving and appending a move into the display is not a functionality implemented in this part. Display includes file name, 
moves, board display, page numbers, hotkey displays.
