#include "./generate.h"

char * asm_buffer_ptr = asm_buffer;
int LABEL_AMOUNT = 0;

struct VarEntity {
	char * name;
	int offset;
};
struct VarEntity * var_map[100][100];
int registered_var_amount[100] = { 0 };
void generate(struct TreeNode * root_node){
	generate_program(root_node);
}
int scope_depth = 0;
int loop_depth = 0;
struct LoopLabels {
	char * loop_start;
	char * loop_end;
};
struct LoopLabels * loop_labels[100];

void generate_program(struct TreeNode * node){
	print_if_explicit("generate_program\n");

	for(int i = 0; i < node->children_amount; i++){
		if(strcmp(node->children[i]->type, "FUNCTION") == 0){
			generate_function(node->children[i]);
		}
		else {
			if(SILENT_ARG != true)
				printf("No handler\n");
		}
	}
}
void generate_function(struct TreeNode * node){
	print_if_explicit("generate_function\n");

	struct TreeNode * child_node = node->children[0];

	if(strcmp(child_node->type, "FUNCTION_DEFINITION") == 0){
		generate_function_definition(child_node);
	}
}

void generate_function_definition(struct TreeNode * node){
	print_if_explicit("generate_function_definition\n");
	char *identifier_value = node->children[1]->value;
	struct TreeNode * parameters_node = node->children[2];
	struct TreeNode * statement_node = node->children[3];

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, ".globl %s\n", identifier_value);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", identifier_value);
	enter_scope();
	generate_prologue(parameters_node);

	generate_function_body(statement_node);
	generate_epilogue();
	exit_scope();
}

void generate_prologue(struct TreeNode * node){
	struct TreeNode * var_identifier_node;

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# PROLOGUE\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%ebp\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl %%esp, %%ebp\n");

	// Link argument values
	for(int i = 0; i < node->children_amount; i++){
		var_identifier_node = node->children[i]->children[1];
		link_var(var_identifier_node->value, i);
	}
}

void generate_epilogue(){
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# EPILOGUE\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl %%ebp, %%esp\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "pop %%ebp\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "ret\n");
}

void generate_function_body(struct TreeNode * node){
	print_if_explicit("generate_function_body\n");
	struct TreeNode * child_node;

	if(node->children_amount > 0){
		for(int i = 0; i < node->children_amount; i++){
			child_node = node->children[i];
		
			if(strcmp(child_node->type, "BLOCK_ITEM") == 0) {
				generate_block_item(child_node);
			} 
			else {
				if(SILENT_ARG != true)
					printf("No handler\n");
			}
		}
	} 

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# NO RETURN STATEMENT\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
}

void generate_compound(struct TreeNode * node){
	print_if_explicit("generate_compound\n");
	struct TreeNode * child_node;
	enter_scope();

	if(node->children_amount > 0){
		for(int i = 0; i < node->children_amount; i++){
			child_node = node->children[i];
		
			if(strcmp(child_node->type, "BLOCK_ITEM") == 0) {
				generate_block_item(child_node);
			} 
			else {
				if(SILENT_ARG != true)
					printf("No handler\n");
			}
		}
	} else {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# NO RETURN STATEMENT\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
	}
	exit_scope();
}

void generate_block_item(struct TreeNode * node){
	print_if_explicit("generate_block_item\n");
	struct TreeNode * child_node = node->children[0];

	if(strcmp(child_node->type, "STATEMENT") == 0) {
		generate_statement(child_node);
	} 
	else if(strcmp(child_node->type, "DECLARATION") == 0) {
		generate_declaration(child_node);
	}
}

