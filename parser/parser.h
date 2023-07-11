#include "../compiler.h"

struct TreeNode * create_node(char *type, char *value);
bool Program();
bool Function();
bool Statement();
bool Expression();
bool Term();
bool Factor();
bool Type();
bool Identifier();
bool Unary_OP();
bool Binary_OP();
bool Match(char *type);
void remove_node_from_deepest();
void set_node_as_deepest(struct TreeNode * current_node);
void set_as_child(struct TreeNode * current_node);
void remove_as_child();
void set_error();

