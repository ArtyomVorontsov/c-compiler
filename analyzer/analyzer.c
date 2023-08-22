#include "analyzer.h"

struct FunctionRecord {
	char * name;
	bool declared;
	bool defined;
	int declared_args_amount;
	int defined_args_amount;
};

struct FunctionRecord * records[100];
int function_records_amount = 0;

void analyze(struct TreeNode * node){
	traverse_tree(node);
}

void traverse_tree(struct TreeNode * node){
	printf("TYPE: %s\n", node->type);
	int i;

	if(strcmp("FUNCTION_DECLARATION", node->type) == 0){
		struct FunctionRecord * fn_record = find_function_record(node->children[1]->value);

		if(fn_record != -1 && (fn_record->declared_args_amount != node->children[2]->children_amount)){
			printf("ERROR: Ambigous function declaration %s\n", node->children[1]->value);
			exit(1);
		}

		register_function_declaration(node->children[1]->value, node->children[2]->children_amount);
	}

	for(i = 0; i < node->children_amount; i++){
		traverse_tree(node->children[i]);
	}
}

/* UTILS */

struct FunctionRecord * find_function_record(char * name){
	int i;

	printf("HI %d\n", function_records_amount);
	printf("HI %s\n", name);
	for(i = 0; i < function_records_amount; i++){
		printf("i %d\n", i);
		printf("HI %s\n", records[i]->name);
		if(strcmp(name, records[i]->name) == 0){
			return records[i];
		}
	}
	printf("HI %d\n", function_records_amount);

	return (struct FunctionRecord *) -1;
}

struct FunctionRecord * register_function_declaration(char * name, int args_amount){
	struct FunctionRecord *record;

	record = (struct FunctionRecord *) malloc(sizeof(struct FunctionRecord));
	record->declared_args_amount = args_amount;
	record->name = name;
	record->declared = true;

	records[function_records_amount++] = record;
}

