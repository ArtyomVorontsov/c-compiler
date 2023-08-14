#include "./parser.h"

struct TreeNode * nodes_stack[100];
struct TreeNode ** pns = nodes_stack;
struct Token * pt = tokens;
int error_tree_depth = -1;
struct Token * error_token;
bool expression_error = false;


struct TreeNode * parse(){
	bool match;
	*pns = nodes_stack[0];

	match = Program();
	if(!match) {
		printf(
			"ERROR: Parsing is failed at line %d:%d. After '%s'\n", 
			error_token->position.line, 
			error_token->position.line_char, 
			error_token->value.string
		), 
		exit(1);
	}

	struct TreeNode * root_node = nodes_stack[0];

	return root_node;
}


bool Program(){
	print_if_explicit("Program\n");
	struct TreeNode *node = create_node("PROGRAM", "PROGRAM");
	bool match = false;
	struct Token * next = pt;
	struct TreeNode * root_node = nodes_stack[0];

	set_node_as_deepest(node);

	while(Function()){
		next = pt;
		match = true;
	}

	// Recover after unsuccessfull statement match
	pt = next;
	node->children_amount--;

	remove_node_from_deepest();	

	if(match) return true;
	set_error();

	return false;
}

bool Function(){
	print_if_explicit("Function\n");
	struct TreeNode *node = create_node("FUNCTION", "FUCTION");
	bool match;

 	set_as_child(node);
	set_node_as_deepest(node);

	match = Type() && 
		Identifier() && 
		Match("OPEN_PARENTHESIS") && 
		Function_Params() &&
		Match("CLOSE_PARENTHESIS") && 
		Match("OPEN_BRACE") && 
		Function_Body() && 
		Match("CLOSE_BRACE");

	remove_node_from_deepest();

	if(match) return true;
	set_error();

	return false;
}

bool Function_Argument(){
	print_if_explicit("Function_Argument\n");
	bool match = false;
	struct TreeNode * node = create_node("FUNCTION_ARGUMENT", "FUNCTION_ARGUMENT");
	struct TreeNode * exp;
	struct Token * next = pt;

 	set_as_child(node);
	set_node_as_deepest(node);

	if(pt = next, remove_node_as_child(node), exp = Expression(), expression_error == 0){ 
		match = true;
 		set_as_child(exp);
	}

	remove_node_from_deepest();	

	if(match) return true;

	set_error();
	return false;
}

bool Function_Arguments(){
	print_if_explicit("Function_Arguments\n");
	bool match = false;
	int i = 0;
	struct Token * next = pt;
	struct TreeNode *node = create_node("FUNCTION_ARGUMENTS", "FUCTION_ARGUMENTS");

	if(cmpstr(pt->type, "CLOSE_PARENTHESIS")) {
		return true;
	}

 	set_as_child(node);
	set_node_as_deepest(node);

	while(Function_Argument()){
		next = pt;
		match = true;

		if(_Match("COMA_OP", false) == 0){
			break;
		}

		if(i++ > 1000){
			printf("Error: argument amount exceeded 999!\n");
			set_error();
			return false;
		}
	}

	remove_node_from_deepest();	

	return true;
}

struct TreeNode * Function_Call(){
	print_if_explicit("Function_Call\n");
	struct TreeNode *node = create_node("FUNCTION_CALL", "FUCTION_CALL");
	bool match;

	set_node_as_deepest(node);

	match = Identifier() && 
		_Match("OPEN_PARENTHESIS", false) && 
		Function_Arguments() &&
		_Match("CLOSE_PARENTHESIS", false);

	remove_node_from_deepest();

	if(match == false){
		set_error();
		//expression_error = true;
		return false;
	}

	return node;
}

bool Function_Param(){
	print_if_explicit("Function_Param\n");
	bool match = false;
	struct TreeNode * node = create_node("FUNCTION_PARAM", "FUNCTION_PARAM");
	struct TreeNode * exp;
	struct Token * next = pt;


 	set_as_child(node);
	set_node_as_deepest(node);

	if(pt = next, remove_node_as_child(node), Match("INT_KEYWORD") && Identifier()){ 
		match = true;
	}

	remove_node_from_deepest();	

	if(match) return true;

	set_error();
	return false;
}

