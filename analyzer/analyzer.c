#include "analyzer.h"

void analyze(struct TreeNode * node){
	traverse_tree(node);
}

void traverse_tree(struct TreeNode * node){
	printf("TYPE: %s\n", node->type);
	int i;

	for(i = 0; i < node->children_amount; i++){
		traverse_tree(node->children[i]);
	}
}

