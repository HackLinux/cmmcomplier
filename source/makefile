CFILES = main.c syntax/syntax.c semantic/semantic.c \
		 common/*.c
		 #common/tree.c common/table.c common/type.c common/math.c common/tokenset.c  
CFLAGS = -ly -lfl -lm #-Wall


parser: syntax/lexical.c $(CFILES)
	gcc $(CFILES) $(CFLAGS) -o parser

syntax/lexical.c: syntax/lexical.l
	flex -o syntax/lexical.c syntax/lexical.l 

syntax/syntax.c: syntax/syntax.y
	bison -o syntax/syntax.c -d syntax/syntax.y 


.PHONY:clean
clean:
	rm -f syntax/lexical.c syntax/syntax.c syntax/syntax.h
	rm -f parser