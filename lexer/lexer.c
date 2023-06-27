#include <stdio.h>
#include <string.h>
#include <ctype.h>

union Value {
	int number;
	char string[100];
};

struct Token {
	char *type;
	union Value value;
};

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

int lex(char *inp);

/* UTILS */
void cpy_str(char *from, char *to, char *buff, int buff_size);

int main(){
	char inp[1000] = { '\0' };

	printf("Type your program\n");

	fgets(inp, 1000, stdin);	

	scanf("%s", inp);
	printf("done\n");
	int out = lex(inp);

	return out;
}

int lex(char *inp){
	char *p = inp, 
	*token_start = inp,
	*token_end = inp,
	*tmp;
	struct Token tokens[100];
	int i = 0;
	

	while(*p != '\0'){

		/* OPEN_BRACE */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_OPEN_BRACE_FA(token_start);
		if(token_end != -1) { 
			tokens[i].type = "OPEN_BRACE";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
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
		if(token_end != -1) { 
			tokens[i].type = "CLOSE_BRACE";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
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
		if(token_end != -1) { 
			tokens[i].type = "OPEN_PARENTHESIS";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
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
		if(token_end != -1) { 
			tokens[i].type = "CLOSE_PARENTHESIS";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
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
		if(token_end != -1) { 
			tokens[i].type = "SEMICOLON";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
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
		if(token_end != -1) { 
			tokens[i].type = "INT";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
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

		if(token_end != -1) { 
			tokens[i].type = "INT_KEYWORD";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
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
		if(token_end != -1) { 
			tokens[i].type = "RETURN_KEYWORD";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
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

		if(token_end != -1) { 
			tokens[i].type = "IDENTIFIER";
			cpy_str(token_start, token_end, tokens[i].value.string, 100);
			i++;
			token_end++;
			continue;
		} else {
			token_end = tmp;
		}
		
		/* SKIP WHITESPACE */
		/* TODO FIX WHITESPACE HANDLER */
		tmp = token_end;
		token_start = token_end;
		token_end = machine_WHITESPACE_FA(token_start);
		if(token_end == -1) token_end = tmp;
		if(token_end != token_start) continue;
		

		printf("No lex handler\n");
		break;	
	}

	printf("%d Results:\n", i);
	while(i--)
		printf("Type: %s | Value: %s\n", tokens[i].type, tokens[i].value.string);
}
/* WHITESPACE FA */

char * machine_WHITESPACE_FA(char *p){
	if(*p == ' ') {
		while(*(p++) == ' ');
		return p - 1;
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
			return -1;
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
			return p;
		default:
			return -1;
	}
}

/* INT KEYWORD FA */

char * machine_INT_KEYWORD_FA(char *p){
	switch (*p){
		case 'i': 
			return machine_step2_INT_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step2_INT_KEYWORD_FA(char *p){
	switch (*p){
		case 'n': 
			return machine_step3_INT_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step3_INT_KEYWORD_FA(char *p){
	switch (*p){
		case 't': 
			return machine_step4_INT_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step4_INT_KEYWORD_FA(char *p){
	switch (*p){
		case ' ': 
			return p - 1;
		default:
			return -1;
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
			return -1;
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
			return p - 1;
		default:
			return -1;
	}	
}

/* OPEN_BRACE FA */

char * machine_OPEN_BRACE_FA(char *p){
	switch (*p){
		case '{': 
			return p;
		default:
			return -1;
	}	
}

/* CLOSE_BRACE FA */

char * machine_CLOSE_BRACE_FA(char *p){
	switch (*p){
		case '}': 
			return p;
		default:
			return -1;
	}	
}

/* OPEN_PARENTHESIS FA */

char * machine_OPEN_PARENTHESIS_FA(char *p){
	switch (*p){
		case '(': 
			return p;
		default:
			return -1;
	}	
}

/* CLOSE_PARENTHESIS FA */

char * machine_CLOSE_PARENTHESIS_FA(char *p){
	switch (*p){
		case ')': 
			return p;
		default:
			return -1;
	}	
}


/* SEMICOLON FA */

char * machine_SEMICOLON_FA(char *p){
	switch (*p){
		case ';': 
			return p;
		default:
			return -1;
	}	
}

/* RETURN FA */

char * machine_RETURN_KEYWORD_FA(char *p) {
	switch (*p){
		case 'r': 
			return machine_step2_RETURN_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step2_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 'e': 
			return machine_step3_RETURN_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step3_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 't': 
			return machine_step4_RETURN_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step4_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 'u': 
			return machine_step5_RETURN_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step5_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 'r': 
			return machine_step6_RETURN_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step6_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case 'n': 
			return machine_step7_RETURN_KEYWORD_FA(p + 1);
		default:
			return -1;
	}	
}

char * machine_step7_RETURN_KEYWORD_FA(char *p){
	switch (*p){
		case ' ': 
			return p - 1;
		case ';': 
			return p - 1;
		default:
			return -1;
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


