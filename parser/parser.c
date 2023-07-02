#include "./parser.h"

struct TreeNode {
	char type[20];
	char value[20];
	struct TreeNode *next;
	struct TreeNode *body;
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


struct Token * pt = tokens;

int parse(){

	bool match;
	match = Program();

	printf("Is valid: %d\n", match);

	return 0;
}

bool Program(){
	bool match;
	match = Function();
	if(match) return true;

	return false;
}

bool Function(){
	bool match;
	match = Type() && Identifier() && Match("OPEN_PARENTHESIS") && Match("CLOSE_PARENTHESIS") && Match("OPEN_BRACE") && Statement() && Match("CLOSE_BRACE");
	if(match) return true;

	return false;
}

bool Statement(){
	bool match;
	match = Match("RETURN_KEYWORD") && Expression() && Match("SEMICOLON");
	if(match) return true;

	return false;
}

bool Expression(){
	bool match;
	match = Match("INT");
	if(match) return true;

	return false;
}

bool Type(){
	bool match;
	match = Int();
	if(match) return true;

	return false;
}

bool Int(){
	bool match;
	match = Match("INT_KEYWORD");
	if(match) return true;

	return false;
}

bool Identifier(){
	return Match("IDENTIFIER");
}

bool Match(char *type){
	if(cmpstr((*pt).type, type)){
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


struct TreeNode create_node(char *type, char *value){
	struct TreeNode node = {
		.type = *type,
		.value = *value
	};

	return node;
}

