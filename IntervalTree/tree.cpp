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

	// 重新计算x和y的max值
	MAX(x) = max(MAX(LEFT(x)), MAX(RIGHT(x)), HIGH(KEY(x)));
	MAX(y) = max(MAX(LEFT(y)), MAX(RIGHT(y)), HIGH(KEY(y)));
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

	// 重新计算x和y的max值
	MAX(x) = max(MAX(LEFT(x)), MAX(RIGHT(x)), HIGH(KEY(x)));
	MAX(y) = max(MAX(LEFT(y)), MAX(RIGHT(y)), HIGH(KEY(y)));
}

void rb_insert_fixup(Node* &T, Node *n)
{
	// 如果n的父亲是红色，则需要调整
	// 如果n的父亲是黑色，则不需要调整，因为n本身是红色
	while(IS_RED(PARENT(n))) {
		// 根据n的叔叔节点的颜色，来决定调整方案
		Node *p = IS_LEFT(PARENT(n))? RIGHT(PARENT(PARENT(n))): LEFT(PARENT(PARENT(n)));

		// 如果叔叔是红色，那很简单，把爷爷的黑色转移给父亲和叔叔，爷爷刷成红色
		// 这样，即满足了性质4，也没有破坏性质5及其他性质
		// 但是，爷爷可能破坏了红黑性质4，则从爷爷开始继续调整（向上递归了两层）
		if (IS_RED(p)) {
			// 父亲刷成黑色
			SET_BLACK(PARENT(n));
			// 叔叔刷成黑色
			SET_BLACK(p);
			// 爷爷刷成红色
			SET_RED(PARENT(PARENT(n)));

			// 从爷爷开始继续调整
			n = PARENT(PARENT(n));
			continue;
		} 

		// 如果叔叔是黑色，就复杂一点，引入旋转操作
		// 如果n是左孩子，那么需要一次右旋+颜色调整即可
		// 如果n是右孩子，则通过一次左旋调整成左孩子，然后按上面情况处理
		if (IS_LEFT(PARENT(n))) { 
			// 如果n是右孩子，通过右旋调整成左孩子
			if (IS_RIGHT(n)) {
				n = PARENT(n);
				left_rotate(T, n);
			}

			// 现在n是左孩子了
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
	
	// 如果n是根节点，则把根设置为黑色
	if (NIL == PARENT(n))
		SET_BLACK(n);
}

Node* tree_insert(Node* &T, Interval key)
{
	// 分配新节点
	Node *n = malloc_node(key);

	// 如果当前是空树，则n节点作为根，刷成黑色即满足红黑性质
	if (NIL == T){
		SET_BLACK(n);
		T = n;
		return n;
	}
	
	// 如果当前不是空树，则先找到插入位置
	Node *p = T;
	Node *q;
	while(NIL != p) {
		// 调整路径上节点的max值
		if (MAX(p) < HIGH(key))
			MAX(p) = HIGH(key);

		q = p;
		if (LT(key,KEY(p)))
			p = LEFT(p);
		else
			p = RIGHT(p);
	}

	// 找到了插入位置，n节点的父亲为q
	PARENT(n) = q;
	if (LT(key, KEY(q))) {
		LEFT(q) = n;
	} else {
		RIGHT(q) = n;
	}

	// 设置n节点为红色
	// 这样只可能违背红黑性质4（性质4的调整应该比性质5简单）
	SET_RED(n);

	// 从n节点开始调整，使之符合红黑性质4
	// 调整的每一步，都不会破坏其他红黑性质
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
			
			// case 2: w的两个孩子都是黑色，把w刷成红色，x的父亲取代x，向上递归处理
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

	
	// 调整路径上节点的max值
	Node *p = PARENT(z);
	while (NIL != p) {
		MAX(p) = max(MAX(LEFT(p)), MAX(RIGHT(p)), HIGH(KEY(p)));
		p = PARENT(p);
	}

	// 如果y是黑色，说明破坏红黑树的规则；如果y是红色，则不会破坏
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

	// 按照两套规则处理：“未经过的节点”和“已经过的节点”
	Node *p = const_cast<Node*>(T);
	while (NIL != p) {
		// ============未经过的节点=============
		// 左孩子不为空, 则向下遍历左孩子
		if (NIL != LEFT(p)) {
			p = LEFT(p);
			continue;
		}
		// 左孩子为空，则访问本节点
		print_node(p);

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
				print_node(p);

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
void print_node(const Node *n)
{
	printf("key: (%d,%d) color: %d \n", LOW(KEY(n)), HIGH(KEY(n)), COLOR(n));
}