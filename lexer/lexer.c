#include "./lexer.h"
int curr_line = 0;
int curr_line_char = 0;
char *new_line_ptr;

struct Token * lex(char *inp){
	char *p = inp, 
	*token_start = inp,
	*token_end = inp,
	*tmp;
	new_line_ptr = inp;
	int i = 0;

	while(*p != '\0'){

		/* OPEN_BRACE */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_OPEN_BRACE_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "OPEN_BRACE";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}
		
		/* CLOSE_BRACE */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_CLOSE_BRACE_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "CLOSE_BRACE";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}

		/* OPEN_PARENTHESIS */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_OPEN_PARENTHESIS_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "OPEN_PARENTHESIS";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}

		/* CLOSE_PARENTHESIS */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_CLOSE_PARENTHESIS_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "CLOSE_PARENTHESIS";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}

		/* SEMICOLON */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_SEMICOLON_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "SEMICOLON";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}

		/* INT */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_INT_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "INT";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}

		/* INT_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_INT_KEYWORD_FA(token_start);

		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "INT_KEYWORD";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}

		/* RETURN_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_RETURN_KEYWORD_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "RETURN_KEYWORD";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}

		/* IDENTIFIER */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_IDENTIFIER_FA(token_start);

		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "IDENTIFIER";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			curr_line_char = token_start - new_line_ptr;
			tokens[i].position.line = curr_line;
			tokens[i].position.line_char += curr_line_char;
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}
		
		/* WHITESPACE */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_WHITESPACE_FA(token_start);
		if(token_end == (void *) FA_FAILED) token_end = tmp;
		if(token_end != token_start) continue;
		
		
		if(SILENT_ARG != true)
			printf("No lex handler c: %c d: %d\n", *token_end, *token_end);
		break;	
	}

	
	if(SILENT_ARG != true){
		printf("%d Results:\n", i);
		while(i--)
			printf("Type: %s | Value: %s | line: %d | line char: %d\n", 
				tokens[i].type, 
				tokens[i].value.string, 
				tokens[i].position.line, 
				tokens[i].position.line_char
			);
	}	
	
	return tokens;
}

/* WHITESPACE FA */
char * machine_WHITESPACE_FA(char *p){
	while((*p == ' ') || (*p == '\t') || (*p == '\n')) {
		if(*p == '\n') set_new_line(p);
		p++;
	}

	return p;
} 

/* IDENTIFIER FA */
char * machine_IDENTIFIER_FA(char *p) {
	switch(*p){
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case '_':
			return machine_step2_IDENTIFIER_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}
}

char * machine_step2_IDENTIFIER_FA(char *p) {
	switch(*p){
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
		case 'i':
		case 'j':
		case 'k':
		case 'l':
		case 'm':
		case 'n':
		case 'o':
		case 'p':
		case 'q':
		case 'r':
		case 's':
		case 't':
		case 'u':
		case 'v':
		case 'w':
		case 'x':
		case 'y':
		case 'z':
		case '_':
			return machine_step2_IDENTIFIER_FA(p + 1);
		case ' ':
		case '(':
		case '\n':
		case '\t':
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}
}

/* INT KEYWORD FA */

char * machine_INT_KEYWORD_FA(char *p){
	switch (*p){
		case 'i': 
			return machine_step2_INT_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_INT_KEYWORD_FA(char *p){
	switch (*p){
		case 'n': 
			return machine_step3_INT_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_INT_KEYWORD_FA(char *p){
	switch (*p){
		case 't': 
			return machine_step4_INT_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step4_INT_KEYWORD_FA(char *p){
	switch (*p){
		case ' ': 
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

/* INT FA */

char * machine_INT_FA(char *p){
	switch(*p){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return machine_step2_INT_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_INT_FA(char *p){
	switch(*p){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return machine_step2_INT_FA(p + 1);
		case ' ':
		case ';':
		case '\n':
		case '\t':
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

/* OPEN_BRACE FA */

char * machine_OPEN_BRACE_FA(char *p){
	switch (*p){
		case '{': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* CLOSE_BRACE FA */

char * machine_CLOSE_BRACE_FA(char *p){
	switch (*p){
		case '}': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* OPEN_PARENTHESIS FA */

char * machine_OPEN_PARENTHESIS_FA(char *p){
	switch (*p){
		case '(': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* CLOSE_PARENTHESIS FA */

char * machine_CLOSE_PARENTHESIS_FA(char *p){
	switch (*p){
		case ')': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}


/* SEMICOLON FA */

char * machine_SEMICOLON_FA(char *p){
	switch (*p){
		case ';': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* RETURN FA */

char * machine_RETURN_KEYWORD_FA(char *p) {
	switch (*p){
		case 'r': 
			return machine_step2_RETURN_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 'e': 
			return machine_step3_RETURN_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 't': 
			return machine_step4_RETURN_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step4_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 'u': 
			return machine_step5_RETURN_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step5_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 'r': 
			return machine_step6_RETURN_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step6_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 'n': 
			return machine_step7_RETURN_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step7_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case ' ': 
		case ';': 
		case '\n':
		case '\t':
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

/* UTILS */

void cpy_str(char *from, char *to, char *buff, int buff_size){
	int i = 0;
	while(from <= to) {
		*buff = *from;
		from++;
		buff++;
		i++;
	}
}

void set_new_line(char *p){
	new_line_ptr = p;
	curr_line++;
}

