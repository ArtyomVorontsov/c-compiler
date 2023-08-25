#include "../compiler.h"

void tree_traverse_1(struct TreeNode * node);
void tree_traverse_2(struct TreeNode * node);
struct FunctionRecord * find_function_record(char * name);
struct FunctionRecord * register_function_declaration(char * name, int args_amount);
struct FunctionRecord * register_function_definition(char * name, int args_amount);