bool Function_Params(){
	print_if_explicit("Function_Params\n");
	bool match = false;
	int i = 0;
	struct Token * next = pt;
	struct TreeNode *node = create_node("FUNCTION_PARAMS", "FUCTION_PARAMS");

	if(cmpstr(pt->type, "CLOSE_PARENTHESIS")) {
		return true;
	}

 	set_as_child(node);
	set_node_as_deepest(node);

	while(Function_Param()){
		next = pt;
		match = true;

		if(_Match("COMA_OP", false) == 0){
			break;
		}

		if(i++ > 1000){
			printf("Error: params amount exceeded 999!\n");
			set_error();
			return false;
		}
	}

	remove_node_from_deepest();	

	return true;

}

bool Block_Item(){
	print_if_explicit("Block_Item\n");
	bool match = false;
	struct TreeNode * node = create_node("BLOCK_ITEM", "BLOCK_ITEM");
	struct TreeNode * exp;
	struct Token * next = pt;

	// Skip semicolon nodes
	if(_Match("SEMICOLON", false)){
		match = true;
		return match;
	}

 	set_as_child(node);
	set_node_as_deepest(node);

	if(pt = next, remove_node_as_child(node), Statement()){ 
		match = true;
	}
	else if (
		pt = next, remove_node_as_child(node), Declaration() && _Match("SEMICOLON", false)
	){
		match = true;
	} 

	remove_node_from_deepest();	

	if(match) return true;

	set_error();
	return false;
}

bool Function_Body(){
	print_if_explicit("Function_Body\n");
	bool match = false;
	int i = 0;
	struct Token * next = pt;
	struct TreeNode * node = create_node("FUNCTION_BODY", "FUNCTION_BODY");

 	set_as_child(node);
	set_node_as_deepest(node);

	while(Block_Item()){
		next = pt;
		match = true;

		if(i++ > 1000){
			printf("Error: expressions amount exceeded 999!\n");
			set_error();
			return false;
		}
	}

	// Recover after unsuccessfull statement match
	pt = next;
	node->children_amount--;

	remove_node_from_deepest();	

	return true;
}

