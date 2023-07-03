#include "./parser.h"

struct TreeNode {
	char * type;
	char * value;
	struct TreeNode * children[10];
	int children_amount;
};

struct TreeNode create_node(char *type, char *value);
bool Program();
bool Function();
bool Statement();
bool Expression();
bool Type();
bool Int();
bool Identifier();
bool Match(char *type);
bool cmpstr(char *s1, char *s2);
void remove_node_from_deepest();
void set_node_as_deepest(struct TreeNode * current_node);
void set_as_child(struct TreeNode * current_node);
void printChildren(struct TreeNode * children[], int children_amount);

struct TreeNode * nodes_stack[100];
struct TreeNode ** pns = nodes_stack;
struct Token * pt = tokens;

int parse(){
	bool match;
	*pns = nodes_stack[0];

	match = Program();

	struct TreeNode root_node = **pns;

	printf("Is valid: %d\n", match);

	printf("Root node: %s\n", root_node.children[0]->type);
	printf("nodes_stack: %d, pns: %d\n", *nodes_stack, *pns);

	printf("\n\nPrint tree:\n");

	
	printChildren(root_node.children, root_node.children_amount);

	return 0;
}

void printChildren(struct TreeNode * children[], int children_amount){
	for(int i = 0; i < children_amount; i++){
		printf(" %s ", children[i]->type);
	}
	printf("\n");

	/*
	for(int i = 0; i < children_amount; i++){
		if(children[i]->children_amount)
			printChildren(children[i]->children, children[i]->children_amount);
	} */

	return;
}

bool Program(){
	struct TreeNode node = create_node("PROGRAM", "PROGRAM");
	bool match;

	set_node_as_deepest(&node);

	match = Function();

	remove_node_from_deepest();	

	if(match) return true;

	return false;
}

bool Function(){
	struct TreeNode node = create_node("FUNCTION", "FUCTION");
	bool match;

 	set_as_child(&node);
	set_node_as_deepest(&node);

	match = Type() && Identifier() && Match("OPEN_PARENTHESIS") && Match("CLOSE_PARENTHESIS") && Match("OPEN_BRACE") && Statement() && Match("CLOSE_BRACE");
	
	remove_node_from_deepest();	

	if(match) return true;

	return false;
}

bool Statement(){
	bool match;

	struct TreeNode node = create_node("STATEMENT", "STATEMENT");
 	set_as_child(&node);
	set_node_as_deepest(&node);

	match = Match("RETURN_KEYWORD") && Expression() && Match("SEMICOLON");

	remove_node_from_deepest();	

	if(match) return true;

	return false;
}

bool Expression(){
	bool match;

	struct TreeNode node = create_node("EXPRESSION", "EXPRESSION");
 	set_as_child(&node);
	set_node_as_deepest(&node);

	match = Match("INT");

	remove_node_from_deepest();	

	if(match) return true;

	return false;
}

bool Type(){
	bool match;

	struct TreeNode node = create_node("TYPE", "TYPE");
 	set_as_child(&node);
	set_node_as_deepest(&node);

	match = Int();

	remove_node_from_deepest();

	if(match) return true;

	return false;
}

bool Int(){
	bool match;

	struct TreeNode node = create_node("INT_KEYWORD", "INT_KEYWORD");
 	set_as_child(&node);
	set_node_as_deepest(&node);

	match = Match("INT_KEYWORD");

	remove_node_from_deepest();

	if(match) return true;

	return false;
}

bool Identifier(){
	bool match;

	struct TreeNode node = create_node("IDENTIFIER", "IDENTIFIER");
 	set_as_child(&node);
	set_node_as_deepest(&node);

	match = Match("IDENTIFIER");

	remove_node_from_deepest();


	if(match) return true;

	return false;
}

bool Match(char *type){
	printf("%s\n", (*pt).type);
	if(cmpstr((*pt).type, type)){
		struct TreeNode node = create_node((*pt).type, (*pt).value.string);
 		set_as_child(&node);
		pt++;
		return true;
	}
	return false;
}

bool cmpstr(char *s1, char *s2){
	bool is_equal = true;
	int str_len = 0;
	while(*(s1 + str_len++) != '\0');

	while(str_len--){
		if(*s1++ != *s2++) {
			is_equal = false;
		}
	}

	return is_equal;
}

void set_as_child(struct TreeNode * current_node){
	// Assign as a child to previous node
	printf("Set as child\n");
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

struct TreeNode create_node(char *type, char *value){
	struct TreeNode node = {
		.type = type,
		.value = value,
		.children_amount = 0
	};

	return node;
}

