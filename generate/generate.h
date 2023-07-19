#include "../compiler.h"
void generate_program(struct TreeNode * node);
void generate_function(struct TreeNode * node);
void generate_statement(struct TreeNode * node);
void generate_expression(struct TreeNode * node);
void generate_binary_op(struct TreeNode * node);
void generate_binary_statement( struct TreeNode * node, char * operator_type );
void generate_addition_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_substraction_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_multiplication_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_division_statement(struct TreeNode * operand1, struct TreeNode * operand2);
void generate_unary_op(struct TreeNode * node);
void generate_term(struct TreeNode * node);
void generate_fact(struct TreeNode * node);
void stack_push();
void stack_pop();
void generate_operand(struct TreeNode * node);

