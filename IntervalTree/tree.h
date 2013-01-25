typedef enum _NodeColor {
	RED,
	BLACK,
} NodeColor;

typedef struct _Interval
{
	int low;
	int high;
} Interval;

typedef struct _Node {
	Interval key;
	NodeColor color;
	_Node *parent;
	_Node *left;
	_Node *right;
	int max;
} Node;

extern Node *NIL;

Node *malloc_node(Interval key);

void free_node(Node *n);

Node* tree_insert(Node* &T, Interval key);

void tree_delete(Node* &T, Node *n);

Node* tree_search(const Node *T, Interval key);

Node* min_interval_search(const Node *T, Interval key);

Node* interval_search(const Node *T, Interval key);

Node* tree_min(const Node *T);

Node* tree_max(const Node *T);

Node* tree_successor(const Node *n);

Node* tree_predecessor(const Node *n);

void inorder_tree_walk(const Node *T);

void print_node(const Node *T);