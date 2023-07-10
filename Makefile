 files := compiler.c compiler.h ./lexer/lexer.c ./lexer/lexer.h ./parser/parser.c ./parser/parser.h ./generate/generate.c ./generate/generate.h

compiler := gcc

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
compiler := gcc
endif
ifeq ($(UNAME), Darwin)
compiler := gcc-11
endif


compiler.out: $(files)
	$(compiler) $(files) -o compiler.out

clean:
	rm compiler.out
