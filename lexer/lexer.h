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


/* UTILS */
void cpy_str(char *from, char *to, char *buff, int buff_size);

