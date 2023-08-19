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

		/* IF_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_IF_KEYWORD_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "IF_KEYWORD";
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

		/* ELSE_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_ELSE_KEYWORD_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "ELSE_KEYWORD";
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

		/* FOR_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_FOR_KEYWORD_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "FOR_KEYWORD";
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

		/* WHILE_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_WHILE_KEYWORD_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "WHILE_KEYWORD";
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

		/* DO_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_DO_KEYWORD_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "DO_KEYWORD";
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

		/* BREAK_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_BREAK_KEYWORD_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "BREAK_KEYWORD";
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

		/* CONTINUE_KEYWORD */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_CONTINUE_KEYWORD_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "CONTINUE_KEYWORD";
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

		/* OPERATORS */

		/* NEGATION */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_NEGATION_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "NEGATION_OP";
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

		/* ADDITION */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_ADDITION_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "ADDITION_OP";
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


		/* MULTIPLICATION */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_MULTIPLICATION_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "MULTIPLICATION_OP";
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

		/* DIVISION */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_DIVISION_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "DIVISION_OP";
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

		/* BITWISE COMPLEMENT */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_BITWISE_COMPLEMENT_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "BITWISE_COMPLEMENT_OP";
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

		/* AND */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_AND_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "AND_OP";
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

		/* OR */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_OR_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "OR_OP";
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

		/* EQUAL */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_EQUAL_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "EQUAL_OP";
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

		/* NOT_EQUAL */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_NOT_EQUAL_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "NOT_EQUAL_OP";
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

		/* LOGICAL NEGATION */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_LOGICAL_NEGATION_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "LOGICAL_NEGATION_OP";
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


		/* LESS_THAN_OR_EQUAL */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_LESS_THAN_OR_EQUAL_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "LESS_THAN_OR_EQUAL_OP";
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

		/* GREATER_THAN_OR_EQUAL */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_GREATER_THAN_OR_EQUAL_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "GREATER_THAN_OR_EQUAL_OP";
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

		/* LESS_THAN */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_LESS_THAN_OP_FA(token_start);

		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "LESS_THAN_OP";
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

		/* GREATER_THAN */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_GREATER_THAN_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "GREATER_THAN_OP";
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

		/* ASSIGN */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_ASSIGN_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "ASSIGN";
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

		/* COLON */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_COLON_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "COLON_OP";
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

		/* QUESTION_MARK */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_QUESTION_MARK_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "QUESTION_MARK_OP";
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

		/* COMA */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_COMA_OP_FA(token_start);
		if(token_end != (void *) FA_FAILED) { 
			tokens[i].type = "COMA_OP";
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
			return machine_step2_IDENTIFIER_FA(p + 1);
		case '+':
		case '-':
		case '*':
		case '/':
		case ' ':
		case '(':
		case ')':
		case ';':
		case ',':
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
		case '-':
		case '+':
		case '/':
		case '*':
		case ')':
		case '<':
		case '>':
		case '=':
		case '&':
		case '|':
		case ',':
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

/* OPERATORS */

/* NEGATION OP FA */
char * machine_NEGATION_OP_FA(char *p) {
	switch (*p){
		case '-': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* BITWISE COMPLEMENT OP FA */
char * machine_BITWISE_COMPLEMENT_OP_FA(char *p) {
	switch (*p){
		case '~': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* LOGICAL NEGATION OP FA */
char * machine_LOGICAL_NEGATION_OP_FA(char *p) {
	switch (*p){
		case '!': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* ADDITION OP FA */
char * machine_ADDITION_OP_FA(char *p) {
	switch (*p){
		case '+': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* MULTIPLICATION OP FA */
char * machine_MULTIPLICATION_OP_FA(char *p) {
	switch (*p){
		case '*': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* DIVISION OP FA */
char * machine_DIVISION_OP_FA(char *p) {
	switch (*p){
		case '/': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* AND OP FA */
char * machine_AND_OP_FA(char *p) {
	switch (*p){
		case '&': 
			return machine_step2_AND_OP_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}


char * machine_step2_AND_OP_FA(char *p) {
	switch (*p){
		case '&': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}


/* OR OP FA */
char * machine_OR_OP_FA(char *p) {
	switch (*p){
		case '|': 
			return machine_step2_OR_OP_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_OR_OP_FA(char *p) {
	switch (*p){
		case '|': 
			return p;
		default:
			return (void *) FA_FAILED;
	}
}

/* EQUAL OP FA */
char * machine_EQUAL_OP_FA(char *p) {
	switch (*p){
		case '=': 
			return machine_step2_EQUAL_OP_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_EQUAL_OP_FA(char *p) {
	switch (*p){
		case '=': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* NOT_EQUAL OP FA */
char * machine_NOT_EQUAL_OP_FA(char *p) {
	switch (*p){
		case '!': 
			return machine_step2_NOT_EQUAL_OP_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_NOT_EQUAL_OP_FA(char *p) {
	switch (*p){
		case '=': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* LESS_THAN OP FA */
char * machine_LESS_THAN_OP_FA(char *p) {
	switch (*p){
		case '<': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* LESS_THAN_OR_EQUAL OP FA */
char * machine_LESS_THAN_OR_EQUAL_OP_FA(char *p) {
	switch (*p){
		case '<': 
			return machine_step2_LESS_THAN_OR_EQUAL_OP_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_LESS_THAN_OR_EQUAL_OP_FA(char *p) {
	switch (*p){
		case '=': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* GREATER_THAN OP FA */
char * machine_GREATER_THAN_OP_FA(char *p) {
	switch (*p){
		case '>': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* GREATER_THAN_OR_EQUAL OP FA */
char * machine_GREATER_THAN_OR_EQUAL_OP_FA(char *p) {
	switch (*p){
		case '>': 
			return machine_step2_GREATER_THAN_OR_EQUAL_OP_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_GREATER_THAN_OR_EQUAL_OP_FA(char *p) {
	switch (*p){
		case '=': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* ASSIGN FA */
char * machine_ASSIGN_FA(char *p) {
	switch (*p){
		case '=': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

/* CONDITIONALS */
char * machine_IF_KEYWORD_FA(char *p) {
	switch (*p){
		case 'i': 
			return machine_step2_IF_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_IF_KEYWORD_FA(char *p) {
	switch (*p){
		case 'f': 
			return machine_step3_IF_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_IF_KEYWORD_FA(char *p) {
	switch (*p){
		case ' ': 
		case '(': 
		case '\n': 
		case '\t': 
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_ELSE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'e': 
			return machine_step2_ELSE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_ELSE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'l': 
			return machine_step3_ELSE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_ELSE_KEYWORD_FA(char *p) {
	switch (*p){
		case 's': 
			return machine_step4_ELSE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step4_ELSE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'e': 
			return machine_step5_ELSE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step5_ELSE_KEYWORD_FA(char *p) {
	switch (*p){
		case ' ': 
		case '{': 
		case '\n': 
		case '\t': 
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_COLON_OP_FA(char *p) {
	switch (*p){
		case ':': 
			return p;
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_QUESTION_MARK_OP_FA(char *p) {
	switch (*p){
		case '?': 
			return p + 1;
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_COMA_OP_FA(char *p) {
	switch (*p){
		case ',': 
			return p + 1;
		default:
			return (void *) FA_FAILED;
	}	
}

/* LOOPS */
char * machine_FOR_KEYWORD_FA(char *p) {
	switch (*p){
		case 'f': 
			return machine_step2_FOR_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_FOR_KEYWORD_FA(char *p) {
	switch (*p){
		case 'o': 
			return machine_step3_FOR_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_FOR_KEYWORD_FA(char *p) {
	switch (*p){
		case 'r': 
			return machine_step4_FOR_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step4_FOR_KEYWORD_FA(char *p) {
	switch (*p){
		case ' ': 
		case '(': 
		case '\t': 
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_WHILE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'w': 
			return machine_step2_WHILE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_WHILE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'h': 
			return machine_step3_WHILE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_WHILE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'i': 
			return machine_step4_WHILE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step4_WHILE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'l': 
			return machine_step5_WHILE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step5_WHILE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'e': 
			return machine_step6_WHILE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step6_WHILE_KEYWORD_FA(char *p) {
	switch (*p){
		case ' ': 
		case '(': 
		case '\t': 
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_DO_KEYWORD_FA(char *p) {
	switch (*p){
		case 'd': 
			return machine_step2_DO_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_DO_KEYWORD_FA(char *p) {
	switch (*p){
		case 'o': 
			return machine_step3_DO_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_DO_KEYWORD_FA(char *p) {
	switch (*p){
		case ' ': 
		case '{': 
		case '\t': 
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_BREAK_KEYWORD_FA(char *p) {
	switch (*p){
		case 'b': 
			return machine_step2_BREAK_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_BREAK_KEYWORD_FA(char *p) {
	switch (*p){
		case 'r': 
			return machine_step3_BREAK_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_BREAK_KEYWORD_FA(char *p) {
	switch (*p){
		case 'e': 
			return machine_step4_BREAK_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step4_BREAK_KEYWORD_FA(char *p) {
	switch (*p){
		case 'a': 
			return machine_step5_BREAK_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step5_BREAK_KEYWORD_FA(char *p) {
	switch (*p){
		case 'k': 
			return machine_step6_BREAK_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step6_BREAK_KEYWORD_FA(char *p) {
	switch (*p){
		case ';': 
			return p - 1;
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'c': 
			return machine_step2_CONTINUE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step2_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'o': 
			return machine_step3_CONTINUE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step3_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'n': 
			return machine_step4_CONTINUE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step4_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case 't': 
			return machine_step5_CONTINUE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step5_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'i': 
			return machine_step6_CONTINUE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step6_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'n': 
			return machine_step7_CONTINUE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step7_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'u': 
			return machine_step8_CONTINUE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step8_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case 'e': 
			return machine_step9_CONTINUE_KEYWORD_FA(p + 1);
		default:
			return (void *) FA_FAILED;
	}	
}

char * machine_step9_CONTINUE_KEYWORD_FA(char *p) {
	switch (*p){
		case ';': 
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

void set_token(char * type, int i, char * token_start, char * token_end){
	tokens[i].type = type;
	cpy_str(token_start, token_end, tokens[i].value.string, 100);
	curr_line_char = token_start - new_line_ptr;
	tokens[i].position.line = curr_line;
	tokens[i].position.line_char += curr_line_char;
	i++;
	token_end++;
}

