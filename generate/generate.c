#include "./generate.h"

char * asm_buffer_ptr = asm_buffer;

void generate(struct TreeNode * root_node){
	generate_program(root_node->children[0]);
}

void generate_program(struct TreeNode * node){
	print_if_explicit("generate_program\n");
	
	if(strcmp(node->type, "FUNCTION") == 0){
			generate_function(node);
	}
	else {
		if(SILENT_ARG != true)
			printf("No handler\n");
	}
}

void generate_function(struct TreeNode * node){
	print_if_explicit("generate_function\n");
	char *identifier_value = node->children[1]->value;
	struct TreeNode * statement_node = node->children[5];
	asm_buffer_ptr += sprintf(asm_buffer_ptr, ".globl %s\n", identifier_value);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", identifier_value);

	generate_statement(statement_node);
}

void generate_statement(struct TreeNode * node){
	print_if_explicit("generate_statement\n");
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
	print_if_explicit("generate_expression\n");
	struct TreeNode * child_node;

	for(int i = 0; i < node->children_amount; i++){
		child_node = node->children[i];

		if(strcmp(child_node->type, "EXPRESSION") == 0) {
			generate_expression(child_node);
		}
		if(strcmp(child_node->type, "BINARY_OP") == 0) {
			generate_binary_op(child_node);
		}
		if(strcmp(child_node->type, "TERM") == 0) {
			generate_term(child_node);
		}
		else {
			if(SILENT_ARG != true)
				printf("No handler\n");
		}
	}
}

void generate_binary_op(struct TreeNode * node){
	print_if_explicit("generate_binary_op\n");
	struct TreeNode * child_node = node->children[0];
	
	generate_binary_statement(child_node->children[0], child_node->type );
}

void generate_binary_statement( struct TreeNode * node, char * operator_type ){
	struct TreeNode * operand1 = node->children[0];
	struct TreeNode * operand2 = node->children[1];

	print_if_explicit("generate_binary_statement\n");
	if(strcmp(operator_type, "ADDITION_OP") == 0){
		generate_addition_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "NEGATION_OP") == 0){
		generate_substraction_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "MULTIPLICATION_OP") == 0){
		generate_multiplication_statement(operand1, operand2);
	}
	
}

void generate_addition_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	if(strcmp(operand1->type, "TERM") == 0){
		generate_term(operand1);
	}
	else if(strcmp(operand1->type, "BINARY_OP") == 0){
		generate_binary_op(operand1);
	}
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# PUSH ON THE STACK\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");

	if(strcmp(operand2->type, "TERM") == 0){
		generate_term(operand2);
	}
	else if(strcmp(operand2->type, "BINARY_OP") == 0){
		generate_binary_op(operand2);
	} 

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# POP FROM THE STACK\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "pop %%ecx\n");

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# ADDITION STATEMENT\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "addl %%ecx, %%eax\n");
}

void generate_substraction_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	if(strcmp(operand1->type, "TERM") == 0){
		generate_term(operand1);
	}
	else if(strcmp(operand1->type, "BINARY_OP") == 0){
		generate_binary_op(operand1);
	}
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# PUSH ON THE STACK\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");

	if(strcmp(operand2->type, "TERM") == 0){
		generate_term(operand2);
	}
	else if(strcmp(operand2->type, "BINARY_OP") == 0){
		generate_binary_op(operand2);
	} 

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# POP FROM THE STACK\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "pop %%ecx\n");

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "xchg %%ecx, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# SUBSTRACTION STATEMENT\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "neg %%ecx\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "addl %%ecx, %%eax\n");
}

void generate_multiplication_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_multiplication_statement\n");
	if(strcmp(operand1->type, "TERM") == 0){
		generate_term(operand1);
	}
	else if(strcmp(operand1->type, "BINARY_OP") == 0){
		generate_binary_op(operand1);
	}
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# PUSH ON THE STACK\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");

	if(strcmp(operand2->type, "TERM") == 0){
		generate_term(operand2);
	}
	else if(strcmp(operand2->type, "BINARY_OP") == 0){
		generate_binary_op(operand2);
	} 

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# POP FROM THE STACK\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "pop %%ecx\n");

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# MULTIPLICATION STATEMENT\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "imul %%eax, %%ecx\n");
	//asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl %%bx, %%eax\n");
	
}

void generate_unary_op(struct TreeNode * node){
	printf("generate_unary_op\n");
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

void generate_term(struct TreeNode * node){
	print_if_explicit("generate_term\n");
	struct TreeNode * child_node = node->children[0];

	// TODO: generate multiplication code

	// TODO: generate division code
	if(strcmp(child_node->type, "FACTOR") == 0){
		generate_fact(child_node);
	}
	if(strcmp(child_node->type, "BINARY_OP") == 0){
		generate_binary_op(child_node);
	}
}

void generate_fact(struct TreeNode * node){
	print_if_explicit("generate_fact\n");
	struct TreeNode * child_node = node->children[0];

	if(strcmp(child_node->type, "INT") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# SET INT\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $%s, %%eax\n", child_node->value);	
	}

	if(strcmp(child_node->type, "UNARY_OP") == 0) {
		generate_unary_op(child_node->children[0]);
	}

	if(strcmp(child_node->type, "EXPRESSION") == 0) {
		// TODO
	}
}


