 files := compiler.c compiler.h ./lexer/lexer.c ./lexer/lexer.h ./parser/parser.c ./parser/parser.h

compiler.out: $(files)
	gcc -o compiler.out $(files)


clean:
	rm compiler.out
