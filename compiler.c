#include "./compiler.h"

int main(int argc, char **argv){
	char inp[1000] = { '\0' };
	char *p = inp;

	FILE *ptr;
	char ch;

	ptr = fopen(argv[1], "r");
	if(NULL == ptr) printf("File can't be opened\n");

	do {
		ch = fgetc(ptr);
		*p++ = ch;
	} while (ch != EOF);

	lex(inp);

	return 0;
}
