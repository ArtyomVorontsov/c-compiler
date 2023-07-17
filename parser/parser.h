#include "../compiler.h"

struct TreeNode * create_node(char *type, char *value);
bool Program();
bool Function();
bool Statement();
struct TreeNode * Expression();
struct TreeNode * Term();
struct TreeNode * Factor();
struct TreeNode * BinOp(struct TreeNode * op_node, struct TreeNode * term_node, struct TreeNode * next_term_node);
bool Type();
bool Identifier();
bool Unary_OP();
struct TreeNode * Binary_OP(struct Token * token);
bool Match(char *type);
void remove_node_from_deepest();
void set_node_as_deepest(struct TreeNode * current_node);
void set_as_child(struct TreeNode * current_node);
void set_node_as_child(struct TreeNode * parent_node, struct TreeNode * current_node);
void remove_as_child();
void remove_node_as_child(struct TreeNode * parent_node);
void set_error();

