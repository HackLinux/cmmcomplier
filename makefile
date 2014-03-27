
all: syntax.c lexical.c

syntax.c: syntax.y
	bison -o syntax.c -d syntax.y  

lexical.c: lexical.l
	flex -o lexical.c lexical.l 

.PHONY:clean

clean:
	rm -f lexical.c syntax.c syntax.h
	rm -f *~
