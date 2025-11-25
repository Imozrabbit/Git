lzw: lzw.o
	gcc -o lzw lzw.o
lzw.o: lzw.c lzw.h
	gcc -ansi -Wall -c lzw.c
clean:
	rm -rf *.o
	rm -rf test*
	rm -rf *.lzw
