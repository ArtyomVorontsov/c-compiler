#include "../compiler.h"
void generate_program(struct TreeNode * node);
void generate_function(struct TreeNode * node);
void generate_statement(struct TreeNode * node);
void generate_multi_statement(struct TreeNode * node);
void generate_expression(struct TreeNode * node);
void generate_binary_op(struct TreeNode * node);
void generate_binary_statement( struct TreeNode * node, char * operator_type );
void generate_addition_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_substraction_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_multiplication_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_division_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_or_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_and_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_equal_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_not_equal_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_less_than_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_less_than_or_equal_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_greater_than_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_greater_than_or_equal_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_assignement_expression(struct TreeNode * node);
void generate_unary_op(struct TreeNode * node);
void generate_term(struct TreeNode * node);
void generate_fact(struct TreeNode * node);
void stack_push();
void stack_pop();
void generate_prologue();
void generate_epilogue();
void generate_operand(struct TreeNode * node);
char * generate_unique_label(char * l);
void generate_declaration_statement(struct TreeNode * node);

void register_var(char * name);
struct VarEntity * create_var_enity(char * name, int offset);
int find_var_index_by_name(char * name);
int find_var_index_by_name_with_error(char * name);
void unregister_var_by_name(char * name);

