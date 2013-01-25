#include "tree.h"
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

int main()
{
	Node *T = NIL;
	int A[] = {3,13,23,20,12,6,10,5,16,7,18,15};
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

	printf("[tree]\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(tree_search(T, A[i]));
	}
	printf("\n");

	printf("[predecessor]\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(tree_predecessor(tree_search(T, A[i])));
	}
	printf("\n");

	printf("[sucessor]\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(tree_successor(tree_search(T, A[i])));
	}
	printf("\n");

	printf("[inorder]\n");
	inorder_tree_walk(T);
	printf("\n\n");

	printf("[os_select]\n");
	for (i=1; i<=sizeof(A)/sizeof(A[0]); i++) {
		print_node(os_select(T, i));
	}
	printf("\n\n");

	printf("os_rank\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		printf("%d ", os_rank(T, tree_search(T, A[i])));
	}
	printf("\n\n");

	printf("os_key_rank\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		printf("%d ", os_key_rank(T, A[i]));
	}
	printf("\n\n");

	printf("[delete]\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(T);
		tree_delete(T, tree_search(T, A[i]));
	}
	print_node(T);
	printf("\n");

	return 0;
}