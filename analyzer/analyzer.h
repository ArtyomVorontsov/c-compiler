#include "../compiler.h"

void tree_traverse_1(struct TreeNode * node);
void tree_traverse_2(struct TreeNode * node);
void tree_traverse_3(struct TreeNode * node);

/* UTILS */
struct FunctionRecord * find_function_record(char * name);
void register_function_declaration(char * name, int args_amount);
void register_function_definition(char * name, int args_amount);

struct VariableRecord * find_variable_record(char * name);
void register_variable_declaration(char * name);
void register_variable_definition(char * name);