bool Statement(){
	print_if_explicit("Statement\n");
	bool match = false;
	struct TreeNode * node = create_node("STATEMENT", "STATEMENT");
	struct TreeNode * exp;
	struct TreeNode * loop;
	struct Token * next = pt;

 	set_as_child(node);
	set_node_as_deepest(node);

	if(
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		Match("RETURN_KEYWORD") && 
		(exp = Expression(), set_node_as_child(node, exp), expression_error == false) && 
		_Match("SEMICOLON", false)
	){
		match = true;
	} 
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		(exp = Expression(), set_node_as_child(node, exp), expression_error == false) && 
		_Match("SEMICOLON", false)
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		(exp = Conditional(), set_node_as_child(node, exp), expression_error == false)
	){

		match = true;
	}
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		_Match("OPEN_BRACE", false) &&
		(exp = Compound(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("CLOSE_BRACE", false)
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		(exp = Expression_Option(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("SEMICOLON", false)
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		Loop()
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		Match("BREAK_KEYWORD") &&
		_Match("SEMICOLON", false)
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_node_as_child(node), 
		expression_error = false, 1) &&
		Match("CONTINUE_KEYWORD") &&
		_Match("SEMICOLON", false)
	){
		match = true;
	}
	else {
		pt = next;
		remove_node_as_child(node);
		expression_error = false;
	}
	remove_node_from_deepest();	

	if(match) return true;

	set_error();
	return false;
}

bool Loop(){
	print_if_explicit("Statement\n");
	bool match = false;
	struct TreeNode * node = create_node("LOOP", "LOOP");
	struct TreeNode * exp;
	struct TreeNode * loop;
	struct Token * next = pt;

 	set_as_child(node);
	set_node_as_deepest(node);

	if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		Match("FOR_KEYWORD") &&
		_Match("OPEN_PARENTHESIS", false) &&
		(exp = Expression_Option(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("SEMICOLON", false) &&
		(exp = Expression_Option(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("SEMICOLON", false) &&
		(exp = Expression_Option(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("CLOSE_PARENTHESIS", false) &&
		Statement()
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		Match("FOR_KEYWORD") &&
		_Match("OPEN_PARENTHESIS", false) &&
		Declaration() &&
		_Match("SEMICOLON", false) &&
		(exp = Expression_Option(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("SEMICOLON", false) &&
		(exp = Expression_Option(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("CLOSE_PARENTHESIS", false) &&
		Statement()
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		Match("WHILE_KEYWORD") &&
		_Match("OPEN_PARENTHESIS", false) &&
		(exp = Expression(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("CLOSE_PARENTHESIS", false) &&
		Statement()
	){
		match = true;
	}
	else if (
		(pt = next, 
		remove_child_nodes(node), 
		expression_error = false, 1) &&
		Match("DO_KEYWORD") &&
		Statement() &&
		Match("WHILE_KEYWORD") &&
		_Match("OPEN_PARENTHESIS", false) &&
		(exp = Expression(), set_node_as_child(node, exp), expression_error == false) &&
		_Match("CLOSE_PARENTHESIS", false) &&
		_Match("SEMICOLON", false)
	){
		match = true;
	}


	remove_node_from_deepest();	

	if(match) return true;

	set_error();
	return false;
}

struct TreeNode * Compound(){
	print_if_explicit("Compound\n");
	bool match = false;
	int i = 0;
	struct Token * next = pt;
	struct TreeNode * node = create_node("COMPOUND", "COMPOUND");

	set_node_as_deepest(node);

	while(Block_Item()){
		next = pt;
		match = true;

		if(i++ > 1000){
			printf("Error: expressions amount exceeded 999!\n");
			set_error();
			return false;
		}
	}

	// Recover after unsuccessfull statement match
	pt = next;
	node->children_amount--;

	remove_node_from_deepest();

	return node;
}

struct TreeNode * Conditional(){
	print_if_explicit("Conditional\n");
	struct TreeNode * node = create_node("CONDITIONAL_STATEMENT", "CONDITIONAL_STATEMENT");
	struct TreeNode * condition_node = create_node("CONDITION", "CONDITION");
	struct TreeNode * true_condition_node = create_node("TRUE_CONDITION", "TRUE_CONDITION");
	struct TreeNode * false_condition_node = create_node("FALSE_CONDITION", "FALSE_CONDITION");
	struct TreeNode * exp;
	struct Token * next = pt;

	set_node_as_deepest(node);
	set_node_as_child(node, condition_node);
	set_node_as_deepest(true_condition_node);
	if (
		(pt = next, 
		remove_node_as_child(condition_node), 
		remove_node_as_child(true_condition_node), 
		expression_error = false, 1) &&
		_Match("IF_KEYWORD", false) && 
		_Match("OPEN_PARENTHESIS", false) &&
		(exp = Expression(), set_node_as_child(condition_node, exp), expression_error == false) && 
		_Match("CLOSE_PARENTHESIS", false) &&
		Statement() &&
		_Match("ELSE_KEYWORD", false)
	){
		set_node_as_child(node, true_condition_node);
		set_node_as_child(node, false_condition_node);

		set_node_as_deepest(false_condition_node);
		Statement();
		remove_node_from_deepest();	

		next = pt;
	}
	else if (
		(pt = next, 
		remove_node_as_child(condition_node), 
		remove_node_as_child(true_condition_node), 
		expression_error = false, 1) &&
		_Match("IF_KEYWORD", false) && 
		_Match("OPEN_PARENTHESIS", false) &&
		(exp = Expression(), set_node_as_child(condition_node, exp), expression_error == false) && 
		_Match("CLOSE_PARENTHESIS", false) &&
		Statement()
	){
		set_node_as_child(node, true_condition_node);
		next = pt;
	}
	else {
		set_error();
		expression_error = true;
	}

	remove_node_from_deepest();	
	remove_node_from_deepest();

	return node;
}


struct TreeNode * Expression_Option(){
	print_if_explicit("Expression_Option\n");
	struct TreeNode * node = create_node("EXPRESSION_OPTION", "EXPRESSION_OPTION");
	struct TreeNode * non_zero_constant_node;
	struct TreeNode * exp;
	struct Token * next = pt;

	set_node_as_deepest(node);
	
	if ((pt = next, remove_as_child(), pt->type) && cmpstr(pt->type, "SEMICOLON")){
		non_zero_constant_node = create_node("NON_ZERO_CONSTANT", "NON_ZERO_CONSTANT");
		set_as_child(non_zero_constant_node);
		next = pt;
	} 
	else if ((pt = next, remove_as_child(), pt->type) && cmpstr(pt->type, "CLOSE_PARENTHESIS")){
		non_zero_constant_node = create_node("NON_ZERO_CONSTANT", "NON_ZERO_CONSTANT");
		set_as_child(non_zero_constant_node);
		next = pt;
	} 
	else if ((pt = next, remove_as_child(), pt->type) && (exp = Expression())){
		set_as_child(exp);
		next = pt;
	} 
	else {
		set_error();
		expression_error = true;
	}

	remove_node_from_deepest();	

	return node;
}

struct TreeNode * Expression(){
	print_if_explicit("Expression\n");
	struct TreeNode * node = create_node("EXPRESSION", "EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;
	struct Token * next = pt;

	set_node_as_deepest(node);
	
	if ((pt = next, remove_as_child(), pt->type) && Assignement_Expression()){
		next = pt;
	} 
	else if(pt = next, remove_as_child(), 1){
		term_node = Conditional_Expression();
		set_node_as_child(node, term_node); 

		while(pt->type && compare_operator_by_precedence_level(0)){
			bin_op_node = Binary_OP();
			next_term_node = Conditional_Expression();

			term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
			remove_node_as_child(node);
			set_node_as_child(node, term_node); 
		}
	}
	
	remove_node_from_deepest();	
	return node;
}

struct TreeNode * Conditional_Expression(){
	print_if_explicit("Conditional_Expression\n");

	struct TreeNode * node = create_node("CONDITIONAL_EXPRESSION", "CONDITIONAL_EXPRESSION");
	struct TreeNode * condition_node = create_node("CONDITION", "CONDITION");
	struct TreeNode * true_condition_node = create_node("TRUE_CONDITION", "TRUE_CONDITION");
	struct TreeNode * false_condition_node = create_node("FALSE_CONDITION", "FALSE_CONDITION");
	struct TreeNode * exp;

	set_node_as_deepest(node);

	exp = Logical_Or_Expression();
	set_node_as_child(node, exp); 

	if(pt->type && compare_operator_by_precedence_level(0)){
		remove_as_child();

		set_node_as_child(condition_node, exp); 

		set_node_as_child(node, condition_node); 
		set_node_as_child(node, true_condition_node); 
		set_node_as_child(node, false_condition_node); 

		if(_Match("QUESTION_MARK_OP", false)) {
			exp = Expression();
			set_node_as_child(true_condition_node, exp); 

			if(_Match("COLON_OP", false)) {
				exp = Conditional_Expression();
				set_node_as_child(false_condition_node, exp);
			} else {
				expression_error = true;
				set_error();
			}
		} else {
			expression_error = true;
			set_error();
		}
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Logical_Or_Expression(){
	print_if_explicit("Logical_Or_Expression\n");
	struct TreeNode * node = create_node("LOGICAL_OR_EXPRESSION", "LOGICAL_OR_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Logical_And_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(1)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Logical_And_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Logical_And_Expression(){
	print_if_explicit("Logical_And_Expression\n");
	struct TreeNode * node = create_node("LOGICAL_AND_EXPRESSION", "LOGICAL_AND_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Equality_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(2)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Equality_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Equality_Expression(){
	print_if_explicit("Equality_Expression\n");
	struct TreeNode * node = create_node("EQUALITY_EXPRESSION", "EQUALITY_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Relational_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(3)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Relational_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Relational_Expression(){
	print_if_explicit("Relational_Expression\n");
	struct TreeNode * node = create_node("RELATIONAL_EXPRESSION", "RELATIONAL_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Addictive_Expression();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(4)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Addictive_Expression();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Addictive_Expression(){
	print_if_explicit("Addictive_Expression\n");
	struct TreeNode * node = create_node("ADDICTIVE_EXPRESSION", "ADDICTIVE_EXPRESSION");
	struct TreeNode * term_node;
	struct TreeNode * next_term_node;
	struct TreeNode * bin_op_node;

	set_node_as_deepest(node);
	term_node = Term();
	set_node_as_child(node, term_node); 

	while(pt->type && compare_operator_by_precedence_level(5)){
		bin_op_node = Binary_OP(pt);
		next_term_node = Term();

		term_node = Binary_Statement(bin_op_node, term_node, next_term_node);
		remove_node_as_child(node);
		set_node_as_child(node, term_node); 
	}

	remove_node_from_deepest();	
	
	return node;
}

struct TreeNode * Term(){
	print_if_explicit("Term\n");
	struct TreeNode * node = create_node("TERM", "TERM");
	struct TreeNode * factor_node;
	struct TreeNode * next_factor_node;
	struct TreeNode * op_node;

	set_node_as_deepest(node);

	factor_node = Factor();
	set_node_as_child(node, factor_node);


	while(pt->type && compare_operator_by_precedence_level(6)){
		op_node = Binary_OP(pt);
		next_factor_node = Factor(pt + 1);

		factor_node = Binary_Statement(op_node, factor_node, next_factor_node);
		remove_node_as_child(node);
		set_node_as_child(node, factor_node); 
	}

	remove_node_from_deepest();	

	return node;
}


struct TreeNode * Factor(){
	print_if_explicit("Factor\n");
	struct TreeNode * node = create_node("FACTOR", "FACTOR");
	struct TreeNode * int_node;
	struct TreeNode * var_node;
	struct TreeNode * semicolon_node;
	struct TreeNode * assignement_node;
	struct TreeNode * exp_node;
	struct TreeNode * op_node;
	struct TreeNode * un_op_node;
	struct Token * next = pt;

	set_node_as_deepest(node);

	if (pt->type && (cmpstr(pt->type, "OPEN_PARENTHESIS"))){
		print_if_explicit("Expression in parenthesis\n");
		pt++;
		exp_node = Expression();

		if (pt->type && (cmpstr(pt->type, "CLOSE_PARENTHESIS") == false)) {
			expression_error = true;
			set_error();
		}
		pt++;

		set_node_as_child(node, exp_node);

	} 
	else if (
		pt->type && 
		(
		 cmpstr(pt->type, "ADDITION_OP") || 
		 cmpstr(pt->type, "NEGATION_OP") ||
		 cmpstr(pt->type, "LOGICAL_NEGATION_OP") ||
		 cmpstr(pt->type, "BITWISE_COMPLEMENT_OP")
		)
	){
		un_op_node = Unary_Statement();
		set_node_as_child(node, un_op_node);
		next = pt;
	}
	else if ((pt = next, remove_as_child(), pt->type) && (cmpstr(pt->type, "INT"))){
		int_node = Int();
		set_node_as_child(node, int_node);
		next = pt;
	}
	else if ((pt = next, remove_as_child(), pt->type) && _Match("SEMICOLON", false)){
		next = pt;
	} 
	else if ((pt = next, remove_as_child(), pt->type) && (exp_node = Function_Call())){
		set_node_as_child(node, exp_node);
		next = pt;
	}
	else if ((pt = next, remove_as_child(), pt->type) && (cmpstr(pt->type, "IDENTIFIER"))){
		var_node = Var();
		set_node_as_child(node, var_node);
		next = pt;
	}
	else {
		pt = next;
	       	remove_as_child();
		expression_error = true;
		set_error();
	}

	remove_node_from_deepest();

	return node;
}

bool Declaration(){
	print_if_explicit("Declaration\n");
	struct TreeNode * node = create_node("DECLARATION", "DECLARATION");
	struct TreeNode * exp;
	struct TreeNode * assignement_op_node;
	struct TreeNode * assignement_node;
	struct TreeNode * int_keyword;
	struct TreeNode * var_node;
	bool match = true;

	set_as_child(node); 
	set_node_as_deepest(node);


	if(cmpstr(pt->type, "INT_KEYWORD")){
		int_keyword = create_node(pt->type, pt->value.string);
		pt++;
		set_node_as_child(node, int_keyword); 


		if(cmpstr((pt + 1)->type, "SEMICOLON")){
			var_node = Var();
			set_node_as_child(node, var_node); 
		} 
		else if (cmpstr((pt + 1)->type, "ASSIGN")) {
			var_node = Var();
			assignement_op_node = Assignement_OP();
			set_node_as_child(node, assignement_op_node); 
			
			exp = Expression();
			set_node_as_child(assignement_op_node, var_node); 
			set_node_as_child(assignement_op_node, exp); 
			
		}
		else if (cmpstr(pt->type, "COMA")) {
			printf("COMA IS NOT SUPPORTED!\n");
			match = false;
			set_error();
		}
	} else {
		match = false;
		set_error();
	}

	remove_node_from_deepest();

	return match;
} 

bool Assignement_Expression(){
	print_if_explicit("Assignement_Expression\n");
	struct TreeNode * node = create_node("ASSIGNEMENT_EXPRESSION", "ASSIGNEMENT_EXPRESSION");
	int success = false;
	struct Token * next = pt;
	struct TreeNode * exp;
	struct TreeNode * var_node;
	struct TreeNode * assignement_op;

	set_as_child(node);
	set_node_as_deepest(node);
	if(cmpstr(pt->type, "IDENTIFIER") && cmpstr((pt + 1)->type, "ASSIGN")){
		var_node = Var();
		assignement_op = Assignement_OP();
		exp = Expression();

		set_as_child(assignement_op);
		set_node_as_child(assignement_op, var_node);
		set_node_as_child(assignement_op, exp);
		success = true;
	}

	remove_node_from_deepest();

	return success; 
}

struct TreeNode * Binary_Statement(struct TreeNode * op_node, struct TreeNode * term_node, struct TreeNode * next_term_node){
	print_if_explicit("Binary_Statement\n");
	struct TreeNode * binary_statement_node = create_node("BINARY_STATEMENT", "BINARY_STATEMENT");

	set_node_as_child(binary_statement_node, term_node);
	set_node_as_child(binary_statement_node, next_term_node);
	set_node_as_child(op_node->children[0], binary_statement_node);
	
	return op_node;
}

struct TreeNode * Unary_Statement(){
	print_if_explicit("Unary_Statement\n");
	struct TreeNode * fact_node;
	struct TreeNode * op_node;
	struct TreeNode * un_op_node;

	un_op_node = create_node("UNARY_STATEMENT", "UNARY_STATEMENT");
	op_node = Unary_OP();
	fact_node = Factor();

	set_node_as_child(un_op_node, op_node);
	set_node_as_child(un_op_node, fact_node);
	
	return un_op_node;
}

struct TreeNode * Var(){
	print_if_explicit("Var\n");
	struct TreeNode * node = create_node("VAR", "VAR");
	struct TreeNode * identifier_node;

	identifier_node = Identifier_OP();
	set_node_as_child(node, identifier_node);
	
	return node;
}

struct TreeNode * Int(){
	print_if_explicit("Int\n");
	struct TreeNode * int_node;

	int_node = create_node(pt->type, pt->value.string);
	pt++;
	
	return int_node;
}

struct TreeNode * Identifier_OP(){
	print_if_explicit("Identifier_OP\n");
	struct TreeNode * identifier_op_node;

	identifier_op_node = create_node(pt->type, pt->value.string);
	pt++;
	
	return identifier_op_node;
}

struct TreeNode * Unary_OP(){
	print_if_explicit("Unary_OP\n");
	struct TreeNode *node = create_node("UNARY_OP", "UNARY_OP");
	set_node_as_deepest(node);

	struct TreeNode *op_node = create_node(pt->type, pt->value.string);
	pt++;
	set_node_as_deepest(node);

	set_node_as_child(node, op_node);

	remove_node_from_deepest();
	remove_node_from_deepest();

	return node;
}

struct TreeNode * Binary_OP(){
	print_if_explicit("Binary_OP\n");
	struct TreeNode *node = create_node("BINARY_OP", "BINARY_OP");
	set_node_as_deepest(node);

	struct TreeNode *op_node = create_node(pt->type, pt->value.string);
	pt++;
	set_node_as_deepest(node);

	set_node_as_child(node, op_node);

	remove_node_from_deepest();
	remove_node_from_deepest();

	return node;
}

struct TreeNode * Assignement_OP(){
	print_if_explicit("Assignement_OP\n");
	struct TreeNode *node;

	node = create_node(pt->type, pt->value.string);
	pt++;

	return node;
}


bool Type(){
	print_if_explicit("Type\n");
	bool match;

	struct TreeNode *node = create_node("TYPE", "TYPE");
 	set_as_child(node);
	set_node_as_deepest(node);

	match = Match("INT_KEYWORD");

	remove_node_from_deepest();

	if(match) return true;
	set_error();

	return false;
}


bool Identifier(){
	print_if_explicit("Identifier\n");
	bool match;

	match = Match("IDENTIFIER");

	if(match) return true;
	set_error();

	return false;
}

bool Match(char *type){
	print_if_explicit("Match\n");
	return _Match(type, true);
}

bool _Match(char *type, bool set_child){
	print_if_explicit("_Match\n");
	if(SILENT_ARG == false)
		printf("\tMatch: SOURCE_CODE_POINTER: '%s'\n\tPARSER_NODE: '%s'\n", pt->type, type);
	if(pt->type && cmpstr(pt->type, type)){
		struct TreeNode * node = create_node(pt->type, pt->value.string);
 		if(set_child) set_as_child(node);
		pt++;
		return true;
	}

	set_error();

	return false;
}

bool compare_operator_by_precedence_level(int level){
	switch(level){
		case 0:
			return cmpstr(pt->type, "QUESTION_MARK_OP") ||

			cmpstr(pt->type, "OR_OP") ||

			cmpstr(pt->type, "AND_OP") ||

			cmpstr(pt->type, "EQUAL_OP") ||
			cmpstr(pt->type, "NOT_EQUAL_OP") ||

			cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 1:
			return cmpstr(pt->type, "OR_OP") ||

			cmpstr(pt->type, "AND_OP") ||

			cmpstr(pt->type, "EQUAL_OP") ||
			cmpstr(pt->type, "NOT_EQUAL_OP") ||

			cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 2:
			return cmpstr(pt->type, "AND_OP") ||

			cmpstr(pt->type, "EQUAL_OP") ||
			cmpstr(pt->type, "NOT_EQUAL_OP") ||

			cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 3:
			return cmpstr(pt->type, "EQUAL_OP") ||
			cmpstr(pt->type, "NOT_EQUAL_OP") ||

			cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 4:
			return cmpstr(pt->type, "LESS_THAN_OP") ||
			cmpstr(pt->type, "LESS_THAN_OR_EQUAL_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OP") ||
			cmpstr(pt->type, "GREATER_THAN_OR_EQUAL_OP") ||

			cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 5:
			return cmpstr(pt->type, "ADDITION_OP") ||
			cmpstr(pt->type, "NEGATION_OP") ||

			cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		case 6:
			return cmpstr(pt->type, "MULTIPLICATION_OP") ||
			cmpstr(pt->type, "DIVISION_OP");

		default:
			return true;
	}

}

void set_as_child(struct TreeNode * current_node){
	// Assign as a child to previous node
	struct TreeNode * parent_node = *(pns - 1);
	
	(*parent_node).children[(*parent_node).children_amount] = current_node;
	(*parent_node).children_amount++;
}

void set_node_as_child(struct TreeNode * parent_node, struct TreeNode * current_node){
	(*parent_node).children[(*parent_node).children_amount] = current_node;
	(*parent_node).children_amount++;
}

void remove_as_child(){
	struct TreeNode * parent_node = *(pns - 1);
	// Remove as a child from previous node
	(*parent_node).children_amount = 0;
}

void remove_child_nodes(struct TreeNode * parent_node){
	(*parent_node).children_amount = 0;
}

void remove_node_as_child(struct TreeNode * parent_node){
	if((*parent_node).children_amount > 0)
		(*parent_node).children_amount--;
}

void set_node_as_deepest(struct TreeNode * current_node){
	// Set current node as deepest 
	*pns = current_node;
	pns++;
}

void remove_node_from_deepest(){
	pns--;
}

struct TreeNode * create_node(char *type, char *value){
	struct TreeNode *node;
	node = (struct TreeNode *) malloc(sizeof(struct TreeNode));
	node->type = type;
	node->value = value;
	node->children_amount = 0;
	return node;
}

void set_error(){
	int curr_error_token_depth = pns - nodes_stack;

	if(curr_error_token_depth > error_tree_depth) {
		error_tree_depth = curr_error_token_depth;
		error_token = pt - 1;
	}
}

