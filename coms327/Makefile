all: info change rankmoves edit

info: main.o
	gcc main.o -o info

main.o: main.c
	gcc -c main.c

change: C.o
	gcc C.o -o change

C.o: C.c
	gcc -c C.c

rankmoves: rank.o
	gcc rank.o -o rankmoves

rank.o: rank.c
	gcc -c rank.c

edit: termboxx.o
	gcc termboxx.o -o edit -ltermbox -Iusr/termbox/include -I~/termbox -I~/Termbox

termboxx.o: termboxx.c
	gcc -c termboxx.c

clean:
	rm -f *.o info change rankmoves edit
