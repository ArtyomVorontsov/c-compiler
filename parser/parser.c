#include "./parser.h"

struct TreeNode * nodes_stack[100];
struct TreeNode ** pns = nodes_stack;
struct Token * pt = tokens;
int error_tree_depth = -1;
struct Token * error_token;
bool expression_error = false;


struct TreeNode * parse(){
	bool match;
	*pns = nodes_stack[0];

	match = Program();
	if(!match) {
		printf(
			"ERROR: Parsing is failed at line %d:%d. After '%s'\n", 
			error_token->position.line, 
			error_token->position.line_char, 
			error_token->value.string
		), 
		exit(1);
	}

	struct TreeNode * root_node = nodes_stack[0];

	return root_node;
}


bool Program(){
	print_if_explicit("Program\n");
	struct TreeNode *node = create_node("PROGRAM", "PROGRAM");
	bool match;
	struct TreeNode * root_node = nodes_stack[0];

	set_node_as_deepest(node);

	match = Function();

	remove_node_from_deepest();	

	if(match) return true;
	set_error();

	return false;
}

bool Function(){
	print_if_explicit("Function\n");
	struct TreeNode *node = create_node("FUNCTION", "FUCTION");
	bool match;

 	set_as_child(node);
	set_node_as_deepest(node);

	match = Type() && 
		Identifier() && 
		Match("OPEN_PARENTHESIS") && 
		Match("CLOSE_PARENTHESIS") && 
		Match("OPEN_BRACE") && 
		Multi_Statement() && 
		Match("CLOSE_BRACE");

	remove_node_from_deepest();

	if(match) return true;
	set_error();

	return false;
}

bool Multi_Statement(){
	print_if_explicit("Multi_Statement\n");
	bool match = false;
	int i = 0;
	struct Token * next = pt;
	struct TreeNode * node = create_node("MULTI_STATEMENT", "MULTI_STATEMENT");

 	set_as_child(node);
	set_node_as_deepest(node);

	while(Statement()){
		next = pt;
		match = true;
		if(i++ > 1000){
			printf("Error: expressions amount exceeded 999!\n");
			set_error();
			return false;
		}
	}

	// Recover after unsuccessfull statement match
	pt = next;
	node->children_amount--;

	remove_node_from_deepest();	

	return true;
}

bool Statement(){
	print_if_explicit("Statement\n");
	bool match = false;
	struct TreeNode * node = create_node("STATEMENT", "STATEMENT");
	struct TreeNode * exp;
	struct Token * next = pt;

 	set_as_child(node);
	set_node_as_deepest(node);

	if(
		(pt = next, 
		remove_node_as_child(node), 
		expression_error = false, 1) &&
		Match("RETURN_KEYWORD") && 
		(exp = Expression(), set_node_as_child(node, exp), expression_error == false) && 
		Match("SEMICOLON")
	){
		match = true;
	} 
	else if (
		(pt = next, 
		remove_node_as_child(node), 
		expression_error = false, 1) &&
		(exp = Expression(), set_node_as_child(node, exp), expression_error == false) && 
		Match("SEMICOLON")
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_node_as_child(node), 
		expression_error = false, 1) &&
		(exp = Declaration_Statement(), set_node_as_child(node, exp), expression_error == false) && 
		Match("SEMICOLON")
	){
		match = true;
	} 
	remove_node_from_deepest();	

	if(match) return true;

	set_error();
	return false;
}


