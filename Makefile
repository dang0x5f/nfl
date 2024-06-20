prog: fe_win.o file_nodes.o
	cc -lncurses -g file_nodes.o fe_win.o -o prog

fe_win.o:
	cc -c fe_win.c -o fe_win.o

file_nodes.o:
	cc -c file_nodes.c -o file_nodes.o

.PHONY: clean tags
tags:
	uctags *.c *.h

clean:
	rm *.o *.out *prog *.core
