#include "../compiler.h"
void generate_program(struct TreeNode * node);
void generate_function(struct TreeNode * node);
void generate_statement(struct TreeNode * node);
void generate_expression(struct TreeNode ** node);
int generate_term(struct TreeNode * node);
int generate_term_r(struct TreeNode * node);
void generate_fact(struct TreeNode * node);
void generate_fact_r(struct TreeNode * node);
void generate_unary_op(struct TreeNode * node);

