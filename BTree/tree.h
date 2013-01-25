#define MAX_CHILD 5
#define MIN_CHILD 2

typedef struct _Node {
	int key_num;
	int key[MAX_CHILD+1];			// 下标均从1开始
	_Node* child[MAX_CHILD+2];
	bool leaf;
} Node;

extern Node *NIL;

Node *malloc_node();

void free_node(Node *n);

Node* tree_insert(Node* &T, int key);

void tree_delete(Node* &T, Node *n);

Node* tree_search(const Node *T, int key);

Node* tree_min(const Node *T);

Node* tree_max(const Node *T);

Node* tree_successor(const Node *n);

Node* tree_predecessor(const Node *n);

void print_node(const Node *T);