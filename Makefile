prog: fe_win.o
	cc -lncurses fe_win.o -o prog

fe_win.o:
	cc -c fe_win.c -o fe_win.o

.PHONY: clean tags
tags:
	uctags *.c *.h

clean:
	rm *.o *.out *prog *.core
