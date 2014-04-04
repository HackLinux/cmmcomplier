CFILES = syntax.c main.c tree.c
CFLAGS = -ly -lfl #-Wall


parser: lexical.c $(CFILES)
	gcc $(CFILES) $(CFLAGS) -o parser

lexical.c: lexical.l
	flex -o lexical.c lexical.l 

syntax.c: syntax.y
	bison -o syntax.c -d -v syntax.y 


.PHONY:clean
clean:
	rm -f lexical.c syntax.c syntax.h
	rm -f parser
	rm -f *~
