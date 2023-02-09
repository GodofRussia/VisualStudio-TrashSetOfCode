#include <iostream>
#include <vector>

using std::vector;

template<class T, class Compare>
class BinaryTree {
public:
	BinaryTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) { }
	~BinaryTree();
	
	void Add(const T& key);
	bool Has(const T& key);

	void DfsInOrder(void (*visit)(const T& key));
	void DfsPreOrder(void (*visit)(const T& key));
	void DfsPostOrder(void (*visit)(const T& key));

private:
	struct Node {
		Node* left;
		Node* right;
		T key;
		Node(const T& key) : left(nullptr), right(nullptr), key(key) { }
	};
	void deleteNode(Node* node);

	void dfsInOrder(Node* node, void (*visit)(const T& key));
	void dfsPreOrder(Node* node, void (*visit)(const T& key));
	void dfsPostOrder(Node* node, void (*visit)(const T& key));

	void recursiveAdd(Node*& node, const T& key);

	Node* root;
	Compare cmp;
};

template<class T, class Compare>
void BinaryTree<T, Compare>::deleteNode(Node* node) {
	if (node == nullptr)
		return;

	deleteNode(node->left);
	deleteNode(node->right);
	delete node;
}

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
	deleteNode(root);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::Add(const T& key) {
	recursiveAdd(root, key);
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::Has(const T& key) {
	Node* node = root;
	while (node != nullptr && node->key != key) {
		if (cmp(key, node->key)) {
			node = node->left;
		}
		else {
			node = node->Right;
		}
	}

	return node != nullptr;
}

template<class T, class Compare>
void BinaryTree<T, Compare>::DfsInOrder(void(*visit)(const T& key)) {
	dfsInOrder(root, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::DfsPreOrder(void(*visit)(const T& key))
{
	dfsPreOrder(root, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::DfsPostOrder(void(*visit)(const T& key))
{
	dfsPostOrder(root, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::dfsInOrder(Node* node, void(*visit)(const T& key))
{
	if (node == nullptr)
		return;

	dfsInOrder(node->left, visit);
	visit(node->key);
	dfsInOrder(node->right, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::dfsPreOrder(Node* node, void(*visit)(const T& key)) {
	if (node == nullptr)
		return;

	visit(node->key);
	dfsPreOrder(node->left, visit);
	dfsPreOrder(node->right, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::dfsPostOrder(Node* node, void(*visit)(const T& key)) {
	if (node == nullptr)
		return;

	dfsPostOrder(node->left, visit);
	dfsPostOrder(node->right, visit);
	visit(node->key);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::recursiveAdd(Node*& node, const T& key) {
	if (node == nullptr) {
		node = new Node(key);
		return;
	}

	if (cmp(key, node->key)) {
		recursiveAdd(node->left, key);
	}
	else {
		recursiveAdd(node->right, key);
	}
}

struct IntCmp {
	bool operator()(int l, int r) const {
		return l < r;
	}
};

int main() {
	IntCmp cmp;
	BinaryTree<int, IntCmp> tree(cmp);
	tree.Add(20);
	tree.Add(10);
	tree.Add(30);
	tree.Add(6);
	tree.Add(19);
	tree.Add(40);
	tree.Add(50);
	tree.DfsPreOrder([](const int& key) { std::cout << key << " "; });
	std::cout << "\n\n";

	tree.DfsInOrder([](const int& key) { std::cout << key << " "; });
	std::cout << "\n\n";

	tree.DfsPostOrder([](const int& key) { std::cout << key << " "; });
	std::cout << "\n\n";
}

