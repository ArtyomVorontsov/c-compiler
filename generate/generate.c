#include "./generate.h"

char * asm_buffer_ptr = asm_buffer;

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
	//printf("generate_expression %s\n", (**node).type);
	struct TreeNode * child_node;

	for(int i = (*node)->children_amount - 1; i >= 0; i--){
		child_node = (*node)->children[i];


		if(strcmp(child_node->type, "EXPRESSION") == 0) {
			generate_expression(&child_node);
		}

		printf("generate_expression %s\n", (child_node )->type);
		if(strcmp(child_node->type, "TERM") == 0) {
			printf("generate_expression %s\n", (*node)->children[i + 1]->type);
			/*
			if(strcmp((child_node + 1)->type, "ADDITION_OP") == 0) {
				printf("ADDITION %s\n", child_node->type);
				generate_fact(node);
				asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");
				generate_fact(node + 2);
				asm_buffer_ptr += sprintf(asm_buffer_ptr, "pop %%ecx\n");
				asm_buffer_ptr += sprintf(asm_buffer_ptr, "addl %%ecx %%eax\n");
				node = node + 3;
			}*/
//			generate_term(&child_node);
		}
		else {
			if(SILENT_ARG != true)
				printf("No handler\n");
		}
	}
}

void generate_unary_op(struct TreeNode * node){
	printf("generate_unary_op");
	if(strcmp(node->type, "NEGATION_OP") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# NEGATION OP\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "neg %%eax\n");	
	}

	if(strcmp(node->type, "BITWISE_COMPLEMENT_OP") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# BITWISE COMPLEMENT OP\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "not %%eax\n");	
	}

	if(strcmp(node->type, "LOGICAL_NEGATION_OP") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOGICAL NEGATION OP\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "sete %%al\n");	
	}
}

void generate_term(struct TreeNode ** node){
	printf("generate_term");
	if(strcmp((*node + 1)->type, "ADDITION_OP") == 0) {
		generate_fact(node);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");
		generate_fact(node + 2);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "pop %%ecx\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "addl %%ecx %%eax\n");
		node = node + 3;
	}
	/*if(strcmp(node->type, "ADDITION_OP") == 0) {
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $%s, %%eax\n", child_node->value);	
	}*/
}

void generate_fact(struct TreeNode ** node){
	printf("generate_fact");
	struct TreeNode * child_node = *node;

	if(strcmp(child_node->type, "INT") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $%s, %%eax\n", child_node->value);	
	}

	if(strcmp(child_node->type, "UNARY_OP") == 0) {
		generate_unary_op(child_node->children[0]);
	}
}


