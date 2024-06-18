prog: fe_win2.o
	cc -lncurses fe_win2.o -o prog

fe_win2.o:
	cc -c fe_win2.c -o fe_win2.o

.PHONY: clean tags
tags:
	uctags *.c *.h

clean:
	rm *.o *.out *prog *.core
