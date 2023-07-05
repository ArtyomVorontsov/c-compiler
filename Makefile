 files := compiler.c compiler.h ./lexer/lexer.c ./lexer/lexer.h ./parser/parser.c ./parser/parser.h ./generate/generate.c ./generate/generate.h

compiler.out: $(files)
	gcc -o compiler.out $(files)


clean:
	rm compiler.out
