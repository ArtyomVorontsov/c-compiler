#include "./compiler.h"

long FA_FAILED = -2;
struct Token tokens[100];
char source_code_buffer[1000] = { '\0' };
bool PRINT_PARSE_TREE_ARG = false;
bool PRINT_PARSE_TREE_EXPLICIT_ARG = false;

int main(int argc, char **argv){
	char *p = source_code_buffer;

	read_file(argv[1], source_code_buffer);

	// Handle parameters
	if(argc > 2){
		if(cmpstr(argv[2], "--ppt")){
			PRINT_PARSE_TREE_ARG = true;
		} 
		if (cmpstr(argv[2], "--ppte")){
			PRINT_PARSE_TREE_EXPLICIT_ARG = true;
		} 
	}

	printf("explicit: %d\n", PRINT_PARSE_TREE_EXPLICIT_ARG);

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

bool cmpstr(char *s1, char *s2){
	bool is_equal = true;
	int str_len = 0;
	while(*(s1 + str_len++) != '\0');

	while(str_len--){
		if(*s1++ != *s2++) {
			is_equal = false;
		}
	}

	return is_equal;
}
