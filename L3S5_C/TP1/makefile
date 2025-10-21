#links .o file to .exe file, for large projects, better to link all .o files once
#-0 <name of the .exe> = output file name, -o means output
lzw : lzw.o
	gcc -o lzw lzw.o

#-c = compile only, turns .c to .o, need to add all depenencies in there
lzw.o : lzw.c lzw.h
	gcc -ansi -Wall -c lzw.c

#delete all .o files and the executable file
clean :
	rm -f *.o
	rm -f lzw