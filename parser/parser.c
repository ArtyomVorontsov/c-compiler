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
	if(!match) 
		printf(
			"ERROR: Parsing is failed at line %d:%d. After '%s'\n", 
			error_token->position.line, 
			error_token->position.line_char, 
			error_token->value.string
		), 
		exit(1);

	struct TreeNode * root_node = nodes_stack[0];

	return root_node;
}


bool Program(){
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
	bool match;
	struct TreeNode * node = create_node("STATEMENT", "STATEMENT");
	struct TreeNode * exp;

 	set_as_child(node);
	set_node_as_deepest(node);

	match = Match("RETURN_KEYWORD") && (exp = Expression(), set_node_as_child(node, exp), 1) && Match("SEMICOLON");

	printf("end type %s\n", pt->type);
	remove_node_from_deepest();	

	if(match) return true;
	set_error();

	return false;
}

struct TreeNode * Expression(){
	printf("Expression\n");
	struct Token * next = pt;
	struct TreeNode * node = create_node("EXPRESSION", "EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;
	struct Token * op;

	set_node_as_deepest(node);
	term_node = Term();
	set_node_as_child(node, term_node); 
	op = pt;

	while(
		op->type && 
		(
		 cmpstr(op->type, "ADDITION_OP") ||
		 cmpstr(op->type, "NEGATION_OP") ||
		 cmpstr(op->type, "MULTIPLICATION_OP") ||
		 cmpstr(op->type, "DIVISION_OP")
		)
	){
		bin_op_node = Binary_OP(op);
		next_term_node = Term(op + 1);

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 

		op = pt ;	
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Term(){
	printf("Term\n");
	struct TreeNode * node = create_node("TERM", "TERM");
	struct TreeNode * factor_node;
	struct TreeNode * next_factor_node;
	struct TreeNode * op_node;
	struct Token * op;

	set_node_as_deepest(node);

	factor_node = Factor();
	set_node_as_child(node, factor_node);

	op = pt + 1;

	while(
		op->type && 
		(
		 cmpstr(op->type, "MULTIPLICATION_OP") ||
		 cmpstr(op->type, "DIVISION_OP")
		)
	){
		op_node = Binary_OP(op);
		next_factor_node = Factor(op + 1);

		factor_node = Binary_Statement(op_node, factor_node, next_factor_node);
		remove_node_as_child(node);
		set_node_as_child(node, factor_node); 
	
		op = pt;
	}

	remove_node_from_deepest();	

	return node;
}


struct TreeNode * Factor(){
	printf("Factor\n");
	struct TreeNode * node = create_node("FACTOR", "FACTOR");
	struct TreeNode * int_node;
	struct TreeNode * exp_node;
	struct TreeNode * op_node;
	struct TreeNode * un_op_node;
	struct Token * next = pt;

	set_node_as_deepest(node);
	if (next->type && (cmpstr(next->type, "OPEN_PARENTHESIS"))){
		pt++;
		exp_node = Expression();

		if (pt->type && (cmpstr(pt->type, "CLOSE_PARENTHESIS") == false)) set_error();
		pt++;

		set_node_as_child(node, exp_node);

	} 
	else if (
		next->type && 
		(
		 cmpstr(next->type, "ADDITION_OP") || 
		 cmpstr(next->type, "NEGATION_OP") ||
		 cmpstr(next->type, "LOGICAL_NEGATION_OP") ||
		 cmpstr(next->type, "BITWISE_COMPLEMENT_OP")
		)
	){
		un_op_node = create_node("UNARY_STATEMENT", "UNARY_STATEMENT");
		op_node = Unary_OP();
		exp_node = Expression();

		set_node_as_child(un_op_node, op_node);
		set_node_as_child(un_op_node, exp_node);

		set_node_as_child(node, un_op_node);

	}
	else if (next->type && (cmpstr(next->type, "INT"))){
		int_node = create_node(next->type, next->value.string);
		set_node_as_child(node, int_node);

		pt++;

	} else {
		set_error();
	}

	remove_node_from_deepest();	

	return node;	
}

struct TreeNode * Binary_Statement(struct TreeNode * op_node, struct TreeNode * term_node, struct TreeNode * next_term_node){
	printf("Binary_Statement\n");
	struct TreeNode * binary_statement_node = create_node("BINARY_STATEMENT", "BINARY_STATEMENT");

	set_node_as_child(binary_statement_node, term_node);
	set_node_as_child(binary_statement_node, next_term_node);
	set_node_as_child(op_node->children[0], binary_statement_node);
	
	return op_node;
}

struct TreeNode * Unary_OP(){
	printf("Unary_OP\n");
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
	printf("Binary_OP\n");
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
	bool match;

	match = Match("IDENTIFIER");

	if(match) return true;
	set_error();

	return false;
}

bool Match(char *type){
	if(pt->type && cmpstr(pt->type, type)){
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

