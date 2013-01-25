#include "tree.h"
#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <stdio.h>

Node NIL_NODE = { {-1,-1}, NodeColor::BLACK, NULL, NULL, NULL,0};
Node *NIL = &NIL_NODE;

#define KEY(n)    (n->key)
#define LOW(i)    (i.low)
#define HIGH(i)   (i.high)
#define COLOR(n)  (n->color)
#define PARENT(n) (n->parent)
#define LEFT(n)   (n->left)
#define RIGHT(n)  (n->right)
#define MAX(n)    (n->max)
#define OVERLAP(i, j) ((LOW(i)<=HIGH(j)) && (LOW(j)<=HIGH(i)))
#define IDENTIFY(i,j) ((LOW(i)==LOW(j)) && (HIGH(i)==HIGH(j)))
#define EQ(i,j)   (LOW(i)==LOW(j))
#define LT(i,j)   (LOW(i)<LOW(j))
#define LE(i,j)   (LOW(i)<=LOW(j))
#define GT(i,j)   (LOW(i)>LOW(j))
#define GE(i,j)   (LOW(i)>=LOW(j))


#define IS_RED(n)    (NodeColor::RED == COLOR(n))
#define IS_BLACK(n)  (NodeColor::BLACK == COLOR(n))
#define SET_RED(n)   (COLOR(n) = NodeColor::RED)
#define SET_BLACK(n) (COLOR(n) = NodeColor::BLACK)
#define IS_ROOT(n)   (NIL == PARENT(n))
#define IS_LEFT(n)   ((NIL != PARENT(n)) && (n == LEFT(PARENT(n))))
#define IS_RIGHT(n)  ((NIL != PARENT(n)) && (n == RIGHT(PARENT(n))))

int max(int a, int b, int c) {
	if (a > b) {
		if (a > c)
			return a;
		return c;
	} else {
		if (b > c)
			return b;
		return c;
	}
}

Node *malloc_node(Interval key)
{
	Node *n = (Node *)malloc(sizeof(Node));
	assert(n);
	KEY(n) = key;
	SET_RED(n);
	PARENT(n) = LEFT(n) = RIGHT(n) = &NIL_NODE;
	n->max = HIGH(key);

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

	// ���¼���x��y��maxֵ
	MAX(x) = max(MAX(LEFT(x)), MAX(RIGHT(x)), HIGH(KEY(x)));
	MAX(y) = max(MAX(LEFT(y)), MAX(RIGHT(y)), HIGH(KEY(y)));
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

	// ���¼���x��y��maxֵ
	MAX(x) = max(MAX(LEFT(x)), MAX(RIGHT(x)), HIGH(KEY(x)));
	MAX(y) = max(MAX(LEFT(y)), MAX(RIGHT(y)), HIGH(KEY(y)));
}

void rb_insert_fixup(Node* &T, Node *n)
{
	// ���n�ĸ����Ǻ�ɫ������Ҫ����
	// ���n�ĸ����Ǻ�ɫ������Ҫ��������Ϊn�����Ǻ�ɫ
	while(IS_RED(PARENT(n))) {
		// ����n������ڵ����ɫ����������������
		Node *p = IS_LEFT(PARENT(n))? RIGHT(PARENT(PARENT(n))): LEFT(PARENT(PARENT(n)));

		// ��������Ǻ�ɫ���Ǻܼ򵥣���үү�ĺ�ɫת�Ƹ����׺����壬үүˢ�ɺ�ɫ
		// ������������������4��Ҳû���ƻ�����5����������
		// ���ǣ�үү�����ƻ��˺������4�����үү��ʼ�������������ϵݹ������㣩
		if (IS_RED(p)) {
			// ����ˢ�ɺ�ɫ
			SET_BLACK(PARENT(n));
			// ����ˢ�ɺ�ɫ
			SET_BLACK(p);
			// үүˢ�ɺ�ɫ
			SET_RED(PARENT(PARENT(n)));

			// ��үү��ʼ��������
			n = PARENT(PARENT(n));
			continue;
		} 

		// ��������Ǻ�ɫ���͸���һ�㣬������ת����
		// ���n�����ӣ���ô��Ҫһ������+��ɫ��������
		// ���n���Һ��ӣ���ͨ��һ���������������ӣ�Ȼ�������������
		if (IS_LEFT(PARENT(n))) { 
			// ���n���Һ��ӣ�ͨ����������������
			if (IS_RIGHT(n)) {
				n = PARENT(n);
				left_rotate(T, n);
			}

			// ����n��������
			SET_BLACK(PARENT(n));
			SET_RED(PARENT(PARENT(n)));
			right_rotate(T, PARENT(PARENT(n)));
		} else {
			if (IS_LEFT(n)) {
				n = PARENT(n);
				right_rotate(T,n);
			}
			SET_BLACK(PARENT(n));
			SET_RED(PARENT(PARENT(n)));
			left_rotate(T,PARENT(PARENT(n)));
		}
	}
	
	// ���n�Ǹ��ڵ㣬��Ѹ�����Ϊ��ɫ
	if (NIL == PARENT(n))
		SET_BLACK(n);
}

