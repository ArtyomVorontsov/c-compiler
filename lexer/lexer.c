#include <stdio.h>
#include <string.h>
#include <ctype.h>

union Value {
	int number;
	char string[10];
};

struct Token {
	char *type;
	union Value value;
};

struct INT_FA {
	char i, n, t, space;
};

struct INT_FA INT_FA_IMP = {
	.i = 'n',
	.n = 't',
	.t = ' ',
	.space = '\0'
};

char * machine_INT_FA(char *p);
char * machine_IDENTIFIER_FA(char *p, int i);
char * machine_step2_IDENTIFIER_FA(char *p, int i);
int get_val_by_key_INT_FA(char c);
int lex(char *inp);

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
		//printf("i: %d\n", i);
		printf("c: %c\n", *token_end);

		/* INT */
		tmp = token_end;
		token_end = machine_INT_FA(token_end);
		token_start = tmp;
		if(token_end != -1) { 
			tokens[i].type = "INT";
			i++;
			continue;
		} else {
			token_end = tmp;
		}
		
		/* IDENTIFIER */

		tmp = token_end;
		token_end = machine_IDENTIFIER_FA(token_end, 0);
		token_start = tmp;
		if(token_end != -1) { 
			tokens[i].type = "IDENTIFIER";
			i++;
			continue;
		} else {
			token_end = tmp;
		}

		/* TODO: SKIP WHITESPACE */



		printf("No lex handler\n");
		break;	
	}

	printf("%d Results:\n", i);
	while(i--)
		printf("Type: %s\n", tokens[i].type);
}
/* IDENTIFIER FA */
char * machine_IDENTIFIER_FA(char *p, int i) {
	printf("p: %c | i: %d\n", *p, i);
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
			return machine_step2_IDENTIFIER_FA(p + 1, i + 1);
		default:
			return -1;
	}
}

char * machine_step2_IDENTIFIER_FA(char *p, int i) {
	printf("p: %c | i: %d\n", *p, i);
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
			return machine_step2_IDENTIFIER_FA(p + 1, i + 1);
		case ' ':
			return p + i;
		default:
			return -1;
	}
}

/* INT FA */
char * machine_INT_FA(char *p){
	char c;
	int i = 0;
	while(1){
		c = get_val_by_key_INT_FA(*(p + i));
		if(c == -1) return -1;
		if(c == '\0' && i >= 3) return p + i + 1;
		i++;
	}
}

int get_val_by_key_INT_FA(char c) {
	switch(c){
		case 'i':
			return INT_FA_IMP.i;
		case 'n':
			return INT_FA_IMP.n;
		case 't':
			return INT_FA_IMP.t;
		case ' ':
			return INT_FA_IMP.space;
		default:
			return -1;
	}
}


