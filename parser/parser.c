#include "./parser.h"

struct TreeNode {
	char * type;
	char * value;
	struct TreeNode * children[10];
	int children_amount;
};

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
void print_children(struct TreeNode * children[], int children_amount, bool explicit);
void print_parse_tree(struct TreeNode * root_node, bool explicit);

struct TreeNode * nodes_stack[100];
struct TreeNode ** pns = nodes_stack;
struct Token * pt = tokens;

int parse(){
	bool match;
	*pns = nodes_stack[0];

	match = Program();


	struct TreeNode * root_node = nodes_stack[0];

	if(PRINT_PARSE_TREE_ARG || PRINT_PARSE_TREE_EXPLICIT_ARG){
		print_parse_tree(root_node, PRINT_PARSE_TREE_EXPLICIT_ARG);
	}

	return 0;
}

void print_parse_tree(struct TreeNode * root_node, bool explicit){
	printf("\n\nPrint tree:\n");

	printf(" %s (%s) %d\n", root_node->type, root_node->value, root_node->children_amount);
	print_children(root_node->children, root_node->children_amount, explicit);
}

void print_children(struct TreeNode * children[], int children_amount, bool explicit){
	for(int i = 0; i < children_amount; i++){
		if(explicit)
			printf(" %s (%s) %d\t", children[i]->type, children[i]->value, children[i]->children_amount);
		else
			printf(" %s\t", children[i]->value);
	}
	printf("\n");

	
	for(int i = 0; i < children_amount; i++){
		if(children[i]->children_amount)
			print_children(children[i]->children, children[i]->children_amount, explicit);
		else 
			printf("\t");
	}

	return;
}

bool Program(){
	struct TreeNode *node = create_node("PROGRAM", "PROGRAM");
	printf("program address: %d\n", node);
	bool match;

	set_node_as_deepest(node);
	printf("program address: %d\n", node);

	match = Function();

	struct TreeNode * root_node = nodes_stack[0];

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
	printf("%s\n", (*pt).type);
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
	printf("Current node %d\n", current_node);
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
	printf("Node address %d\n", &node);
	return node;
}