Node* tree_insert(Node* &T, Interval key)
{
	// �����½ڵ�
	Node *n = malloc_node(key);

	// �����ǰ�ǿ�������n�ڵ���Ϊ����ˢ�ɺ�ɫ������������
	if (NIL == T){
		SET_BLACK(n);
		T = n;
		return n;
	}
	
	// �����ǰ���ǿ����������ҵ�����λ��
	Node *p = T;
	Node *q;
	while(NIL != p) {
		// ����·���Ͻڵ��maxֵ
		if (MAX(p) < HIGH(key))
			MAX(p) = HIGH(key);

		q = p;
		if (LT(key,KEY(p)))
			p = LEFT(p);
		else
			p = RIGHT(p);
	}

	// �ҵ��˲���λ�ã�n�ڵ�ĸ���Ϊq
	PARENT(n) = q;
	if (LT(key, KEY(q))) {
		LEFT(q) = n;
	} else {
		RIGHT(q) = n;
	}

	// ����n�ڵ�Ϊ��ɫ
	// ����ֻ����Υ���������4������4�ĵ���Ӧ�ñ�����5�򵥣�
	SET_RED(n);

	// ��n�ڵ㿪ʼ������ʹ֮���Ϻ������4
	// ������ÿһ�����������ƻ������������
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

	
	// ����·���Ͻڵ��maxֵ
	Node *p = PARENT(z);
	while (NIL != p) {
		MAX(p) = max(MAX(LEFT(p)), MAX(RIGHT(p)), HIGH(KEY(p)));
		p = PARENT(p);
	}

	// ���y�Ǻ�ɫ��˵���ƻ�������Ĺ������y�Ǻ�ɫ���򲻻��ƻ�
	if (IS_BLACK(y)) {
		rb_delete_fixup(T, z);
	}
	free_node(y);
}

#ifndef RECURSION
Node* tree_min(const Node *T)
{
	Node *p = const_cast<Node*>(T);
	if (NIL == p)
		return p;
	
	while ((NIL != LEFT(p)) && (p = LEFT(p)));
	return p;
}
#else
Node* tree_min(const Node *T)
{
	Node *p = const_cast<Node*>(T);
	if (NIL == p)
		return p;
	
	return (NIL != LEFT(p))? tree_min(LEFT(p)): p;
}
#endif

#ifndef RECURSION
Node* tree_max(const Node *T)
{
	Node *p = const_cast<Node*>(T);
	if (NIL == p)
		return p;

	while ((NIL != RIGHT(p)) && (p = RIGHT(p)));
	return p;
}
#else
Node* tree_max(const Node *p)
{
	Node *p = const_cast<Node*>(T);
	if (NIL == p)
		return p;

	return (NIL != RIGHT(p))? tree_max(RIGHT(p)): p;
}
#endif

#ifndef RECURSION
Node* tree_search(const Node *T, Interval key)
{
	Node *p = const_cast<Node*>(T);
	if (NIL == p)
		return p;

	while(NIL != p) {
		if (EQ(key,KEY(p)))
			return p;
		else if (LT(key,KEY(p)))
			p = LEFT(p);
		else
			p = RIGHT(p);
	}
	return p;
}

#else

Node* tree_search(const Node *T, Interval key)
{
	Node *p = const_cast<Node*>(T);
	if ((NIL == p) || QE(key,KEY(p))) {
		return p;
	} else if (LT(key,KEY(p))) {
		return tree_search(LEFT(p), key);
	} else {
		return tree_search(RIGHT(p), key);
	}
}
#endif

#ifndef RECURSION
Node* interval_search(const Node *T, Interval key)
{
	Node *p = const_cast<Node*>(T);
	
	while(NIL != p && !OVERLAP(key, KEY(p))) {
		if (MAX(LEFT(p)) >= LOW(key)) {
			p = LEFT(p);
		} else {
			p = RIGHT(p);
		}
	}
	return p;
}

#else

Node* interval_search(const Node *T, Interval key)
{
	if (NIL == p || OVERLAP(key, KEY(p))) 
		return p;

	if (MAX(LEFT(p)) >= LOW(key)) {
		return tree_search(LEFT(p), key);
	} else 
		return tree_search(RIGHT(p), key);
	}
}
#endif

Node* min_interval_search(const Node *T, Interval key)
{
	Node *p = const_cast<Node*>(T);
	if (NIL == p)
		return NIL;

	Node *q = NIL;
	if ((NIL!=LEFT(p)) && (MAX(LEFT(p))>=LOW(key))) {
		q = min_interval_search(LEFT(p), key);
		if (NIL != q)
			return q;
		else if (OVERLAP(key, KEY(p)))
			return p;
		else
			return NIL;
	} else if (OVERLAP(key, KEY(p))){
		return p;
	} else {
		return min_interval_search(RIGHT(p), key);
	}
}
Node* tree_successor(const Node *n)
{
	Node *p = const_cast<Node*>(n);
	if (NIL == p)
		return p;

	if (NIL != RIGHT(p))
		return tree_min(RIGHT(p));
	
	while (IS_RIGHT(p) && (p = PARENT(p)));	
	return PARENT(p);
}

Node* tree_predecessor(const Node *n)
{
	Node *p = const_cast<Node*>(n);
	if (NIL == p)
		return p;
	
	if (NIL != LEFT(p))
		return tree_max(LEFT(p));

	while(IS_LEFT(p) && (p = PARENT(p)));
	return PARENT(p);
}

#ifdef RECURSION
void inorder_tree_walk(const Node *T)
{
	if (NIL == T)
		return;

	inorder_tree_walk(LEFT(T));
	print_node(p);
	inorder_tree_walk(RIGHT(T));
}
#else
void inorder_tree_walk(const Node *T)
{
	if (NIL == T)
		return;

	// �������׹�������δ�����Ľڵ㡱�͡��Ѿ����Ľڵ㡱
	Node *p = const_cast<Node*>(T);
	while (NIL != p) {
		// ============δ�����Ľڵ�=============
		// ���Ӳ�Ϊ��, �����±�������
		if (NIL != LEFT(p)) {
			p = LEFT(p);
			continue;
		}
		// ����Ϊ�գ�����ʱ��ڵ�
		print_node(p);

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
				print_node(p);

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
void print_node(const Node *n)
{
	printf("key: (%d,%d) color: %d \n", LOW(KEY(n)), HIGH(KEY(n)), COLOR(n));
}