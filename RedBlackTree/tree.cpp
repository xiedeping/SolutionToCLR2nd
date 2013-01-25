#include "tree.h"
#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

Node NIL_NODE = {0, NodeColor::BLACK, NULL, NULL, NULL,};
Node *NIL = &NIL_NODE;

#define KEY(n)    (n->key)
#define COLOR(n)  (n->color)
#define PARENT(n) (n->parent)
#define LEFT(n)   (n->left)
#define RIGHT(n)  (n->right)

#define IS_RED(n)    (NodeColor::RED == COLOR(n))
#define IS_BLACK(n)  (NodeColor::BLACK == COLOR(n))
#define SET_RED(n) (COLOR(n) = NodeColor::RED)
#define SET_BLACK(n) (COLOR(n) = NodeColor::BLACK)
#define IS_ROOT(n) (NIL == PARENT(n))
#define IS_LEFT(n)  ((NIL != PARENT(n)) && (n == LEFT(PARENT(n))))
#define IS_RIGHT(n) ((NIL != PARENT(n)) && (n == RIGHT(PARENT(n))))

Node *malloc_node(int key)
{
	Node *n = (Node *)malloc(sizeof(Node));
	assert(n);
	KEY(n) = key;
	COLOR(n) = NodeColor::RED;
	PARENT(n) = LEFT(n) = RIGHT(n) = &NIL_NODE;

	return n;
}

void free_node(Node *n)
{
	free(n);
}

void left_rotate(Node* &T, Node *x)
{
	if (NIL == RIGHT(x))
		return;

	Node *y = RIGHT(x);

	// y�����ӹҵ�x���Һ���
	RIGHT(x) = LEFT(y);
	if (NIL != LEFT(y))
		PARENT(LEFT(y)) = x;
	
	// x�ĸ�����Ϊy�ĸ���
	PARENT(y) = PARENT(x);

	// x�Ǹ��ڵ㣬��y���µĸ������򣬰�y�ҵ�x�ĸ��ڵ���
	if (NIL == PARENT(x)) {
		T = y;;
	} else if (IS_LEFT(x)) {
		LEFT(PARENT(x)) = y;
	} else {
		RIGHT(PARENT(x)) = y;
	}

	// ��x�ҵ�y������
	LEFT(y) = x;
	PARENT(x) = y;
}

void right_rotate(Node* &T, Node *x)
{
	if (NIL == LEFT(x))
		return;

	Node *y = LEFT(x);

	// y���Һ��ӹҵ�x������
	LEFT(x) = RIGHT(y);
	if (NIL != RIGHT(y))
		PARENT(RIGHT(y)) = x;
	
	// x�ĸ�����Ϊy�ĸ���
	PARENT(y) = PARENT(x);

	// x�Ǹ��ڵ㣬��y���µĸ������򣬰�y�ҵ�x�ĸ��ڵ���
	if (NIL == PARENT(x)) {
		T = y;
	} else if (IS_LEFT(x)) {
		LEFT(PARENT(x)) = y;
	} else {
		RIGHT(PARENT(x)) = y;
	}

	// ��x�ҵ�y���Һ���
	RIGHT(y) = x;
	PARENT(x) = y;
}

void rb_insert_fixup(Node* &T, Node *n)
{
	// ���n�ĸ����Ǻ�ɫ������Ҫ����
	while(IS_RED(PARENT(n))) {
		// n������ڵ�
		Node *p = IS_LEFT(PARENT(n))? RIGHT(PARENT(PARENT(n))): LEFT(PARENT(PARENT(n)));
		if (IS_RED(p)) {
			SET_BLACK(PARENT(n));
			SET_BLACK(p);
			SET_RED(PARENT(PARENT(n)));
			n = PARENT(PARENT(n));
			continue;
		} 

		if (IS_LEFT(PARENT(n))) { 
			if (IS_RIGHT(n)) {
				n = PARENT(n);
				// ����
				left_rotate(T, n);
			}
			SET_BLACK(PARENT(n));
			SET_RED(PARENT(PARENT(n)));
			// ����
			right_rotate(T, PARENT(PARENT(n)));
		} else {
			if (IS_LEFT(n)) {
				n = PARENT(n);
				// ����
				right_rotate(T,n);
			}
			SET_BLACK(PARENT(n));
			SET_RED(PARENT(PARENT(n)));
			// ����
			left_rotate(T,PARENT(PARENT(n)));
		}
	}
	
	// ���n�Ǹ��ڵ㣬��Ѹ�����Ϊ��ɫ
	if (NIL == PARENT(n))
		SET_BLACK(n);
}

Node* tree_insert(Node* &T, int key)
{
	// �����½ڵ�
	Node *n = malloc_node(key);
	if (NIL == T){
		T = n;
		rb_insert_fixup(T, n);
		return n;
	}
		
	// �ҵ�����λ��
	Node *p = T;
	Node *q;
	while(NIL != p) {
		q = p;
		if (key < KEY(p))
			p = LEFT(p);
		else
			p = RIGHT(p);
	}

	// ���ڵ�Ϊq
	PARENT(n) = q;
	if (key < KEY(q)) {
		LEFT(q) = n;
	} else {
		RIGHT(q) = n;
	}

	// ����
	rb_insert_fixup(T, n);
	return n;
}

