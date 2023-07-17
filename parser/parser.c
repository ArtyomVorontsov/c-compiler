#include "./parser.h"

struct TreeNode * nodes_stack[100];
struct TreeNode ** pns = nodes_stack;
struct Token * pt = tokens;
int error_tree_depth = -1;
struct Token * error_token;


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
		Statement() && 
		Match("CLOSE_BRACE");

	remove_node_from_deepest();

	if(match) return true;
	set_error();

	return false;
}

bool Statement(){
	print_if_explicit("Statement\n");
	bool match;
	struct TreeNode * node = create_node("STATEMENT", "STATEMENT");
	struct TreeNode * exp;

 	set_as_child(node);
	set_node_as_deepest(node);

	match = Match("RETURN_KEYWORD") && (exp = Expression(), set_node_as_child(node, exp), 1) && Match("SEMICOLON");

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
	term_node = Term();
	set_node_as_child(node, term_node); 

	while(
		pt->type && 
		(
		 cmpstr(pt->type, "ADDITION_OP") ||
		 cmpstr(pt->type, "NEGATION_OP") ||
		 cmpstr(pt->type, "MULTIPLICATION_OP") ||
		 cmpstr(pt->type, "DIVISION_OP")
		)
	){
		bin_op_node = Binary_OP(pt);
		next_term_node = Term(pt + 1);

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


	while(
		pt->type && 
		(
		 cmpstr(pt->type, "MULTIPLICATION_OP") ||
		 cmpstr(pt->type, "DIVISION_OP")
		)
	){
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
	struct TreeNode * exp_node;
	struct TreeNode * op_node;
	struct TreeNode * un_op_node;

	set_node_as_deepest(node);
	if (pt->type && (cmpstr(pt->type, "OPEN_PARENTHESIS"))){
		print_if_explicit("Expression in parenthesis\n");
		pt++;
		exp_node = Expression();

		if (pt->type && (cmpstr(pt->type, "CLOSE_PARENTHESIS") == false)) {
			set_error();
			printf("Missing parenthesis error.\n");
			exit(1);
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
	}
	else if (pt->type && (cmpstr(pt->type, "INT"))){
		int_node = Int();
		set_node_as_child(node, int_node);
	} else {
		set_error();
		printf("Factor error.\n");
		exit(1);
	}

	remove_node_from_deepest();	

	return node;	
}

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
	if(pt->type && cmpstr(pt->type, type)){
		if(SILENT_ARG == false)
			printf("\tMatch: SOURCE_CODE_POINTER: '%s'\n\tPARSER_NODE: '%s'\n", pt->type, type);
		struct TreeNode * node = create_node(pt->type, pt->value.string);
 		set_as_child(node);
		pt++;
		return true;
	}

	set_error();

	return false;
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
	(*parent_node).children_amount--;
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

