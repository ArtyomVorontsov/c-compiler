#include "./parser.h"


struct TreeNode * create_node(char *type, char *value);
bool Program();
bool Function();
bool Statement();
bool Expression();
bool Type();
bool Identifier();
bool Match(char *type);
void remove_node_from_deepest();
void set_node_as_deepest(struct TreeNode * current_node);
void set_as_child(struct TreeNode * current_node);

struct TreeNode * nodes_stack[100];
struct TreeNode ** pns = nodes_stack;
struct Token * pt = tokens;

struct TreeNode * parse(){
	bool match;
	*pns = nodes_stack[0];

	match = Program();
	if(!match) printf("ERROR!!!\n");
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

	return false;
}

bool Function(){
	struct TreeNode *node = create_node("FUNCTION", "FUCTION");
	bool match;

 	set_as_child(node);
	set_node_as_deepest(node);

	match = Type() && Identifier() && Match("OPEN_PARENTHESIS") && Match("CLOSE_PARENTHESIS") && Match("OPEN_BRACE") && Statement() && Match("CLOSE_BRACE");

		
	struct TreeNode * root_node = nodes_stack[0];
	
	remove_node_from_deepest();	

	if(match) return true;

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

	return false;
}

bool Expression(){
	bool match;

	struct TreeNode *node = create_node("EXPRESSION", "EXPRESSION");
 	set_as_child(node);
	set_node_as_deepest(node);

	match = Match("INT");

	remove_node_from_deepest();	

	if(match) return true;

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

	return false;
}


bool Identifier(){
	bool match;

	match = Match("IDENTIFIER");

	if(match) return true;

	return false;
}

bool Match(char *type){
	if(cmpstr((*pt).type, type)){
		struct TreeNode * node = create_node((*pt).type, (*pt).value.string);
 		set_as_child(node);
		pt++;
		return true;
	}
	return false;
}


void set_as_child(struct TreeNode * current_node){
	// Assign as a child to previous node
	struct TreeNode * parent_node = *(pns - 1);
	
	(*parent_node).children[(*parent_node).children_amount] = current_node;
	(*parent_node).children_amount++;
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
	//(TreeNode *) 
	struct TreeNode *node;
	node = (struct TreeNode *) malloc(sizeof(struct TreeNode));
	node->type = type;
	node->value = value;
	node->children_amount = 0;
	return node;
}

