prog: interface.o window_list.o
	cc -lncurses interface.o window_list.o -o prog

interface.o:
	cc -c interface.c -o interface.o

window_list.o:
	cc -c window_list.c -o window_list.o

clean:
	rm *.o prog 
