
all: lexical.c syntax.y parser

lexical.c: lexical.l
	flex -o lexical.c lexical.l 

syntax.c: syntax.y
	bison -o syntax.c -d syntax.y 

parser: lexical.c syntax.c main.c
	gcc syntax.c main.c -ly -lfl -o parser

.PHONY:clean play

clean:
	rm -f lexical.c syntax.c syntax.h
	rm -f parser
	rm -f *~

play: parser
	./parser test.cmm