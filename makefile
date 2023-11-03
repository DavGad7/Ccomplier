compiler: macro_spreading.o first_transition.o second_transition.o assembler.o general.o
	gcc -g -ansi macro_spreading.o first_transition.o second_transition.o assembler.o general.o -o compiler -lm
macro_spreading.o: macro_spreading.c macro_spreading.h general.h
	gcc -c -Wall -ansi -pedantic macro_spreading.c -o macro_spreading.o -lm
first_transition.o: first_transition.c first_transition.h general.h
	gcc -c -Wall -ansi -pedantic first_transition.c -o first_transition.o -lm
second_transition.o: second_transition.c second_transition.h general.h
	gcc -c -Wall -ansi -pedantic second_transition.c -o second_transition.o -lm
assembler.o: assembler.c assembler.h first_transition.h second_transition.h macro_spreading.h
	gcc -c -Wall -ansi -pedantic assembler.c -o assembler.o -lm
general.o: general.c general.h
	gcc -c -Wall -ansi -pedantic general.c -o general.o -lm
