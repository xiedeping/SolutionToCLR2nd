#include "tree.h"
#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

Node NIL_NODE = {0, {}, {}, false};
Node *NIL = &NIL_NODE;

#define KEY_NUM(n)    (n->key_num)
#define KEY(n)    (n->key)
#define PARENT(n) (n->parent)
#define CHILD(n)  (n->child)
#define LEAF(n)   (n->leaf)

#define IS_ROOT(n)   (NIL == PARENT(n))
#define IS_LEAF(n) (n->leaf)
#define IS_FULL(n) (KEY_NUM(n) == MAX_CHILD)

Node *malloc_node()
{
	Node *n = (Node *)malloc(sizeof(Node));
	assert(n);
	KEY_NUM(n) = 0;
	LEAF(n) = true;

	return n;
}

void free_node(Node *n)
{
	free(n);
}

void tree_split_child(Node *x, int i, Node *y)
{
	// 注意：y是满节点，x不是满节点，下标从1开始

	// 创建一个新节点，作为y的右兄弟
	Node *z = malloc_node();
	LEAF(z) = LEAF(y);
	KEY_NUM(z) = MIN_CHILD;

	// 拷贝y节点的右半部分key（从下标t+1开始)到z节点
	for (int j=1;j<=MIN_CHILD;j++) {
		KEY(z)[j] = KEY(y)[j+MIN_CHILD+1];
	}
	KEY_NUM(y) = KEY_NUM(z) = MIN_CHILD;

	// 如果y不是叶节点，则拷贝y节点的右半部分child（从下标t+1开始）到z节点
	if (!IS_LEAF(y)) {
		for (int j=1;j<=MIN_CHILD;j++) {
			CHILD(z)[j] = CHILD(z)[j+MIN_CHILD+1];
		}
	}

	// x节点，从下标i开始的key，全部右移一格
	for (int j=KEY_NUM(x);j>=i;j--) {
		KEY(x)[j+1] = KEY(x)[j];
	}
	// 将y节点下标为t的key，提升到x节点的i位置
	KEY(x)[i] = KEY(y)[MIN_CHILD+1];

	// x节点，从下标i+1开始的child，全部右移一格
	for (int j=KEY_NUM(x)+1;j>i;j--) {
		CHILD(x)[j+1] = CHILD(x)[j];
	}
	// x节点下标为i+1的child，指向z节点
	CHILD(x)[i+1] = z;
	
	// z节点的key数量+1
	KEY_NUM(x)++;
}

void tree_insert_nofull(Node *x, int key)
{
	// 注意: x节点肯定未满

	int i=KEY_NUM(x);
	// 如果x是叶节点，则直接找到位置插入
	if (IS_LEAF(x)) {
		while (i>=1 && key<KEY(x)[i]) {
			KEY(x)[i+1] = KEY(x)[i];
			i--;
		}
		KEY(x)[i+1] = key;
		KEY_NUM(x)++;
		return;
	}

	// x不是叶节点，找到待插入的节点y
	while (i>=1 && key<KEY(x)[i]) {
		i--;
	}
	i++;
	Node *y = CHILD(x)[i];
	
	// 检查节点y是不是已满，如果是，则先分裂
	if (IS_FULL(y))
		tree_split_child(x, i, y);
	
	// 在x节点中找到插入的child节点
	if (key > KEY(x)[i])
		i++;

	// 递归插入到child节点
	tree_insert_nofull(CHILD(y)[i], key);
}

Node* tree_insert(Node* &T, int key)
{
	// 如果根节点已满，则分裂根节点
	if (IS_FULL(T)) {
		Node *n = malloc_node();
		CHILD(n)[1] = T;
		tree_split_child(n, 1, T);
		T = n;
	}
	
	// 当前节点肯定未满，找到插入位置，插入
	tree_insert_nofull(T, key);

	return T;
}

void tree_delete(Node* &T, Node *x)
{
	
}

#ifndef RECURSION
Node* tree_search(const Node *T, int key)
{
	Node *p = const_cast<Node*>(T);	
	while(NIL != p) {
		int i=1;
		while (i<=KEY_NUM(p) && key<KEY(p)[i])
			i++;
		if (i<=KEY_NUM(p) && key==KEY(p)[i])
			return p;
		if (IS_LEAF(p))
			return NIL;
		p = CHILD(p)[i];
	}
	return p;
}

#else

Node* tree_search(const Node *T, int key)
{
	Node *p = const_cast<Node*>(T);
	if ((NIL == p) 
		return p;
	
	int i=1;
	while (i<=KEY_NUM(p) && key>KEY(p)[i])
		i++;
	if (i<=KEY_NUM(p) && key == KEY(p)[i])
		return p;
	if (IS_LEAF(p))
		return NIL;
	return tree_search(CHILD(p)[i], key);
}
#endif

void print_node(const Node *n)
{
	printf("key: ");
	for (int i=1;i<=KEY_NUM(n);i++) {
		printf("%d ", KEY(n)[i]);
	}
	printf("\n");
}