void rb_delete_fixup(Node* &T, Node *x)
{
	while ((NIL!= x) && !IS_ROOT(x) && IS_BLACK(x)) {
		if (IS_LEFT(x)) {
			// w��x�����ֵܣ�w�����治ͬ����ʽ��ѡ������
			// �ܹ����ִ���ʽ��
			// case 1: w�Ǻ�ɫ���������ӵ���ɫ����ν
			// case 2: w�Ǻ�ɫ�����Ӻ�ɫ���Һ��Ӻ�ɫ
			// case 3: w�Ǻ�ɫ�����Ӻ�ɫ���Һ��Ӻ�ɫ
			// case 4: w�Ǻ�ɫ���Һ��Ӻ�ɫ�����ӵ���ɫ����ν
			Node *w = RIGHT(PARENT(x));  
			// case1: w�Ǻ�ɫ����ͨ��һ����������ˢ�ɺ�ɫ��ת��case 2��3��4
			if (IS_RED(w)) {
				SET_BLACK(w);
				SET_RED(PARENT(x));
				left_rotate(T, PARENT(x));
			}
			
			// case 2: w���������Ӷ��Ǻ�ɫ����wˢ�ɺ�ɫ��x�ĸ���ȡ��x�����ϵݹ鴦��
			if (IS_BLACK(LEFT(w)) && IS_BLACK(RIGHT(w))) {
				SET_RED(w);
				x = PARENT(x);
				continue;
			}
			// case 3: w�����Ӻ�ɫ���Һ��Ӻ�ɫ����w����ˢ�ɺ�ɫ��wˢ�ɺ�ɫ����һ��������ת��case 4
			if (IS_BLACK(RIGHT(w))) {
				SET_BLACK(LEFT(w));
				SET_RED(w);
				right_rotate(T, w);
				w = PARENT(w);    // ת��case 4
			} 
			
			// case 4: w���Һ���Ϊ��ɫ����wˢ�ɺ�ɫ��w�ҽڵ�ˢ�ɺ�ɫ��x����ˢ�ɺ�ɫ����һ�����������������ʣ���������
			COLOR(w) = COLOR(PARENT(x));
			SET_BLACK(RIGHT(w));
			SET_BLACK(PARENT(x));
			left_rotate(T, PARENT(x));
			x = T; 
		} else {
			// w��x�����ֵܣ�w�����治ͬ����ʽ��ѡ������
			// �ܹ����ִ���ʽ��
			// case 1: w�Ǻ�ɫ���������ӵ���ɫ����ν
			// case 2: w�Ǻ�ɫ�����Ӻ�ɫ���Һ��Ӻ�ɫ
			// case 3: w�Ǻ�ɫ�����Ӻ�ɫ���Һ��Ӻ�ɫ
			// case 4: w�Ǻ�ɫ���Һ��Ӻ�ɫ�����ӵ���ɫ����ν
			Node *w = LEFT(PARENT(x));  
			// case1: w�Ǻ�ɫ����ͨ��һ����������ˢ�ɺ�ɫ��ת��case 2��3��4
			if (IS_RED(w)) {
				SET_BLACK(w);
				SET_RED(PARENT(x));
				right_rotate(T, PARENT(x));
			}
			
			// case 2: w���������Ӷ��Ǻ�ɫ����wˢ�ɺ�ɫ��x�ĸ���ȡ��x�����ϵݹ鴦��
			if (IS_BLACK(LEFT(w)) && IS_BLACK(RIGHT(w))) {
				SET_RED(w);
				x = PARENT(x);
				continue;
			}
			// case 3: w�����Ӻ�ɫ���Һ��Ӻ�ɫ����w�Һ���ˢ�ɺ�ɫ��wˢ�ɺ�ɫ����һ��������ת��case 4
			if (IS_BLACK(LEFT(w))) {
				SET_BLACK(RIGHT(w));
				SET_RED(w);
				left_rotate(T, w);
				w = PARENT(w);    // ת��case 4
			}
			
			// case 4: w������Ϊ��ɫ����wˢ�ɺ�ɫ��w��ڵ�ˢ�ɺ�ɫ��x����ˢ�ɺ�ɫ����һ�����������������ʣ���������
			COLOR(w) = COLOR(PARENT(x));
			SET_BLACK(LEFT(w));
			SET_BLACK(PARENT(x));
			right_rotate(T, PARENT(x));
			x = T;
		}
	}

	// ���x�Ǹ��ڵ㣬��Ϊ��ɫ����ˢ�ɺ�ɫ�����ɱ��ֺ��������
	SET_BLACK(x);
}

