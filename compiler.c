#include "./compiler.h"

long FA_FAILED = -2;
struct Token tokens[100];
char source_code_buffer[1000] = { '\0' };

/* PARAMS */
bool PRINT_PARSE_TREE_ARG = false;
bool PRINT_PARSE_TREE_EXPLICIT_ARG = false;
bool SILENT_ARG = false;

int main(int argc, char **argv){
	char *p = source_code_buffer;
	struct TreeNode * root_node;
	read_file(argv[1], source_code_buffer);

	// Handle parameters
	if(argc > 2){
		if(cmpstr(argv[2], "--ppt")){
			PRINT_PARSE_TREE_ARG = true;
		} 
		if (cmpstr(argv[2], "--ppte")){
			PRINT_PARSE_TREE_EXPLICIT_ARG = true;
		} 
		if (cmpstr(argv[2], "-s")){
			SILENT_ARG = true;
		} 
	}

	lex(p);
	if(SILENT_ARG != true)
		printf("Lexing completed.\n\n");

	root_node = parse();
	if(SILENT_ARG != true)
		printf("\nParsing completed.\n\n");

	// Print parse tree
	if(PRINT_PARSE_TREE_ARG || PRINT_PARSE_TREE_EXPLICIT_ARG){
		print_parse_tree(root_node, PRINT_PARSE_TREE_EXPLICIT_ARG);
	}
	// TODO: Generate should return buffer with assembly
	generate(root_node);
	if(SILENT_ARG != true)
		printf("\nCode generation completed.\n\n");

	
	// TODO: Created file should have the same same as opened one just without extension (.c)
	FILE *fp;
	fp = fopen("./program", "w");

	// TODO: Write here assembly to file

	fclose(fp);


	return 0;
}

void print_parse_tree(struct TreeNode * root_node, bool explicit){
	printf("\n\nPrint tree:\n");

	printf(" %s (%s) %d\n", root_node->type, root_node->value, root_node->children_amount);
	print_children(root_node->children, root_node->children_amount, explicit);
}

void print_children(struct TreeNode * children[], int children_amount, bool explicit){
	for(int i = 0; i < children_amount; i++){
		if(explicit)
			printf(" %s (%s) %d\t", children[i]->type, children[i]->value, children[i]->children_amount);
		else
			printf(" %s\t", children[i]->value);
	}
	printf("\n");

	
	for(int i = 0; i < children_amount; i++){
		if(children[i]->children_amount)
			print_children(children[i]->children, children[i]->children_amount, explicit);
		else 
			printf("\t");
	}

	return;
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
