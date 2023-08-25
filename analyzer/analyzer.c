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
	print_if_explicit("Analysis started.\n\n");

	tree_traverse_1(node);
	tree_traverse_2(node);
}

void tree_traverse_1(struct TreeNode * node){
	int i;
	struct FunctionRecord * fn_record;
	char * function_name;
	int function_args_amount;

	if(strcmp("FUNCTION_DECLARATION", node->type) == 0){
		print_if_explicit("FUNCTION_DECLARAION\n");

		function_name = node->children[1]->value;
		function_args_amount = node->children[2]->children_amount;
		fn_record = find_function_record(function_name);

		if(fn_record != -1 && fn_record->declared && (fn_record->declared_args_amount != function_args_amount)){
			printf("ERROR: Ambigous function declaration %s.\n", function_name);
			exit(1);
		}


		if(fn_record != -1 && fn_record->declared == false){
			fn_record->declared = true;
			fn_record->declared_args_amount = function_args_amount;
		} 
		else {
			register_function_declaration(function_name, function_args_amount);
		}
	}
	else if(strcmp("FUNCTION_DEFINITION", node->type) == 0){
		print_if_explicit("FUNCTION_DEFINITION\n");

		function_name = node->children[1]->value;
		function_args_amount = node->children[2]->children_amount;
		fn_record = find_function_record(function_name);

		if(fn_record != -1 && fn_record->defined){
			printf("ERROR: Function %s is already defined.\n", function_name);
			exit(1);
		}

		if(fn_record != -1 && (fn_record->declared_args_amount != function_args_amount)){
			printf("ERROR: Ambigous function definition %s.\n", function_name);
			exit(1);
		}

		if(fn_record != -1 && fn_record->defined == false){
			fn_record->defined = true;
			fn_record->defined_args_amount = function_args_amount;
		} else {
			register_function_definition(function_name, function_args_amount);
		}

	}

	for(i = 0; i < node->children_amount; i++){
		tree_traverse_1(node->children[i]);
	}
}

void tree_traverse_2(struct TreeNode * node){
	int i;
	struct FunctionRecord * fn_record;
	char * function_name;
	int function_args_amount;

	if(strcmp("FUNCTION_DECLARATION", node->type) == 0){
		print_if_explicit("FUNCTION_DECLARAION\n");

		function_name = node->children[1]->value;
		function_args_amount = node->children[2]->children_amount;
		fn_record = find_function_record(function_name);


	}
	else if(strcmp("FUNCTION_DEFINITION", node->type) == 0){
		print_if_explicit("FUNCTION_DEFINITION\n");

		function_name = node->children[1]->value;
		function_args_amount = node->children[2]->children_amount;
		fn_record = find_function_record(function_name);

	}
	else if(strcmp("FUNCTION_CALL", node->type) == 0){
		print_if_explicit("FUNCTION_CALL\n");

		function_name = node->children[0]->value;
		function_args_amount = node->children_amount == 2 ? node->children[1]->children_amount : 0;
		fn_record = find_function_record(function_name);

		if(fn_record == -1 || (fn_record->declared == false && fn_record->defined == false)){
			printf("ERROR: function %s is not declared or defined.\n", function_name);
			exit(1);
		}

		if(fn_record->defined && fn_record->defined_args_amount > function_args_amount) {
			printf("ERROR: in function call of %s arguments amount is smaller than in function definition.\n", function_name);
			exit(1);
		}

		if(fn_record->defined && fn_record->defined_args_amount < function_args_amount) {
			printf("ERROR: in function call of %s arguments amount is larger than in function definition.\n", function_name);
			exit(1);
		}

		if(fn_record->declared && fn_record->declared_args_amount > function_args_amount) {
			printf("ERROR: in function call of %s arguments amount is smaller than in function declaration.\n", function_name);
			exit(1);
		}

		if(fn_record->declared && fn_record->declared_args_amount < function_args_amount) {
			printf("ERROR: in function call of %s arguments amount is larger than in function declaration.\n", function_name);
			exit(1);
		}
	}

	for(i = 0; i < node->children_amount; i++){
		tree_traverse_2(node->children[i]);
	}
}

/* UTILS */

struct FunctionRecord * find_function_record(char * name){
	int i;

	for(i = 0; i < function_records_amount; i++){
		if(strcmp(name, records[i]->name) == 0){
			return records[i];
		}
	}

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

struct FunctionRecord * register_function_definition(char * name, int args_amount){
	struct FunctionRecord *record;

	record = (struct FunctionRecord *) malloc(sizeof(struct FunctionRecord));
	record->defined_args_amount = args_amount;
	record->name = name;
	record->defined = true;

	records[function_records_amount++] = record;
}