void tree_delete(Node* &T, Node *x)
{
	if (NIL == T || NIL == x)
		return;

	// y��Ҫɾ���Ľڵ�
	Node *y = NULL;
	if ((NIL == LEFT(x)) || (NIL == RIGHT(x))) 
		y = x;
	else 
		y = tree_successor(x);

	if (y != x)
		KEY(x) = KEY(y);

	// y�϶�ֻ��һ������
	Node * z = (NIL != LEFT(y))? LEFT(y): RIGHT(y);	
	// ��ʹz��NIL��Ҳ�����ϣ������Ӱ��rb_delete_fixup
	PARENT(z) = PARENT(y);

	if (NIL == PARENT(y)) {
		// ���ڵ㷢���仯
		T = z;
	} else if (IS_LEFT(y)){
		LEFT(PARENT(y)) = z;
	} else {
		RIGHT(PARENT(y)) = z;
	}

	// ���y�Ǻ�ɫ��˵���ƻ�������Ĺ������y�Ǻ�ɫ���򲻻��ƻ�
	if (IS_BLACK(y)) {
		rb_delete_fixup(T, z);
	}
	free_node(y);
}

#ifndef RECURSION
Node* tree_min(Node *T)
{
	if (NIL == T)
		return T;
	
	while ((NIL != LEFT(T)) && (T = LEFT(T)));
	return T;
}
#else
Node* tree_min(Node *T)
{
	if (NIL == T)
		return T;
	
	return (NIL != LEFT(T))? tree_min(LEFT(T)): T;
}
#endif

#ifndef RECURSION
Node* tree_max(Node *T)
{
	if (NIL == T)
		return T;

	while ((NIL != RIGHT(T)) && (T = RIGHT(T)));
	return T;
}
#else
Node* tree_max(Node *T)
{
	if (NIL == T)
		return T;

	return (NIL != RIGHT(T))? tree_max(RIGHT(T)): T;
}
#endif

#ifndef RECURSION
Node* tree_search(Node *T, int key)
{
	if (NIL == T)
		return T;

	while(NIL != T) {
		if (key == KEY(T))
			return T;
		else if (key < KEY(T))
			T = LEFT(T);
		else
			T = RIGHT(T);
	}
	return T;
}

#else

Node* tree_search(Node *T, int key)
{
	if ((NIL == T) || (key == KEY(T))) {
		return T;
	} else if (key < KEY(T)) {
		return tree_search(LEFT(T), key);
	} else {
		return tree_search(RIGHT(T), key);
	}
}
#endif

Node* tree_successor(Node *n)
{
	if (NIL == n)
		return n;

	if (NIL != RIGHT(n))
		return tree_min(RIGHT(n));
	
	while (IS_RIGHT(n) && (n = PARENT(n)));	
	return PARENT(n);
}

Node* tree_predecessor(Node *n)
{
	if (NIL == n)
		return n;
	
	if (NIL != LEFT(n))
		return tree_max(LEFT(n));

	while(IS_LEFT(n) && (n = PARENT(n)));
	return PARENT(n);
}

#ifdef RECURSION
void inorder_tree_walk(Node *T)
{
	if (NIL == T)
		return;

	inorder_tree_walk(LEFT(T));
	printf("%d, ", KEY(T));
	inorder_tree_walk(RIGHT(T));
}
#else
void inorder_tree_walk(Node *T)
{
	if (NIL == T)
		return;

	// �������׹�������δ�����Ľڵ㡱�͡��Ѿ����Ľڵ㡱
	Node *p = T;
	while (NIL != p) {
		// ============δ�����Ľڵ�=============
		// ���Ӳ�Ϊ��, �����±�������
		if (NIL != LEFT(p)) {
			p = LEFT(p);
			continue;
		}
		// ����Ϊ�գ�����ʱ��ڵ�
		printf("%d, ",KEY(p));

		// �Һ��Ӳ�Ϊ�գ������������
		if (NIL != RIGHT(p)) {
			p = RIGHT(p);
			continue;
		}

		// ============�Ѿ����Ľڵ�=============
		// �Һ���Ϊ�գ���ʼ���ϴ�����
		while (true) {
			if (IS_RIGHT(p)) {
				// ������ڵ��Ǹ��׵��Һ��ӣ���������ϴ�����
				p = PARENT(p);
			} else {
				// ���ڵ��Ǹ��׵����ӣ�����ʸ���
				p = PARENT(p);
				printf("%d, ", KEY(p));

				// ����Һ��Ӳ�Ϊ�գ����������������δ�����Ľڵ㡱��
				if (NIL != RIGHT(p)) {
					p = RIGHT(p);
					break;
				}
				// �Һ���Ϊ�գ���������ϴ�����
			}

			// ��������Ǹ�������ζ�ŷ��ʽ���
			if (IS_ROOT(p))
				return;
		}
	}
}
#endif
void print_node(Node *n)
{
	printf("key: %d color: %d\n", KEY(n), COLOR(n));
}