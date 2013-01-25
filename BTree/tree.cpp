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
	// ע�⣺y�����ڵ㣬x�������ڵ㣬�±��1��ʼ

	// ����һ���½ڵ㣬��Ϊy�����ֵ�
	Node *z = malloc_node();
	LEAF(z) = LEAF(y);
	KEY_NUM(z) = MIN_CHILD;

	// ����y�ڵ���Ұ벿��key�����±�t+1��ʼ)��z�ڵ�
	for (int j=1;j<=MIN_CHILD;j++) {
		KEY(z)[j] = KEY(y)[j+MIN_CHILD+1];
	}
	KEY_NUM(y) = KEY_NUM(z) = MIN_CHILD;

	// ���y����Ҷ�ڵ㣬�򿽱�y�ڵ���Ұ벿��child�����±�t+1��ʼ����z�ڵ�
	if (!IS_LEAF(y)) {
		for (int j=1;j<=MIN_CHILD;j++) {
			CHILD(z)[j] = CHILD(z)[j+MIN_CHILD+1];
		}
	}

	// x�ڵ㣬���±�i��ʼ��key��ȫ������һ��
	for (int j=KEY_NUM(x);j>=i;j--) {
		KEY(x)[j+1] = KEY(x)[j];
	}
	// ��y�ڵ��±�Ϊt��key��������x�ڵ��iλ��
	KEY(x)[i] = KEY(y)[MIN_CHILD+1];

	// x�ڵ㣬���±�i+1��ʼ��child��ȫ������һ��
	for (int j=KEY_NUM(x)+1;j>i;j--) {
		CHILD(x)[j+1] = CHILD(x)[j];
	}
	// x�ڵ��±�Ϊi+1��child��ָ��z�ڵ�
	CHILD(x)[i+1] = z;
	
	// z�ڵ��key����+1
	KEY_NUM(x)++;
}

void tree_insert_nofull(Node *x, int key)
{
	// ע��: x�ڵ�϶�δ��

	int i=KEY_NUM(x);
	// ���x��Ҷ�ڵ㣬��ֱ���ҵ�λ�ò���
	if (IS_LEAF(x)) {
		while (i>=1 && key<KEY(x)[i]) {
			KEY(x)[i+1] = KEY(x)[i];
			i--;
		}
		KEY(x)[i+1] = key;
		KEY_NUM(x)++;
		return;
	}

	// x����Ҷ�ڵ㣬�ҵ�������Ľڵ�y
	while (i>=1 && key<KEY(x)[i]) {
		i--;
	}
	i++;
	Node *y = CHILD(x)[i];
	
	// ���ڵ�y�ǲ�������������ǣ����ȷ���
	if (IS_FULL(y))
		tree_split_child(x, i, y);
	
	// ��x�ڵ����ҵ������child�ڵ�
	if (key > KEY(x)[i])
		i++;

	// �ݹ���뵽child�ڵ�
	tree_insert_nofull(CHILD(y)[i], key);
}

Node* tree_insert(Node* &T, int key)
{
	// ������ڵ�����������Ѹ��ڵ�
	if (IS_FULL(T)) {
		Node *n = malloc_node();
		CHILD(n)[1] = T;
		tree_split_child(n, 1, T);
		T = n;
	}
	
	// ��ǰ�ڵ�϶�δ�����ҵ�����λ�ã�����
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