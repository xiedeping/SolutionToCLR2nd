#include "tree.h"
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

int main()
{
	Node *T = NIL;
	Interval A[] = {
		{0, 3},
		{5, 8},
		{6, 10},
		{8, 9},
		{15, 23},
		{16, 21},
		{17, 19},
		{19, 20},
		{25, 30},
		{26, 26},
	};
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

	printf("[interval_search]\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(interval_search(T, A[i]));
	}
	printf("\n");

	printf("[min_interval_search]\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(min_interval_search(T, A[i]));
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

	printf("[delete]\n");
	for (i=0; i<sizeof(A)/sizeof(A[0]); i++) {
		print_node(T);
		tree_delete(T, tree_search(T, A[i]));
	}
	print_node(T);
	printf("\n");

	return 0;
}