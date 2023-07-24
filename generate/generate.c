#include "./generate.h"

char * asm_buffer_ptr = asm_buffer;
int LABEL_AMOUNT = 0;

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
		else if(strcmp(child_node->type, "LOGICAL_OR_EXPRESSION") == 0) {
			generate_expression(child_node);
		}
		else if(strcmp(child_node->type, "LOGICAL_AND_EXPRESSION") == 0) {
			generate_expression(child_node);
		}
		else if(strcmp(child_node->type, "EQUALITY_EXPRESSION") == 0) {
			generate_expression(child_node);
		}
		else if(strcmp(child_node->type, "RELATIONAL_EXPRESSION") == 0) {
			generate_expression(child_node);
		}
		else if(strcmp(child_node->type, "ADDICTIVE_EXPRESSION") == 0) {
			generate_expression(child_node);
		}
		else if(strcmp(child_node->type, "BINARY_OP") == 0) {
			generate_binary_op(child_node);
		}
		else if(strcmp(child_node->type, "TERM") == 0) {
			generate_term(child_node);
		}
		else if(strcmp(child_node->type, "FACTOR") == 0) {
			generate_fact(child_node);
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
	else if (strcmp(operator_type, "DIVISION_OP") == 0){
		generate_division_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "OR_OP") == 0){
		generate_or_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "AND_OP") == 0){
		generate_and_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "EQUAL_OP") == 0){
		generate_equal_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "NOT_EQUAL_OP") == 0){
		generate_not_equal_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "LESS_THAN_OP") == 0){
		generate_less_than_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "LESS_THAN_OR_EQUAL_OP") == 0){
		generate_less_than_or_equal_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "GREATER_THAN_OP") == 0){
		generate_greater_than_statement(operand1, operand2);
	}
	else if (strcmp(operator_type, "GREATER_THAN_OR_EQUAL_OP") == 0){
		generate_greater_than_or_equal_statement(operand1, operand2);
	}
}
void generate_or_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_or_statement\n");
	char * clause2_label = generate_unique_label("_clause2");
	char * end_label = generate_unique_label("_end");

	generate_operand(operand1);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# OR STATEMENT\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "je %s\n", clause2_label);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $1, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", end_label);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", clause2_label);
	generate_operand(operand2);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "setne %%al\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", end_label);
}

void generate_and_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_and_statement\n");
	char * clause2_label = generate_unique_label("_clause2");
	char * end_label = generate_unique_label("_end");

	generate_operand(operand1);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# OR STATEMENT\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "jne %s\n", clause2_label);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", end_label);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", clause2_label);
	generate_operand(operand2);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "setne %%al\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", end_label);
}

void generate_equal_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_equal_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# EQUAL STATEMENT\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl %%eax, %%ecx\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "sete %%al\n");
}

void generate_not_equal_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_not_equal_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# NOT EQUAL STATEMENT\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl %%eax, %%ecx\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "setne %%al\n");
}

void generate_less_than_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_less_than_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LESS THAN STATEMENT\n");	

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl %%eax, %%ecx\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "setl %%al\n");
}

void generate_less_than_or_equal_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_less_than_or_equal_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LESS THAN OR EQUAL STATEMENT\n");	

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl %%eax, %%ecx\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "setle %%al\n");
}

void generate_greater_than_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_greater_than_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# GREATER THAN STATEMENT\n");	

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl %%eax, %%ecx\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "setg %%al\n");
}

void generate_greater_than_or_equal_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_greater_than_or_equal_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# GREATER THAN OR EQUAL STATEMENT\n");	

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl %%eax, %%ecx\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "setge %%al\n");
}

void generate_addition_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_addition_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# ADDITION STATEMENT\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "addl %%ecx, %%eax\n");
}

void generate_substraction_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_substraction_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "xchg %%ecx, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# SUBSTRACTION STATEMENT\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "neg %%ecx\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "addl %%ecx, %%eax\n");
}

void generate_multiplication_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_multiplication_statement\n");

	generate_operand(operand1);
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# MULTIPLICATION STATEMENT\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "imul %%ecx, %%eax\n");
}

void generate_division_statement(struct TreeNode * operand1, struct TreeNode * operand2){
	print_if_explicit("generate_division_statement\n");

	generate_operand(operand1);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cdq\n");
	stack_push();
	generate_operand(operand2);
	stack_pop();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "xchg %%ecx, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# DIVISION STATEMENT\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "idivl %%ecx, %%eax\n");
} 

void generate_operand(struct TreeNode * node){
	if(strcmp(node->type, "EXPRESSION") == 0) {
		generate_expression(node);
	}
	else if(strcmp(node->type, "LOGICAL_OR_EXPRESSION") == 0) {
		generate_expression(node);
	}
	else if(strcmp(node->type, "LOGICAL_AND_EXPRESSION") == 0) {
		generate_expression(node);
	}
	else if(strcmp(node->type, "EQUALITY_EXPRESSION") == 0) {
		generate_expression(node);
	}
	else if(strcmp(node->type, "RELATIONAL_EXPRESSION") == 0) {
		generate_expression(node);
	}
	else if(strcmp(node->type, "ADDICTIVE_EXPRESSION") == 0) {
		generate_expression(node);
	}
	else if(strcmp(node->type, "BINARY_OP") == 0) {
		generate_binary_op(node);
	}
	else if(strcmp(node->type, "TERM") == 0) {
		generate_term(node);
	}
	else if(strcmp(node->type, "FACTOR") == 0) {
		generate_fact(node);
	}
	else {
		if(SILENT_ARG != true)
			printf("No handler\n");
	}
}

void generate_unary_op(struct TreeNode * node){
	print_if_explicit("generate_unary_op\n");
	struct TreeNode * child_node_unary_op = node->children[0];
	struct TreeNode * child_node_op = child_node_unary_op->children[0];
	struct TreeNode * child_node_fact = node->children[1];

	generate_fact(child_node_fact);
	if(strcmp(child_node_op->type, "NEGATION_OP") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# NEGATION OP\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "neg %%eax\n");	
	}
	else if(strcmp(child_node_op->type, "BITWISE_COMPLEMENT_OP") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# BITWISE COMPLEMENT OP\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "not %%eax\n");	
	}
	else if(strcmp(child_node_op->type, "LOGICAL_NEGATION_OP") == 0) {
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
	else if(strcmp(child_node->type, "UNARY_OP") == 0) {
		generate_unary_op(child_node->children[0]);
	}
	else if(strcmp(child_node->type, "EXPRESSION") == 0) {
		generate_expression(child_node);
	}
	else if(strcmp(child_node->type, "UNARY_STATEMENT") == 0) {
		generate_unary_op(child_node);
	}
}

void stack_push(){
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# PUSH ON THE STACK\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");
}


void stack_pop(){
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# POP FROM THE STACK\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "pop %%ecx\n");
}

char * generate_unique_label(char * l){
	char * label  = malloc(100);
	sprintf(label, "%s_%d", l, LABEL_AMOUNT++);	
	return label;
}

