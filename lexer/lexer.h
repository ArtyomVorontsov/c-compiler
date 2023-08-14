#include "../compiler.h"

char * machine_INT_KEYWORD_FA(char *p);
char * machine_step2_INT_KEYWORD_FA(char *p);
char * machine_step3_INT_KEYWORD_FA(char *p);
char * machine_step4_INT_KEYWORD_FA(char *p);

char * machine_RETURN_KEYWORD_FA(char *p);
char * machine_step2_RETURN_KEYWORD_FA(char *p);
char * machine_step3_RETURN_KEYWORD_FA(char *p);
char * machine_step4_RETURN_KEYWORD_FA(char *p);
char * machine_step5_RETURN_KEYWORD_FA(char *p);
char * machine_step6_RETURN_KEYWORD_FA(char *p);
char * machine_step7_RETURN_KEYWORD_FA(char *p);

char * machine_IDENTIFIER_FA(char *p);
char * machine_step2_IDENTIFIER_FA(char *p);

char * machine_INT_FA(char *p);
char * machine_step2_INT_FA(char *p);

char * machine_WHITESPACE_FA(char *p);

char * machine_OPEN_BRACE_FA(char *p);
char * machine_CLOSE_BRACE_FA(char *p);

char * machine_OPEN_PARENTHESIS_FA(char *p);
char * machine_CLOSE_PARENTHESIS_FA(char *p);

char * machine_SEMICOLON_FA(char *p);

char * machine_ASSIGN_FA(char *p);

/* OPERATORS FA */
char * machine_NEGATION_OP_FA(char *p);
char * machine_ADDITION_OP_FA(char *p);
char * machine_MULTIPLICATION_OP_FA(char *p);
char * machine_DIVISION_OP_FA(char *p);
char * machine_BITWISE_COMPLEMENT_OP_FA(char *p);
char * machine_LOGICAL_NEGATION_OP_FA(char *p);
char * machine_AND_OP_FA(char *p);
char * machine_step2_AND_OP_FA(char *p);
char * machine_OR_OP_FA(char *p);
char * machine_step2_OR_OP_FA(char *p);
char * machine_EQUAL_OP_FA(char *p);
char * machine_step2_EQUAL_OP_FA(char *p);
char * machine_NOT_EQUAL_OP_FA(char *p);
char * machine_step2_NOT_EQUAL_OP_FA(char *p);
char * machine_LESS_THAN_OP_FA(char *p);
char * machine_LESS_THAN_OR_EQUAL_OP_FA(char *p);
char * machine_step2_LESS_THAN_OR_EQUAL_OP_FA(char *p);
char * machine_GREATER_THAN_OP_FA(char *p);
char * machine_GREATER_THAN_OR_EQUAL_OP_FA(char *p);
char * machine_step2_GREATER_THAN_OR_EQUAL_OP_FA(char *p);
char * machine_COMA_OP_FA(char *p);

/* CONDITIONALS */
char * machine_IF_KEYWORD_FA(char *p);
char * machine_step2_IF_KEYWORD_FA(char *p);
char * machine_step3_IF_KEYWORD_FA(char *p);

char * machine_ELSE_KEYWORD_FA(char *p);
char * machine_step2_ELSE_KEYWORD_FA(char *p);
char * machine_step3_ELSE_KEYWORD_FA(char *p);
char * machine_step4_ELSE_KEYWORD_FA(char *p);
char * machine_step5_ELSE_KEYWORD_FA(char *p);

char * machine_COLON_OP_FA(char *p);
char * machine_QUESTION_MARK_OP_FA(char *p);

/* LOOPS */
char * machine_FOR_KEYWORD_FA(char *p);
char * machine_step2_FOR_KEYWORD_FA(char *p);
char * machine_step3_FOR_KEYWORD_FA(char *p);
char * machine_step4_FOR_KEYWORD_FA(char *p);

char * machine_WHILE_KEYWORD_FA(char *p);
char * machine_step2_WHILE_KEYWORD_FA(char *p);
char * machine_step3_WHILE_KEYWORD_FA(char *p);
char * machine_step4_WHILE_KEYWORD_FA(char *p);
char * machine_step5_WHILE_KEYWORD_FA(char *p);
char * machine_step6_WHILE_KEYWORD_FA(char *p);

char * machine_DO_KEYWORD_FA(char *p);
char * machine_step2_DO_KEYWORD_FA(char *p);
char * machine_step3_DO_KEYWORD_FA(char *p);

char * machine_BREAK_KEYWORD_FA(char *p);
char * machine_step2_BREAK_KEYWORD_FA(char *p);
char * machine_step3_BREAK_KEYWORD_FA(char *p);
char * machine_step4_BREAK_KEYWORD_FA(char *p);
char * machine_step5_BREAK_KEYWORD_FA(char *p);
char * machine_step6_BREAK_KEYWORD_FA(char *p);

char * machine_CONTINUE_KEYWORD_FA(char *p);
char * machine_step2_CONTINUE_KEYWORD_FA(char *p);
char * machine_step3_CONTINUE_KEYWORD_FA(char *p);
char * machine_step4_CONTINUE_KEYWORD_FA(char *p);
char * machine_step5_CONTINUE_KEYWORD_FA(char *p);
char * machine_step6_CONTINUE_KEYWORD_FA(char *p);
char * machine_step7_CONTINUE_KEYWORD_FA(char *p);
char * machine_step8_CONTINUE_KEYWORD_FA(char *p);
char * machine_step9_CONTINUE_KEYWORD_FA(char *p);

/* UTILS */
void cpy_str(char *from, char *to, char *buff, int buff_size);
void set_new_line(char *p);
void set_token(char * type, int i, char * token_start, char * token_end);

