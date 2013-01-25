typedef enum _NodeColor {
	RED,
	BLACK,
} NodeColor;

typedef struct _Node {
	int key;
	NodeColor color;
	int size;
	_Node *parent;
	_Node *left;
	_Node *right;
}Node;

extern Node *NIL;

Node *malloc_node(int key);

void free_node(Node *n);

Node* tree_insert(Node* &T, int key);

void tree_delete(Node* &T, Node *n);

Node* tree_search(const Node *T, int key);

Node* tree_min(const Node *T);

Node* tree_max(const Node *T);

Node* tree_successor(const Node *n);

Node* tree_predecessor(const Node *n);

void inorder_tree_walk(const Node *T);

void print_node(const Node *T);

Node* os_select(const Node *T, int i);

int os_rank(const Node *T, const Node *n);

int os_key_rank(const Node *T, int key);