void generate_declaration(struct TreeNode * node){
	print_if_explicit("generate_declaration_statement\n");
	struct TreeNode * var_node;
	struct TreeNode * exp_node;
	struct TreeNode * second_child = node->children[1];
	struct TreeNode * identifier_node;

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# DECLARATION STATEMENT\n");	
	if(strcmp(second_child->type, "ASSIGN") == 0){
		// Declaration of variable with assignement
		var_node = second_child->children[0];
		exp_node = second_child->children[1];

		// Check if variable already exists
		struct VarEntity * var = get_var_by_name_in_current_scope(var_node->children[0]->value);

		if(var == -1){
			// generate expression which will be assigned to var
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "# INIT VAR\n");	
			generate_expression(exp_node);

			// push var on stack
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "# PUSH VAR ON TO THE STACK\n");	
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");

			// Register variable
			register_var(var_node->children[0]->value);
		} else {
			printf("ERROR: Variable %s is already declared.\n", var->name);
			exit(1);
		}

	} else {
		// Empty variable declaration
		var_node = second_child;
		identifier_node = var_node->children[0];

		// Check if variable already exists
		struct VarEntity * var = get_var_by_name_in_current_scope(identifier_node->value);

		if(var == -1){
			// Initialize variable to zero
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "# INIT VAR\n");	
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $0, %%eax\n");
			// Push variable on to the stack
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "# PUSH VAR ON TO THE STACK\n");	
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");

			// Register variable
			register_var(var_node->children[0]->value);
		} else {
			printf("ERROR: Variable %s is already declared.\n", var->name);
			exit(1);
		}
	}
}


void generate_statement(struct TreeNode * node){
	print_if_explicit("generate_statement\n");
	struct TreeNode * child_node;

	for(int i = node->children_amount - 1; i > -1; i--){
		child_node = node->children[i];
	
		if(strcmp(child_node->type, "EXPRESSION") == 0) {
			generate_expression(child_node);
		} 
		else if(strcmp(child_node->type, "RETURN_KEYWORD") == 0) {
			generate_epilogue();
			
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "# RETURN\n");	
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "ret\n");	
		} 
		else if(strcmp(child_node->type, "CONDITIONAL_STATEMENT") == 0) {
			generate_conditional_statement(child_node);
		} 
		else if(strcmp(child_node->type, "COMPOUND") == 0) {
			generate_compound(child_node);
		}
		else if(strcmp(child_node->type, "LOOP") == 0) {
			generate_loop(child_node);
		}
		else if(strcmp(child_node->type, "BREAK_KEYWORD") == 0) {
			generate_break(child_node);
		}
		else if(strcmp(child_node->type, "CONTINUE_KEYWORD") == 0) {
			generate_continue(child_node);
		}
		else {
			if(SILENT_ARG != true)
				printf("No handler\n");
		}
	}
}
void generate_break(struct TreeNode * node){
	print_if_explicit("generate_break\n");
	char * loop_end_label = get_current_loop_end_label();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# BREAK\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", loop_end_label);
}

void generate_continue(struct TreeNode * node){
	print_if_explicit("generate_continue\n");
	char * loop_start_label = get_current_loop_start_label();

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# CONTINUE\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", loop_start_label);
}

void generate_loop(struct TreeNode * node){
	print_if_explicit("generate_loop\n");
	struct TreeNode * loop_type_node = node->children[0];

	if(strcmp(loop_type_node->type, "FOR_KEYWORD") == 0) {
		generate_for_loop(node);
	}
	else if(strcmp(loop_type_node->type, "WHILE_KEYWORD") == 0) {
		generate_while_loop(node);
	}
	else if(strcmp(loop_type_node->type, "DO_KEYWORD") == 0) {
		generate_do_while_loop(node);
	}

}

void generate_do_while_loop(struct TreeNode * node){
	print_if_explicit("generate_do_while_loop\n");

	struct TreeNode * condition_node = node->children[3];
	struct TreeNode * statement_node = node->children[1];
	char * loop_start_label = generate_unique_label("_start");
	char * loop_end_label = generate_unique_label("_end");

	register_loop(loop_start_label, loop_end_label);
	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_start_label);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP BODY\n");
	generate_statement(statement_node);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP CONDITION\n");
	generate_expression(condition_node);
	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP CONDITION CHECK\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "je %s\n", loop_end_label);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", loop_start_label);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_end_label);

	unregister_loop();
}

