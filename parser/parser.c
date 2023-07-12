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
 	set_as_child(node);
	set_node_as_deepest(node);

	match = Match("RETURN_KEYWORD") && Expression() && Match("SEMICOLON");

	remove_node_from_deepest();	

	if(match) return true;
	set_error();

	return false;
}

bool Expression(){
	struct Token * next = pt;
	struct Token * lookahead = pt + 1;
	bool lookahead_is_bin_op = false;
	bool success = false;

	struct TreeNode *node = create_node("EXPRESSION", "EXPRESSION");
 	set_as_child(node);
	set_node_as_deepest(node);

	// At least 1 Term production will be invoked
	if((node->children_amount = 0, pt = next, Term())){
		success = true;
		lookahead = pt;
		lookahead_is_bin_op = lookahead->type && 
		(
		 cmpstr(lookahead->type, "ADDITION_OP") ||
		 cmpstr(lookahead->type, "NEGATION_OP") ||
		 cmpstr(lookahead->type, "MULTIPLICATION_OP") ||
		 cmpstr(lookahead->type, "DIVISION_OP")
		);

		while(lookahead_is_bin_op){
			next = pt;
			
			// Invoke binary op productions
			if(lookahead_is_bin_op){
				if((Match("ADDITION_OP") || Match("NEGATION_OP")) && Term()){
					success = true;
				} else {
					success = false;
				}
			} else {
				break;
			}

			lookahead = pt;
			lookahead_is_bin_op = lookahead->type && 
			(
			 cmpstr(lookahead->type, "ADDITION_OP") ||
			 cmpstr(lookahead->type, "NEGATION_OP") ||
			 cmpstr(lookahead->type, "MULTIPLICATION_OP") ||
			 cmpstr(lookahead->type, "DIVISION_OP")
			);
		}
		remove_node_from_deepest();	
		return success;

	} 

	remove_node_from_deepest();	

	set_error();

	return false;
}

bool Term(){
	struct Token * next = pt;
	struct Token * lookahead = pt + 1;
	bool lookahead_is_bin_op = false;
	bool success = false;
	struct TreeNode *node = create_node("TERM", "TERM");

 	set_as_child(node);
	set_node_as_deepest(node);

	// At least 1 Factor production will be invoked
	if(node->children_amount = 0, pt = next, Factor()){
		success = true;
		lookahead = pt;
		lookahead_is_bin_op = lookahead->type && 
		(cmpstr(lookahead->type, "MULTIPLICATION_OP") ||
		 cmpstr(lookahead->type, "DIVISION_OP"));

		// Invoke binary op productions
		while(lookahead_is_bin_op){
			next = pt;
		
			if(lookahead_is_bin_op){
				if((Match("MULTIPLICATION_OP") || Match("DIVISION_OP")) && Factor()){
					success = true;
				} else {
					success = false;
				}
			} else {
				break;
			}
	
			lookahead = pt;
			lookahead_is_bin_op = lookahead->type && 
			(cmpstr(lookahead->type, "MULTIPLICATION_OP") ||
			 cmpstr(lookahead->type, "DIVISION_OP"));
		}
		remove_node_from_deepest();	

		return success;
	} 

	remove_node_from_deepest();	

	set_error();

	return false;
}

bool Factor(){
	struct Token * next = pt;
	struct TreeNode *node = create_node("FACTOR", "FACTOR");

 	set_as_child(node);
	set_node_as_deepest(node);

	if(node->children_amount = 0, pt = next, Match("OPEN_PARENTHESIS") && Expression() && Match("CLOSE_PARENTHESIS")){
		remove_node_from_deepest();	
		return true;
	} 
	else if (node->children_amount = 0, pt = next, Unary_OP() && Factor()){
		remove_node_from_deepest();	
		return true;
	}
	else if (node->children_amount = 0, pt = next, Match("INT")){
		remove_node_from_deepest();
		return true;
	}

	remove_node_from_deepest();	

	set_error();

	return false;
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

bool Binary_OP(){
	struct Token * next = pt;
	struct TreeNode *node = create_node("BINARY_OP", "BINARY_OP");

 	set_as_child(node);
	set_node_as_deepest(node);

	if(node->children_amount = 0, pt = next, Match("NEGATION_OP")){
		remove_node_from_deepest();	
		return true;
	} 
	else if (node->children_amount = 0, pt = next, Match("ADDITION_OP")){
		remove_node_from_deepest();	
		return true;
	} 
	else if (node->children_amount = 0, pt = next, Match("MULTIPLICATION_OP")){
		remove_node_from_deepest();	
		return true;
	} 
	else if (node->children_amount = 0, pt = next, Match("DIVISION_OP")){
		remove_node_from_deepest();	
		return true;
	} 

	remove_node_from_deepest();

	set_error();

	return false;
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

