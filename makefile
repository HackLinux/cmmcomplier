
all:
	@cd source; make
	cp source/parser parser

.PHONY:clean
clean:
	@cd source; make clean
	rm -f parser
	rm -f *~

.PHONY:parse
parse: all
	./parser test.cmm