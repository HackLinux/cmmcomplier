CFILES = syntax.c main.c tree.c semantic.c table.c type.c common/math.c common/tokenset.c 
CFLAGS = -ly -lfl -lm #-Wall


parser: lexical.c $(CFILES)
	gcc $(CFILES) $(CFLAGS) -o parser

lexical.c: lexical.l
	flex -o lexical.c lexical.l 

syntax.c: syntax.y
	bison -o syntax.c -d syntax.y 


.PHONY:clean
clean:
	rm -f lexical.c syntax.c syntax.h syntax.output
	rm -f parser
	rm -f *~
