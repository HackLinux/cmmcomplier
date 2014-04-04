
parser:
	@cd source; make

.PHONY: clean
clean:
	@cd source; make clean
	rm -f parser
	rm -f *~

.PHONY: run
run: parser
	./parser test.cmm