#include <stdio.h>

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
} INT_FA_IMP = {
	.i = 'n',
	.n = 't',
	.t = ' ',
	.space = '\0'
};
int INT_FA_LENGTH = 3;

char * machine_INT_FA(char *p);
int getval(struct INT_FA *ep, int n);
int get_val_by_key_INT_FA(char c);
int get_val_by_position_INT_FA(int n);
int lex(char *inp);

int main(){
	char inp[1000] = { '\0' };

	printf("Type your program\n");
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
		tmp = token_end;
		token_end = machine_INT_FA(token_end);
		token_start = tmp;

		
		printf("token_end: %d\n", token_end);
		printf("token_start: %d\n", token_start);

		if(token_end != -1) { 
			tokens[i].type = "INT";
			i++;
			continue;
		}


		printf("No lex handler\n");
		break;	
	}

	printf("%d Results:\n", i);
	while(i--)
		printf("Type: %s\n", tokens[i].type);
}

/* INT FA */
char * machine_INT_FA(char *p){
	char c;
	int i = 0;
	while(1){
		c = get_val_by_key_INT_FA(*(p + i));
		printf("c: %d | d: %d\n", c, *(p + i));
		if(c == -1) return -1;
		if(c == '\0') return p + i + 1;
		i++;
	}
}

int get_val_by_position_INT_FA(int n) {
	switch(n) {
		case 0: 
			return INT_FA_IMP.i;
		case 1: 
			return INT_FA_IMP.n;
		case 2:
			return INT_FA_IMP.t;
		case 3:
			return INT_FA_IMP.space;
		default:
			return -1;
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


