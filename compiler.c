#include "./compiler.h"

long FA_FAILED = -2;
struct Token tokens[100];
char source_code_buffer[1000] = { '\0' };

int main(int argc, char **argv){
	char *p = source_code_buffer;

	read_file(argv[1], source_code_buffer);
	lex(p);
	printf("Lexing completed.\n\n");
	parse();
	printf("Parsing completed.\n\n");

	return 0;
}

char * read_file(char * file_name, char * buff){
	char *p = buff;
	FILE *ptr;
	char ch;

	ptr = fopen(file_name, "r");
	if(NULL == ptr) printf("File can't be opened\n");

	do {
		ch = fgetc(ptr);
		*p++ = ch;
	} while (ch != EOF);
}