void generate_while_loop(struct TreeNode * node){
	print_if_explicit("generate_while_loop\n");

	struct TreeNode * condition_node = node->children[1];
	struct TreeNode * statement_node = node->children[2];
	char * loop_start_label = generate_unique_label("_start");
	char * loop_end_label = generate_unique_label("_end");

	register_loop(loop_start_label, loop_end_label);
	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_start_label);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP CONDITION\n");
	generate_expression(condition_node);
	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP CONDITION CHECK\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "je %s\n", loop_end_label);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP BODY\n");
	generate_statement(statement_node);

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", loop_start_label);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_end_label);

	unregister_loop();
}

void generate_for_loop(struct TreeNode * node){
	print_if_explicit("generate_for_loop\n");

	struct TreeNode * init_node  = node->children[1];
	struct TreeNode * condition_node = node->children[2];
	struct TreeNode * post_expression_node = node->children[3];
	struct TreeNode * statement_node = node->children[4];
	char * loop_start_label = generate_unique_label("_start");
	char * loop_end_label = generate_unique_label("_end");
	char * loop_post_expression_label = generate_unique_label("_post_expression");

	register_loop(loop_post_expression_label, loop_end_label);

	if(strcmp(init_node->type, "EXPRESSION_OPTION") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP INIT EXPRESSION\n");
		generate_expression(init_node);
		
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_start_label);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP CONDITION\n");
		generate_expression(condition_node);
		
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP CONDITION CHECK\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "je %s\n", loop_end_label);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP BODY\n");
		generate_statement(statement_node);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# POST EXPRESSION\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_post_expression_label);
		generate_expression(post_expression_node);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", loop_start_label);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_end_label);

	}
	else if(strcmp(init_node->type, "DECLARATION") == 0) {
		enter_scope();
		
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP INIT EXPRESSION\n");
		generate_declaration(init_node);
		
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_start_label);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP CONDITION\n");
		generate_expression(condition_node);
		
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP CONDITION CHECK\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "je %s\n", loop_end_label);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# LOOP BODY\n");
		generate_statement(statement_node);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_post_expression_label);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# POST EXPRESSION\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_post_expression_label);
		generate_expression(post_expression_node);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", loop_start_label);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", loop_end_label);
			
		exit_scope();
	}


	unregister_loop();

}

void generate_conditional_statement(struct TreeNode * node){
	print_if_explicit("generate_conditional_statement\n");
	struct TreeNode * condition_node = node->children[0];
	struct TreeNode * true_condition_node = node->children[1];
	struct TreeNode * false_condition_node;

	char * true_label = generate_unique_label("_true");
	char * false_label = generate_unique_label("_false");
	char * end_label = generate_unique_label("_end");
	
	if(strcmp(condition_node->type, "CONDITION") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# CONDITION\n");
		generate_expression(condition_node->children[0]);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "je %s\n", false_label);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# TRUE CONDITION\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", true_label);
		generate_statement(true_condition_node->children[0]);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", end_label);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# FALSE CONDITION\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", false_label);


		if(node->children_amount == 3){
			false_condition_node = node->children[2];
			generate_statement(false_condition_node->children[0]);
		}

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", end_label);
	}
}

