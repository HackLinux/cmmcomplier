
parser:
	@cd source; make

.PHONY:clean run

clean:
	@cd source; make clean
	rm -f parser
	rm -f *~

run: parser
	./parser test.cmm