#include "tree.h"
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

int main()
{
	Node *T = malloc_node();
	int A[] = { 3,4,6,2,8,34,16,84,12,93,28,38};
		
	int i = 0;
	printf("[insert]\n");
	for (i = 0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(tree_insert(T, A[i]));
	}
	printf("\n");

	printf("[min]\n");
	print_node(tree_min(T));
	printf("\n");

	printf("[max]\n");
	print_node(tree_max(T));
	printf("\n");

	printf("[delete]\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(T);
		tree_delete(T, tree_search(T, A[i]));
	}
	print_node(T);
	printf("\n");

	return 0;
}