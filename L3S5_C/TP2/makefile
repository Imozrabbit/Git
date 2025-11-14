octave: main.o synthe.o save.o
	gcc main.o synthe.o save.o -o octave -lm
main.o: main.c synthe.h save.h msg.h
	gcc -ansi -Wall -c main.c
synthe.o: synthe.c synthe.h msg.h
	gcc -ansi -Wall -c synthe.c
save.o: save.c save.h msg.h synthe.h
	gcc -ansi -Wall -c save.c
clean:
	rm -f *.o
	rm -f octave
	rm -f test*
	rm -f *.csv
