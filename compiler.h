#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>

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
bool cmpstr(char *s1, char *s2);

/* CONSTANTS */
extern long FA_FAILED;
extern struct Token tokens[100];
extern bool PRINT_PARSE_TREE_ARG;
extern bool PRINT_PARSE_TREE_EXPLICIT_ARG;

