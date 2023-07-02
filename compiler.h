#include <stdio.h>
#include <string.h>
#include <ctype.h>

/* LEXER */
union Value {
	int number;
	char string[100];
};

struct Token {
	char *type;
	union Value value;
};

struct Token * lex(char *inp);

/* PARSER */
int parse();

/* UTILS */
char * read_file(char * file_name, char * buff);

/* CONSTANTS */
extern int FA_FAILED;
extern struct Token tokens[100];

