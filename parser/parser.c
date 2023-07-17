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

	printf("\tpt: %s\n", pt->type);
	term_node = Term();
	printf("\tPT AFTER TERM: %s\n", pt->type);
	op = pt ;

	int i = 0;

	while(
		op->type && 
		(
		 cmpstr(op->type, "ADDITION_OP") ||
		 cmpstr(op->type, "NEGATION_OP") ||
		 cmpstr(op->type, "MULTIPLICATION_OP") ||
		 cmpstr(op->type, "DIVISION_OP")
		)
	){
		printf("\top: %s %s\n", pt->type, pt->value.string);
		bin_op_node = Binary_OP(op);
		printf("\tpt after Binary_OP: %s %s\n", pt->type, pt->value.string);
		printf("\tnext term node: %s \n", (op+1)->type);
		next_term_node = Term(op + 1);
		printf("\tpt after next Term: %s %s\n", pt->type, pt->value.string);

		printf("PRINT\n");
		printf("term_node: %s\n", term_node->children[0]->type);
		printf("next_term_node: %s\n", next_term_node->children[0]->type);

		term_node = BinOp(bin_op_node, term_node, next_term_node);
		if(i == 0) 
			set_node_as_child(node, term_node); 

		op = pt + 1;	
		i++;
	}

	return node;
}

struct TreeNode * Term(){
	printf("Term\n");
	struct TreeNode * node = create_node("TERM", "TERM");
	struct TreeNode * factor_node;
	struct TreeNode * next_factor_node;
	struct TreeNode * op_node;
	struct Token * op;
	int i = 0;

	factor_node = Factor();
	set_node_as_child(node, factor_node);

	op = pt + 1;

	
	printf("FACTOR_NODE: %s\n", factor_node->type);
	while(
		op->type && 
		(
		 cmpstr(op->type, "MULTIPLICATION_OP") ||
		 cmpstr(op->type, "DIVISION_OP")
		)
	){
		op_node = Binary_OP(op);
		next_factor_node = Factor(op + 1);

		factor_node = BinOp(op_node, factor_node, next_factor_node);
	//	if(i == 0) set_node_as_child(node, factor_node);
	
		op = pt + 1;	
	}


	return node;
}


struct TreeNode * Factor(){
	printf("Factor\n");
	printf("Factor pt: %s\n", pt->type);
	struct TreeNode * node = create_node("FACTOR", "FACTOR");
	struct TreeNode * int_node;
	struct TreeNode * exp_node;

	struct Token * next = pt;

	if(
		next->type && 
		(
		 cmpstr(next->type, "OPEN_PARENTHESIS")
		)
	){
		pt++;
		exp_node = Expression();

		if(
			pt->type && (cmpstr((pt + 1)->type, "CLOSE_PARENTHESIS") == false)
			
		){
			// error
			exit(1);
		}
		pt++;

		set_node_as_child(node, exp_node);

		return node;	
	} 
	/* else if (node->children_amount = 0, pt = next, Unary_OP() && Factor()){
		remove_node_from_deepest();	
		return true;
	} */
	else if (
			next->type && 
			(
			 cmpstr(next->type, "INT")
			)
	){
		printf("INT!!!\n");
		int_node = create_node(next->type, next->value.string);
		set_node_as_child(node, int_node);

		pt++;

		return node;	
	}

	//exit(1);
	//set_error();

}

struct TreeNode * BinOp(struct TreeNode * op_node, struct TreeNode * term_node, struct TreeNode * next_term_node){
	printf("BinOp\n");
	//printf("term_node: %s\n", term_node->children[0]->type);
	//printf("next_term_node: %s\n", next_term_node->children[0]->type);

	set_node_as_child(op_node->children[0], term_node);
	set_node_as_child(op_node->children[0], next_term_node);
	
	return op_node;
}

bool Unary_OP(){
	struct Token * next = pt;
	struct TreeNode *node = create_node("UNARY_OP", "UNARY_OP");

 	set_as_child(node);
	set_node_as_deepest(node);

	if(node->children_amount = 0, pt = next, Match("NEGATION_OP")){
		remove_node_from_deepest();	
		return true;
	} 
	else if (node->children_amount = 0, pt = next, Match("BITWISE_COMPLEMENT_OP")){
		remove_node_from_deepest();	
		return true;
	} 
	else if (node->children_amount = 0, pt = next, Match("LOGICAL_NEGATION_OP")){
		remove_node_from_deepest();	
		return true;
	}

	remove_node_from_deepest();

	set_error();

	return false;
}

struct TreeNode * Binary_OP(struct Token * token){
	printf("Binary_OP\n");
	struct TreeNode *node = create_node("BINARY_OP", "BINARY_OP");
	struct TreeNode *op_node = create_node(token->type, token->value.string);
	pt++;

	set_node_as_child(node, op_node);

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

