#include "./generate.h"

char * asm_buffer_ptr = asm_buffer;

void generate(struct TreeNode * root_node){
	generate_program(root_node->children[0]);
}

void generate_program(struct TreeNode * node){
	if(strcmp(node->type, "FUNCTION") == 0){
			generate_function(node);
	}
	else {
		if(SILENT_ARG != true)
			printf("No handler\n");
	}
}

void generate_function(struct TreeNode * node){
	char *identifier_value = node->children[1]->value;
	struct TreeNode * statement_node = node->children[5];
	asm_buffer_ptr += sprintf(asm_buffer_ptr, ".globl %s\n", identifier_value);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", identifier_value);

	generate_statement(statement_node);
}

void generate_statement(struct TreeNode * node){
	struct TreeNode * child_node;

	for(int i = node->children_amount - 1; i >= 0; i--){
		child_node = node->children[i];
	
		if(strcmp(child_node->type, "EXPRESSION") == 0) {
			generate_expression(child_node);
		} 
		else if(strcmp(child_node->type, "RETURN_KEYWORD") == 0) {
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "ret\n");	
		} 
		else {
			if(SILENT_ARG != true)
				printf("No handler\n");
		}
	}
}

void generate_expression(struct TreeNode * node){
	struct TreeNode * child_node;

	for(int i = node->children_amount - 1; i >= 0; i--){
		child_node = node->children[i];
	
		if(strcmp(child_node->type, "INT") == 0) {
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $%s, %%eax\n", child_node->value);	
		}
		else {
			if(SILENT_ARG != true)
				printf("No handler\n");
		}
	}
}

