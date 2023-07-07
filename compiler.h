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

struct Position {
	int line;
	int line_char;
};

struct Token {
	char *type;
	union Value value;
	struct Position position;
};

struct Token * lex(char *inp);

/* PARSER */
struct TreeNode {
	char * type;
	char * value;
	struct TreeNode * children[10];
	int children_amount;
};

struct TreeNode * parse();

/* GENERATE */
void generate(struct TreeNode * root_node);

/* UTILS */
char * read_file(char * file_name, char * buff);
bool cmpstr(char *s1, char *s2);
void print_children(struct TreeNode * children[], int children_amount, int depth, bool explicit);
void print_parse_tree(struct TreeNode * root_node, bool explicit);

/* CONSTANTS */
extern long FA_FAILED;
extern struct Token tokens[100];
extern char asm_buffer[10000];

/* PARAMS */
extern bool PRINT_PARSE_TREE_ARG;
extern bool PRINT_PARSE_TREE_EXPLICIT_ARG;
extern bool SILENT_ARG;

