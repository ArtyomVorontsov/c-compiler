#include "../compiler.h"

struct TreeNode * create_node(char *type, char *value);
bool Program();
bool Function();
bool Statement();
bool Multi_Statement();
struct TreeNode * Expression();
struct TreeNode * Logical_Or_Expression();
struct TreeNode * Logical_And_Expression();
struct TreeNode * Equality_Expression();
struct TreeNode * Relational_Expression();
struct TreeNode * Addictive_Expression();
struct TreeNode * Assignement_Expression();
struct TreeNode * Term();
struct TreeNode * Factor();
struct TreeNode * Binary_Statement(struct TreeNode * op_node, struct TreeNode * term_node, struct TreeNode * next_term_node);
struct TreeNode * Unary_Statement();
bool Assignement_Statement();
struct TreeNode * Declaration_Statement();
bool Type();
bool Identifier();
struct TreeNode * Var();
struct TreeNode * Unary_OP();
struct TreeNode * Binary_OP();
struct TreeNode * Int();
struct TreeNode * Identifier_OP();
struct TreeNode * Assignement_OP();
bool Match(char *type);
bool compare_operator_by_precedence_level(int level);
void remove_node_from_deepest();
void set_node_as_deepest(struct TreeNode * current_node);
void set_as_child(struct TreeNode * current_node);
void set_node_as_child(struct TreeNode * parent_node, struct TreeNode * current_node);
void remove_as_child();
void remove_node_as_child(struct TreeNode * parent_node);
void set_error();

