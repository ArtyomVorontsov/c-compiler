#include "../compiler.h"

void traverse_tree(struct TreeNode * node);
struct FunctionRecord * find_function_record(char * name);
struct FunctionRecord * register_function_declaration(char * name, int args_amount);
