

scanner: lexical.c
	gcc lexical.c -lfl -o scanner 

lex.yy.c: lexical.l
	flex lexical.l -o lexical.c

.PHONY:clean scan

clean:
	rm -f lexical.c scanner 
	rm -f *~

scan: scanner
	./scanner test.cmm