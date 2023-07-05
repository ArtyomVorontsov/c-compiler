#include "./generate.h"


void generate(struct TreeNode * root_node){
	printf("\nCode generation!\n");
	
	generate_program(root_node->children[0]);
}

void generate_program(struct TreeNode * node){
	printf("type: %s\n", node->type);
	if(strcmp(node->type, "FUNCTION") == 0){
			generate_function(node);
	}
	else 
		printf("No handler\n");
	
	/*	
	switch(node->type){
		case "FUNCTION":
			generate_function(node);
			break;

		default:
			printf("No handler\n");

	} */
}

void generate_function(struct TreeNode * node){
	char *identifier_value = node->children[1]->value;
	struct TreeNode * statement_node = node->children[5];
	printf(".globl _%s\n", identifier_value);
	printf("_%s:\n", identifier_value);

	generate_statement(statement_node);
}

void generate_statement(struct TreeNode * node){
	struct TreeNode * child_node;

	for(int i = node->children_amount; i > 0; i--){
		child_node = node->children[i];
	

		if(strcmp(node->type, "RETURN") == 0) {
			printf("ret");	
		} 
		else if(strcmp(node->type, "FUNCTION") == 0) {
			printf("mov $%s", child_node->value);	
		}
		else {
			printf("No handler\n");
		}
		/*
		switch(child_node->type){
			case "RETURN":
				printf("ret");	
				break;
			case "EXPRESSION":
				printf("mov $%s", child_node->value.string);	
				break;
			default:
				printf("No handler\n");
				break;
		} */
	}
}

