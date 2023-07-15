#include "./generate.h"

char * asm_buffer_ptr = asm_buffer;
struct TreeNode * curr_node_ptr;

void generate(struct TreeNode * root_node){
	generate_program(root_node->children[0]);
}

void generate_program(struct TreeNode * node){
	printf("generate_program\n");
	
	if(strcmp(node->type, "FUNCTION") == 0){
			generate_function(node);
	}
	else {
		if(SILENT_ARG != true)
			printf("No handler\n");
	}
}

void generate_function(struct TreeNode * node){
	printf("generate_function\n");
	char *identifier_value = node->children[1]->value;
	struct TreeNode * statement_node = node->children[5];
	asm_buffer_ptr += sprintf(asm_buffer_ptr, ".globl %s\n", identifier_value);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", identifier_value);

	generate_statement(statement_node);
}

void generate_statement(struct TreeNode * node){
	printf("generate_statement\n");
	struct TreeNode * child_node;

	for(int i = node->children_amount - 1; i >= 0; i--){
		child_node = node->children[i];
	
		if(strcmp(child_node->type, "EXPRESSION") == 0) {
			generate_expression(&child_node);
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

void generate_expression(struct TreeNode ** node){
	printf("generate_expression\n");
	struct TreeNode * child_node;

	child_node = (*node)->children[0];

	if(strcmp(child_node->type, "EXPRESSION") == 0) {
		generate_expression(&child_node);
	}

	if(strcmp(child_node->type, "TERM") == 0) {
		generate_term(child_node);
	}
	else {
		if(SILENT_ARG != true)
			printf("No handler\n");
	}
}


int generate_term(struct TreeNode * node){
	printf("generate_term\n");
	struct TreeNode * child_node = node->children[0];

	if(strcmp(child_node->type, "FACTOR") == 0) {
		generate_fact(child_node);
	}
}

int generate_term_r(struct TreeNode * node){
	printf("generate_term_r\n");
	struct TreeNode * child_node = node->children[0];

	if(strcmp(child_node->type, "FACTOR_R") == 0) {
		generate_fact(child_node);
	}
}

void generate_fact(struct TreeNode * node){
	printf("generate_fact\n");
	printf("%s\n", node->children[0]->type);

	struct TreeNode * child_node = node->children[0];

	if(strcmp(child_node->type, "UNARY_OP") == 0) {
		generate_unary_op(child_node);
	}

	if(strcmp(child_node->type, "FACTOR_R") == 0) {
		generate_fact_r(child_node);
	}

	if(strcmp(child_node->type, "INT") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $%s, %%eax\n", child_node->value);
	}

}


void generate_fact_r(struct TreeNode * node){
	printf("generate_fact_r\n");

	struct TreeNode * child_node = node->children[0];
	printf("%s\n", child_node->type);

	if(strcmp(child_node->type, "UNARY_OP") == 0) {
		generate_unary_op(child_node);
	}

	if(strcmp(child_node->type, "INT") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $%s, %%eax\n", child_node->value);
	}
}

void generate_unary_op(struct TreeNode * node){
	printf("generate_unary_op\n");
	struct TreeNode * child_node = node->children[0];
	printf("%s\n", child_node->type);

	if(strcmp(child_node->type, "NEGATION_OP") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# NEGATION OP\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "neg %%eax\n");	
	}

	if(strcmp(child_node->type, "BITWISE_COMPLEMENT_OP") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# BITWISE COMPLEMENT OP\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "not %%eax\n");	
	}

	if(strcmp(child_node->type, "LOGICAL_NEGATION_OP") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOGICAL NEGATION OP\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "sete %%al\n");	
	}
}