void generate_expression(struct TreeNode * node){
	print_if_explicit("generate_expression\n");
	struct TreeNode * child_node = node->children[0];

	if(strcmp(child_node->type, "EXPRESSION") == 0) {
		generate_expression(child_node);
	}
	else if(strcmp(child_node->type, "EXPRESSION_OPTION") == 0) {
		generate_expression(child_node);
	}
	else if((strcmp(child_node->type, "CONDITIONAL_EXPRESSION") == 0) && child_node->children_amount == 3) {
		generate_conditional_expression(child_node);
	}
	else if((strcmp(child_node->type, "CONDITIONAL_EXPRESSION") == 0) && child_node->children_amount == 1) {
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
	else if(strcmp(child_node->type, "ASSIGNEMENT_EXPRESSION") == 0) {
		generate_assignement_expression(child_node);
	}
	else if(strcmp(child_node->type, "NON_ZERO_CONSTANT") == 0){
		generate_non_zero_constant_expression(child_node);
	}
	else if(strcmp(child_node->type, "FUNCTION_ARGUMENT") == 0){
		generate_expression(child_node);
	}
	else {
		if(SILENT_ARG != true)
			printf("No handler\n");
	}
	print_if_explicit("generate_expression_exit\n");
}

void generate_non_zero_constant_expression(struct TreeNode * node){
	print_if_explicit("generate_non_zero_constant_expression\n");

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# NON ZERO CONSTANT\n");
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $1, %%eax\n");
}

void generate_conditional_expression(struct TreeNode * node){
	print_if_explicit("generate_conditional_expression\n");
	struct TreeNode * condition_node = node->children[0];
	struct TreeNode * true_condition_node = node->children[1];
	struct TreeNode * false_condition_node = node->children[2];

	char * true_label = generate_unique_label("_true");
	char * false_label = generate_unique_label("_false");
	char * end_label = generate_unique_label("_end");
	
	if(strcmp(condition_node->type, "CONDITION") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# CONDITION\n");
		generate_expression(condition_node->children[0]);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "cmpl $0, %%eax\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "je %s\n", false_label);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# TRUE CONDITION\n");
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", true_label);
		generate_expression(true_condition_node->children[0]);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "jmp %s\n", end_label);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# FALSE CONDITION\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", false_label);

		generate_expression(false_condition_node);

		asm_buffer_ptr += sprintf(asm_buffer_ptr, "%s:\n", end_label);
	}
}

void generate_assignement_expression(struct TreeNode * node){
	print_if_explicit("generate_assignement_expression\n");
	struct TreeNode * var_node;
	struct TreeNode * exp_node;
	struct TreeNode * identifier_node;
	struct TreeNode * child_node = node->children[0];
	struct VarEntity * var;

	// Declaration of variable with assignement
	var_node = child_node->children[0];
	identifier_node = var_node->children[0];
	exp_node = child_node->children[1];

	// Check if variable already exists
	var = get_var_by_name_with_error(identifier_node->value);

	// generate expression which will be assigned to var
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# ASSIGN VAR\n");	
	generate_expression(exp_node);
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl %%eax, %d(%%ebp)\n", var->offset);

	print_if_explicit("generate_assignement_expression_exit\n");
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
	else if(strcmp(child_node->type, "BINARY_OP") == 0){
		generate_binary_op(child_node);
	}
	print_if_explicit("generate_term_exit\n");
}

void generate_fact(struct TreeNode * node){
	print_if_explicit("generate_fact\n");
	struct TreeNode * child_node = node->children[0];
	struct VarEntity * var;

	if(strcmp(child_node->type, "INT") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# SET INT\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl $%s, %%eax\n", child_node->value);	
	}
	if(strcmp(child_node->type, "VAR") == 0) {
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "# REFERENCE VAR\n");
		var = get_var_by_name_with_error(child_node->children[0]->value);
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "movl %d(%%ebp), %%eax\n", var->offset);
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
	else if(strcmp(child_node->type, "FUNCTION_CALL") == 0) {
		generate_function_call(child_node);
	}
}

void generate_function_call(struct TreeNode * node){
	print_if_explicit("generate_function_call\n");
	struct TreeNode * arguments = node->children[1];
	bool fn_arguments_available = node->children_amount > 1;

	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# FUNCTION CALL\n");	
	if(fn_arguments_available)
		// Push arguments to the stack in reverse order
		for(int i = arguments->children_amount - 1; i >= 0; i--){
			generate_expression(arguments->children[i]);
			asm_buffer_ptr += sprintf(asm_buffer_ptr, "push %%eax\n");
		}
	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "call %s\n", node->children[0]->value);
	// After function call deallocate arguments if needed
	if(fn_arguments_available)
		asm_buffer_ptr += sprintf(asm_buffer_ptr, "addl $%d, %%esp\n", arguments->children_amount * 4);
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

