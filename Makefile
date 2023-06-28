 files := compiler.c compiler.h ./lexer/lexer.c ./lexer/lexer.h

compiler.out: $(files)
	gcc -o compiler.out $(files)


clean:
	rm compiler.out
