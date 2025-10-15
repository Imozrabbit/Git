#links .o file to .exe file, for large projects, better to link all .o files once
#-0 <name of the .exe> = output file name
lzw : lzw.o
	gcc -o lzw lzw.o

#-c = compile only, turns .c to .o
lzw.o : lzw.c
	gcc -c lzw.c

clean :
	rm -f lzw.o