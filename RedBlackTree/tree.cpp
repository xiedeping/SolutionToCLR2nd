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

	// y的左孩子挂到x的右孩子
	RIGHT(x) = LEFT(y);
	if (NIL != LEFT(y))
		PARENT(LEFT(y)) = x;
	
	// x的父亲作为y的父亲
	PARENT(y) = PARENT(x);

	// x是根节点，则y是新的根；否则，把y挂到x的父节点下
	if (NIL == PARENT(x)) {
		T = y;;
	} else if (IS_LEFT(x)) {
		LEFT(PARENT(x)) = y;
	} else {
		RIGHT(PARENT(x)) = y;
	}

	// 把x挂到y的左孩子
	LEFT(y) = x;
	PARENT(x) = y;
}

void right_rotate(Node* &T, Node *x)
{
	if (NIL == LEFT(x))
		return;

	Node *y = LEFT(x);

	// y的右孩子挂到x的左孩子
	LEFT(x) = RIGHT(y);
	if (NIL != RIGHT(y))
		PARENT(RIGHT(y)) = x;
	
	// x的父亲作为y的父亲
	PARENT(y) = PARENT(x);

	// x是根节点，则y是新的根；否则，把y挂到x的父节点下
	if (NIL == PARENT(x)) {
		T = y;
	} else if (IS_LEFT(x)) {
		LEFT(PARENT(x)) = y;
	} else {
		RIGHT(PARENT(x)) = y;
	}

	// 把x挂到y的右孩子
	RIGHT(y) = x;
	PARENT(x) = y;
}

void rb_insert_fixup(Node* &T, Node *n)
{
	// 如果n的父亲是红色，则需要调整
	while(IS_RED(PARENT(n))) {
		// n的叔叔节点
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
				// 左旋
				left_rotate(T, n);
			}
			SET_BLACK(PARENT(n));
			SET_RED(PARENT(PARENT(n)));
			// 右旋
			right_rotate(T, PARENT(PARENT(n)));
		} else {
			if (IS_LEFT(n)) {
				n = PARENT(n);
				// 右旋
				right_rotate(T,n);
			}
			SET_BLACK(PARENT(n));
			SET_RED(PARENT(PARENT(n)));
			// 左旋
			left_rotate(T,PARENT(PARENT(n)));
		}
	}
	
	// 如果n是根节点，则把根设置为黑色
	if (NIL == PARENT(n))
		SET_BLACK(n);
}

Node* tree_insert(Node* &T, int key)
{
	// 分配新节点
	Node *n = malloc_node(key);
	if (NIL == T){
		T = n;
		rb_insert_fixup(T, n);
		return n;
	}
		
	// 找到插入位置
	Node *p = T;
	Node *q;
	while(NIL != p) {
		q = p;
		if (key < KEY(p))
			p = LEFT(p);
		else
			p = RIGHT(p);
	}

	// 父节点为q
	PARENT(n) = q;
	if (key < KEY(q)) {
		LEFT(q) = n;
	} else {
		RIGHT(q) = n;
	}

	// 调整
	rb_insert_fixup(T, n);
	return n;
}

