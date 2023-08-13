#include "../compiler.h"
void generate_program(struct TreeNode * node);
void generate_function(struct TreeNode * node);
void generate_function_body(struct TreeNode * node);
void generate_compound(struct TreeNode * node);
void generate_block_item(struct TreeNode * node);
void generate_statement(struct TreeNode * node);
void generate_expression(struct TreeNode * node);
void generate_binary_op(struct TreeNode * node);
void generate_conditional_statement(struct TreeNode * node);
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
void generate_conditional_expression(struct TreeNode * node);
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
void generate_declaration(struct TreeNode * node);

/* LOOPS */
void generate_loop(struct TreeNode * node);
void generate_for_loop(struct TreeNode * node);
void generate_while_loop(struct TreeNode * node);
void generate_do_while_loop(struct TreeNode * node);
void generate_break(struct TreeNode * node);

void register_loop(char * loop_start_label, char * loop_end_label);
char * get_current_loop_start_label();
char * get_current_loop_end_label();
void unregister_loop();

void register_var(char * name);
struct VarEntity * create_var_enity(char * name, int offset);
void unregister_var_by_name(char * name);
struct VarEntity * get_var_by_name(char * name);
struct VarEntity * get_var_by_name_in_current_scope(char * name);
struct VarEntity * get_var_by_name_with_error(char * name);
void enter_scope();
void exit_scope();
void create_var_map();