struct TreeNode * Expression(){
	print_if_explicit("Expression\n");
	struct TreeNode * node = create_node("EXPRESSION", "EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Logical_And_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(0)){
		bin_op_node = Binary_OP();
		next_term_node = Logical_And_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Logical_Or_Expression(){
	print_if_explicit("Logical_Or_Expression\n");
	struct TreeNode * node = create_node("LOGICAL_OR_EXPRESSION", "LOGICAL_OR_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Logical_And_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(0)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Logical_And_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Logical_And_Expression(){
	print_if_explicit("Logical_And_Expression\n");
	struct TreeNode * node = create_node("LOGICAL_AND_EXPRESSION", "LOGICAL_AND_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Equality_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(1)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Equality_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Equality_Expression(){
	print_if_explicit("Equality_Expression\n");
	struct TreeNode * node = create_node("EQUALITY_EXPRESSION", "EQUALITY_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Relational_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(2)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Relational_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Relational_Expression(){
	print_if_explicit("Relational_Expression\n");
	struct TreeNode * node = create_node("RELATIONAL_EXPRESSION", "RELATIONAL_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Addictive_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(3)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Addictive_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Addictive_Expression(){
	print_if_explicit("Addictive_Expression\n");
	struct TreeNode * node = create_node("ADDICTIVE_EXPRESSION", "ADDICTIVE_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Term();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(4)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Term();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Term(){
	print_if_explicit("Term\n");
	struct TreeNode * node = create_node("TERM", "TERM");
	struct TreeNode * factor_node;
	struct TreeNode * next_factor_node;
	struct TreeNode * op_node;

	set_node_as_deepest(node);

	factor_node = Factor();
	set_node_as_child(node, factor_node);


	while(pt->type && compare_operator_by_precedence_level(5)){
		op_node = Binary_OP(pt);
		next_factor_node = Factor(pt + 1);

		factor_node = Binary_Statement(op_node, factor_node, next_factor_node);
		remove_node_as_child(node);
		set_node_as_child(node, factor_node); 
	}

	remove_node_from_deepest();	

	return node;
}


struct TreeNode * Factor(){
	print_if_explicit("Factor\n");
	struct TreeNode * node = create_node("FACTOR", "FACTOR");
	struct TreeNode * int_node;
	struct TreeNode * identifier_node;
	struct TreeNode * assignement_node;
	struct TreeNode * exp_node;
	struct TreeNode * op_node;
	struct TreeNode * un_op_node;
	struct Token * next = pt;

	set_node_as_deepest(node);
	if (pt->type && (cmpstr(pt->type, "OPEN_PARENTHESIS"))){
		print_if_explicit("Expression in parenthesis\n");
		pt++;
		exp_node = Expression();

		if (pt->type && (cmpstr(pt->type, "CLOSE_PARENTHESIS") == false)) {
			expression_error = true;
			set_error();
		}
		pt++;

		set_node_as_child(node, exp_node);

	} 
	else if (
		pt->type && 
		(
		 cmpstr(pt->type, "ADDITION_OP") || 
		 cmpstr(pt->type, "NEGATION_OP") ||
		 cmpstr(pt->type, "LOGICAL_NEGATION_OP") ||
		 cmpstr(pt->type, "BITWISE_COMPLEMENT_OP")
		)
	){
		un_op_node = Unary_Statement();
		set_node_as_child(node, un_op_node);
		next = pt;
	}
	else if ((pt = next, remove_as_child(), pt->type) && Assignement_Statement()){
		next = pt;
	}
	else if ((pt = next, printf("INT %s\n", pt->type), remove_as_child(), pt->type) && (cmpstr(pt->type, "INT"))){
		printf("INT MATCH!!\n");
		int_node = Int();
		set_node_as_child(node, int_node);
		next = pt;
	}
	else if ((pt = next, remove_as_child(), pt->type) && (cmpstr(pt->type, "IDENTIFIER"))){
		identifier_node = Identifier_OP();
		set_node_as_child(node, identifier_node);
		next = pt;
	} 
	else {
		pt = next;
	       	remove_as_child();
		expression_error = true;
		set_error();
	}

	printf("FACTOR END %s\n", pt->type);

	remove_node_from_deepest();	

	return node;
}
/*
bool Declaration_Statement(){
	print_if_explicit("Declaration_Statement\n");
	struct TreeNode * node = create_node("DECLARATION_STATEMENT", "DECLARATION_STATEMENT");
	int success = false;

	set_as_child(node);
	set_node_as_deepest(node);

	if(pt->type && Type() && Identifier()){
		success = true;
	}

	remove_node_from_deepest();

	return success;
} */

 struct TreeNode * Declaration_Statement(){
	print_if_explicit("Declaration_Statement\n");
	struct TreeNode * node = create_node("DECLARATION_STATEMENT", "DECLARATION_STATEMENT");
	struct TreeNode * exp;
	struct TreeNode * assignement_op_node;
	struct TreeNode * assignement_node;
	struct TreeNode * int_keyword;
	struct TreeNode * identifier_node;
	set_node_as_deepest(node);


	if(cmpstr(pt->type, "INT_KEYWORD")){
		int_keyword = create_node(pt->type, pt->value.string);
		pt++;
		set_node_as_child(node, int_keyword); 

		identifier_node = create_node("IDENTIFIER", pt->value.string);
		pt++;

		if(cmpstr(pt->type, "SEMICOLON")){
			set_node_as_child(node, identifier_node); 
		} 
		else if (cmpstr(pt->type, "ASSIGN")) {
			assignement_op_node = Assignement_OP();
			set_node_as_child(node, assignement_op_node); 
			
			exp = Expression();
			set_node_as_child(assignement_op_node, identifier_node); 
			set_node_as_child(assignement_op_node, exp); 
			
			//assignement_node = Assignement_Statement(assignement_op_node, identifier_node, exp);

		}
		else if (cmpstr(pt->type, "COMA")) {
			printf("COMA IS NOT SUPPORTED!\n");
		}
	} else {
		printf("ERRROR NODEE %s!\n", pt->type);
		expression_error = true;
		set_error();
	}

	remove_node_from_deepest();

	printf("NODEE %s!\n", pt->type);
	return node;
} 

/* bool Declaration_Statement(){
	print_if_explicit("Declaration_Statement\n");
	struct TreeNode * node = create_node("DECLARATION_STATEMENT", "DECLARATION_STATEMENT");
	int success = false;
	struct Token * next = pt;

	set_as_child(node);
	set_node_as_deepest(node);

	if((pt = next, remove_as_child(), pt->type) && Type() && Identifier() && Match("ASSIGN") && Expression()){
		success = true;
	}
	else if((pt = next, remove_as_child(), pt->type) && Type() && Identifier() && Match("ASSIGN")){
		success = true;
	}

	remove_node_from_deepest();

	return success; 
} */

bool Assignement_Statement(){
	print_if_explicit("Assignement_Statement\n");
	printf("Assignement_Statement %s\n", pt->type);
	struct TreeNode * node = create_node("ASSIGNEMENT_STATEMENT", "ASSIGNEMENT_STATEMENT");
	int success = false;
	struct Token * next = pt;
	struct TreeNode * exp;

	set_as_child(node);
	set_node_as_deepest(node);

	if((pt = next, remove_as_child(), pt->type) && Identifier() && Match("ASSIGN") && (exp = Expression())){
		set_as_child(exp);
		success = true;
	} 

	remove_node_from_deepest();

	return success; 
/*
	print_if_explicit("Assignement_Statement\n");
	struct TreeNode * assignement_statement_node = create_node("ASSIGNEMENT_STATEMENT", "ASSIGNEMENT_STATEMENT");

	set_node_as_child(assignement_op_node, identifier_node);
	set_node_as_child(assignement_op_node, expression_node);
	set_node_as_child(assignement_statement_node, assignement_op_node); 
	
	return assignement_statement_node; */
}
/*
struct TreeNode * Assignement_Statement(struct TreeNode * assignement_op_node, struct TreeNode * identifier_node, struct TreeNode * expression_node){
	print_if_explicit("Assignement_Statement\n");
	printf("AAAAAAAAA %s\n", pt->type);
	struct TreeNode * node = create_node("ASSIGNEMENT_STATEMENT", "ASSIGNEMENT_STATEMENT");
	int success = false;
	struct Token * next = pt;

	set_as_child(node);
	set_node_as_deepest(node);

	if((pt = next, remove_as_child(), pt->type) && Type() && Identifier() && Match("ASSIGN") && Int()){
		success = true;
	}
	else if((pt = next, remove_as_child(), pt->type) && Type() && Identifier() && Match("ASSIGN") && Identifier()){
		success = true;
	}
	else if((pt = next, remove_as_child(), pt->type) && Identifier() && Match("ASSIGN") && Int()){
		success = true;
	}
	else if((pt = next, remove_as_child(), pt->type) && Identifier() && Match("ASSIGN") && Identifier()){
		success = true;
	}

	remove_node_from_deepest();

	return success; 

	print_if_explicit("Assignement_Statement\n");
	struct TreeNode * assignement_statement_node = create_node("ASSIGNEMENT_STATEMENT", "ASSIGNEMENT_STATEMENT");

	set_node_as_child(assignement_op_node, identifier_node);
	set_node_as_child(assignement_op_node, expression_node);
	set_node_as_child(assignement_statement_node, assignement_op_node); 
	
	return assignement_statement_node;
} */

struct TreeNode * Binary_Statement(struct TreeNode * op_node, struct TreeNode * term_node, struct TreeNode * next_term_node){
	print_if_explicit("Binary_Statement\n");
	struct TreeNode * binary_statement_node = create_node("BINARY_STATEMENT", "BINARY_STATEMENT");

	set_node_as_child(binary_statement_node, term_node);
	set_node_as_child(binary_statement_node, next_term_node);
	set_node_as_child(op_node->children[0], binary_statement_node);
	
	return op_node;
}

struct TreeNode * Unary_Statement(){
	print_if_explicit("Unary_Statement\n");
	struct TreeNode * fact_node;
	struct TreeNode * op_node;
	struct TreeNode * un_op_node;

	un_op_node = create_node("UNARY_STATEMENT", "UNARY_STATEMENT");
	op_node = Unary_OP();
	fact_node = Factor();

	set_node_as_child(un_op_node, op_node);
	set_node_as_child(un_op_node, fact_node);
	
	return un_op_node;
}

struct TreeNode * Int(){
	print_if_explicit("Int\n");
	struct TreeNode * int_node;

	int_node = create_node(pt->type, pt->value.string);
	pt++;
	
	return int_node;
}

struct TreeNode * Identifier_OP(){
	print_if_explicit("Identifier_OP\n");
	struct TreeNode * identifier_op_node;

	identifier_op_node = create_node(pt->type, pt->value.string);
	pt++;
	
	return identifier_op_node;
}

struct TreeNode * Unary_OP(){
	print_if_explicit("Unary_OP\n");
	struct TreeNode *node = create_node("UNARY_OP", "UNARY_OP");
	set_node_as_deepest(node);

	struct TreeNode *op_node = create_node(pt->type, pt->value.string);
	pt++;
	set_node_as_deepest(node);

	set_node_as_child(node, op_node);

	remove_node_from_deepest();
	remove_node_from_deepest();

	return node;
}

struct TreeNode * Binary_OP(){
	print_if_explicit("Binary_OP\n");
	struct TreeNode *node = create_node("BINARY_OP", "BINARY_OP");
	set_node_as_deepest(node);

	struct TreeNode *op_node = create_node(pt->type, pt->value.string);
	pt++;
	set_node_as_deepest(node);

	set_node_as_child(node, op_node);

	remove_node_from_deepest();
	remove_node_from_deepest();

	return node;
}

struct TreeNode * Assignement_OP(){
	print_if_explicit("Assignement_OP\n");
	struct TreeNode *node;

	node = create_node(pt->type, pt->value.string);
	pt++;

	return node;
}


bool Type(){
	print_if_explicit("Type\n");
	bool match;

	struct TreeNode *node = create_node("TYPE", "TYPE");
 	set_as_child(node);
	set_node_as_deepest(node);

	match = Match("INT_KEYWORD");

	remove_node_from_deepest();

	if(match) return true;
	set_error();

	return false;
}


bool Identifier(){
	print_if_explicit("Identifier\n");
	bool match;

	match = Match("IDENTIFIER");

	if(match) return true;
	set_error();

	return false;
}

bool Match(char *type){
	print_if_explicit("Match\n");
	if(SILENT_ARG == false)
		printf("\tMatch: SOURCE_CODE_POINTER: '%s'\n\tPARSER_NODE: '%s'\n", pt->type, type);
	if(pt->type && cmpstr(pt->type, type)){
		struct TreeNode * node = create_node(pt->type, pt->value.string);
 		set_as_child(node);
		pt++;
		return true;
	}

	set_error();

	return false;
}

bool compare_operator_by_precedence_level(int level){
	switch(level){
		case 0:
			return cmpstr(pt->type, "OR_OP") ||

			cmpstr(pt->type, "AND_OP") ||

			cmpstr(pt->type, "EQUAL_OP") ||
			cmpstr(pt->type, "NOT_EQUAL_OP") ||

			cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 1:
			return cmpstr(pt->type, "AND_OP") ||

			cmpstr(pt->type, "EQUAL_OP") ||
			cmpstr(pt->type, "NOT_EQUAL_OP") ||

			cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 2:
			return cmpstr(pt->type, "EQUAL_OP") ||
			cmpstr(pt->type, "NOT_EQUAL_OP") ||

			cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 3:
			return cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 4:
			return cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 5:
			return cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		default:
			return true;
	}

}

void set_as_child(struct TreeNode * current_node){
	// Assign as a child to previous node
	struct TreeNode * parent_node = *(pns - 1);
	
	(*parent_node).children[(*parent_node).children_amount] = current_node;
	(*parent_node).children_amount++;
}

void set_node_as_child(struct TreeNode * parent_node, struct TreeNode * current_node){
	(*parent_node).children[(*parent_node).children_amount] = current_node;
	(*parent_node).children_amount++;
}

void remove_as_child(){
	struct TreeNode * parent_node = *(pns - 1);
	// Remove as a child from previous node
	(*parent_node).children_amount = 0;
}

void remove_node_as_child(struct TreeNode * parent_node){
	if((*parent_node).children_amount > 0)
		(*parent_node).children_amount--;
}

void set_node_as_deepest(struct TreeNode * current_node){
	// Set current node as deepest 
	*pns = current_node;
	pns++;
}

void remove_node_from_deepest(){
	pns--;
}

struct TreeNode * create_node(char *type, char *value){
	struct TreeNode *node;
	node = (struct TreeNode *) malloc(sizeof(struct TreeNode));
	node->type = type;
	node->value = value;
	node->children_amount = 0;
	return node;
}

void set_error(){
	int curr_error_token_depth = pns - nodes_stack;

	if(curr_error_token_depth > error_tree_depth) {
		error_tree_depth = curr_error_token_depth;
		error_token = pt - 1;
	}
}

