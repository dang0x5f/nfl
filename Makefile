prog: fe_win2.o file_nodes.o
	cc -lncurses file_nodes.o fe_win2.o -o prog

fe_win2.o:
	cc -c fe_win2.c -o fe_win2.o

file_nodes.o:
	cc -c file_nodes.c -o file_nodes.o

.PHONY: clean tags
tags:
	uctags *.c *.h

clean:
	rm *.o *.out *prog *.core