// Var map operations

void register_var(char * name){
	print_if_explicit("register_var\n");
	struct VarEntity * var = get_var_by_name(name);

	if(var > -1){
		printf("ERROR: Variable %s is already declared\n", name);
		exit(1);
	}

	var_map[scope_depth][registered_var_amount[scope_depth]] = create_var_enity(name, -(registered_var_amount[scope_depth] + 1) * 4);
	registered_var_amount[scope_depth]++;
}

void link_var(char * name, int offset){
	print_if_explicit("link_var\n");

	var_map[scope_depth][registered_var_amount[scope_depth]] = create_var_enity(name, (offset + 2) * 4);
	registered_var_amount[scope_depth]++;
}

struct VarEntity * create_var_enity(char * name, int offset){
	print_if_explicit("create_var_enity\n");
	struct VarEntity * var_entity_p = malloc(sizeof (struct VarEntity));
	var_entity_p->name = name;
	var_entity_p->offset = offset;

	return var_entity_p;
}

void create_var_map(){
	for(int i = 0; i < registered_var_amount[scope_depth]; i++)
		var_map[scope_depth][i] = create_var_enity("", -1);
}

void enter_scope(){
	print_if_explicit("enter_scope\n");

	scope_depth++;
	registered_var_amount[scope_depth] = registered_var_amount[scope_depth - 1];
	create_var_map();
}

void exit_scope(){
	print_if_explicit("exit_scope\n");

	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "# DEALLOCATE VARIABLES\n");	
	asm_buffer_ptr += sprintf(asm_buffer_ptr, "addl $%d, %%esp\n", (registered_var_amount[scope_depth] - registered_var_amount[scope_depth - 1]) * 4);	

	// Dealocate variables
	registered_var_amount[scope_depth] = 0;
	scope_depth--;
}


struct VarEntity * get_var_by_name_with_error(char * name) {
	print_if_explicit("get_var_by_name_with_error\n");

	struct VarEntity * var = get_var_by_name(name);

	if(var == -1){
		printf("ERROR: Variable %s is not declared\n", name);
		exit(1);
	}


	return var;
}

struct VarEntity * get_var_by_name(char * name){
	print_if_explicit("get_var_by_name\n");

	for(int i = scope_depth; i > -1; i--){
		for(int j = 0; j < registered_var_amount[i]; j++){
			if(strcmp(var_map[i][j]->name, name) == 0){
				return var_map[i][j];
			}
		}
	}

	return -1;
}

struct VarEntity * get_var_by_name_in_current_scope(char * name){
	print_if_explicit("get_var_by_name_in_current_scope\n");

	for(int i = 0; i < registered_var_amount[scope_depth]; i++){
		if(strcmp(var_map[scope_depth][i]->name, name) == 0){
			return var_map[scope_depth][i];
		}
	}

	return -1;
}

void unregister_var_by_name(char * name){
	struct VarEntity * var = get_var_by_name_with_error(name);
	var->name = "\0";
	var->offset = -1;
	registered_var_amount[scope_depth]--;
}

void register_loop(char * loop_start_label, char * loop_end_label){
	struct LoopLabels * loop_labels_entity = malloc(sizeof(struct LoopLabels));
	
	loop_depth++;
		
	loop_labels_entity->loop_start = loop_start_label;
	loop_labels_entity->loop_end = loop_end_label;

	loop_labels[loop_depth] = loop_labels_entity;
}

void unregister_loop(){
	loop_depth--;
}

char * get_current_loop_start_label(){
	return loop_labels[loop_depth]->loop_start;
}

char * get_current_loop_end_label(){
	return loop_labels[loop_depth]->loop_end;
}

