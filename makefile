
all:
	@cd source; make
	cp source/parser parser

.PHONY:clean parse
clean:
	@cd source; make clean
	rm -f parser
	rm -f *.ir
	rm -f *~

parse: all
	./parser test.cmm