void rb_delete_fixup(Node* &T, Node *x)
{
	while ((NIL!= x) && !IS_ROOT(x) && IS_BLACK(x)) {
		if (IS_LEFT(x)) {
			// w是x的右兄弟，w是下面不同处理方式的选择依据
			// 总共四种处理方式：
			// case 1: w是红色，两个孩子的颜色无所谓
			// case 2: w是黑色，左孩子黑色，右孩子黑色
			// case 3: w是黑色，左孩子红色，右孩子黑色
			// case 4: w是黑色，右孩子红色，左孩子的颜色无所谓
			Node *w = RIGHT(PARENT(x));  
			// case1: w是红色，则通过一次左旋，并刷成黑色，转成case 2、3、4
			if (IS_RED(w)) {
				SET_BLACK(w);
				SET_RED(PARENT(x));
				left_rotate(T, PARENT(x));
			}
			
			// case 2: w的两个孩子都是黑色，把w刷成红色，x的父亲取代x，线上递归处理
			if (IS_BLACK(LEFT(w)) && IS_BLACK(RIGHT(w))) {
				SET_RED(w);
				x = PARENT(x);
				continue;
			}
			// case 3: w的左孩子红色，右孩子黑色，把w左孩子刷成黑色，w刷成红色，做一次右旋，转成case 4
			if (IS_BLACK(RIGHT(w))) {
				SET_BLACK(LEFT(w));
				SET_RED(w);
				right_rotate(T, w);
				w = PARENT(w);    // 转成case 4
			} 
			
			// case 4: w的右孩子为红色，把w刷成红色，w右节点刷成黑色，x父亲刷成黑色，做一次左旋，满足红黑性质，结束处理
			COLOR(w) = COLOR(PARENT(x));
			SET_BLACK(RIGHT(w));
			SET_BLACK(PARENT(x));
			left_rotate(T, PARENT(x));
			x = T; 
		} else {
			// w是x的左兄弟，w是下面不同处理方式的选择依据
			// 总共四种处理方式：
			// case 1: w是红色，两个孩子的颜色无所谓
			// case 2: w是黑色，左孩子黑色，右孩子黑色
			// case 3: w是黑色，左孩子红色，右孩子黑色
			// case 4: w是黑色，右孩子红色，左孩子的颜色无所谓
			Node *w = LEFT(PARENT(x));  
			// case1: w是红色，则通过一次右旋，并刷成黑色，转成case 2、3、4
			if (IS_RED(w)) {
				SET_BLACK(w);
				SET_RED(PARENT(x));
				right_rotate(T, PARENT(x));
			}
			
			// case 2: w的两个孩子都是黑色，把w刷成红色，x的父亲取代x，线上递归处理
			if (IS_BLACK(LEFT(w)) && IS_BLACK(RIGHT(w))) {
				SET_RED(w);
				x = PARENT(x);
				continue;
			}
			// case 3: w的左孩子黑色，右孩子红色，把w右孩子刷成黑色，w刷成红色，做一次左旋，转成case 4
			if (IS_BLACK(LEFT(w))) {
				SET_BLACK(RIGHT(w));
				SET_RED(w);
				left_rotate(T, w);
				w = PARENT(w);    // 转成case 4
			}
			
			// case 4: w的左孩子为红色，把w刷成红色，w左节点刷成黑色，x父亲刷成黑色，做一次右旋，满足红黑性质，结束处理
			COLOR(w) = COLOR(PARENT(x));
			SET_BLACK(LEFT(w));
			SET_BLACK(PARENT(x));
			right_rotate(T, PARENT(x));
			x = T;
		}
	}

	// 如果x是根节点，或为红色，则都刷成黑色，即可保持红黑树性质
	SET_BLACK(x);
}

void tree_delete(Node* &T, Node *x)
{
	if (NIL == T || NIL == x)
		return;

	// y是要删除的节点
	Node *y = NULL;
	if ((NIL == LEFT(x)) || (NIL == RIGHT(x))) 
		y = x;
	else 
		y = tree_successor(x);

	if (y != x)
		KEY(x) = KEY(y);

	// y肯定只有一个孩子
	Node * z = (NIL != LEFT(y))? LEFT(y): RIGHT(y);	
	// 即使z是NIL，也给挂上，否则会影响rb_delete_fixup
	PARENT(z) = PARENT(y);

	if (NIL == PARENT(y)) {
		// 根节点发生变化
		T = z;
	} else if (IS_LEFT(y)){
		LEFT(PARENT(y)) = z;
	} else {
		RIGHT(PARENT(y)) = z;
	}

	// 如果y是黑色，说明破坏红黑树的规则；如果y是红色，则不会破坏
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

	// 按照两套规则处理：“未经过的节点”和“已经过的节点”
	Node *p = T;
	while (NIL != p) {
		// ============未经过的节点=============
		// 左孩子不为空, 则向下遍历左孩子
		if (NIL != LEFT(p)) {
			p = LEFT(p);
			continue;
		}
		// 左孩子为空，则访问本节点
		printf("%d, ",KEY(p));

		// 右孩子不为空，则访问右子树
		if (NIL != RIGHT(p)) {
			p = RIGHT(p);
			continue;
		}

		// ============已经过的节点=============
		// 右孩子为空，则开始向上处理父亲
		while (true) {
			if (IS_RIGHT(p)) {
				// 如果本节点是父亲的右孩子，则继续向上处理父亲
				p = PARENT(p);
			} else {
				// 本节点是父亲的左孩子，则访问父亲
				p = PARENT(p);
				printf("%d, ", KEY(p));

				// 如果右孩子不为空，则访问右子树（“未经过的节点”）
				if (NIL != RIGHT(p)) {
					p = RIGHT(p);
					break;
				}
				// 右孩子为空，则继续向上处理父亲
			}

			// 如果父亲是根，则意味着访问结束
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