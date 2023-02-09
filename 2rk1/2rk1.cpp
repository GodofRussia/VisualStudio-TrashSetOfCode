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

	void DfsInOrder([&value, &isSimilar](const int& key) {
		if (key != value)
			isSimilar = false;
		}) const;
	T& getRootValue() const { return root->key; }

private:
	struct Node {
		Node* left;
		Node* right;
		T key;
		Node(const T& key) : left(nullptr), right(nullptr), key(key) { }
	};
	void deleteNode(Node* node);

	void dfsInOrder(Node* node, [&value, &isSimilar](const int& key) {
		if (key != value)
			isSimilar = false;
		}) const;
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
void BinaryTree<T, Compare>::DfsInOrder([&value, &isSimilar](const int& key) {
	if (key != value)
		isSimilar = false;
	}) const {
	dfsInOrder(root, visit);
}

template<class T, class Compare>
void BinaryTree<T, Compare>::dfsInOrder(Node* node, ) const {
	if (node == nullptr)
		return;

	dfsInOrder(node->left, visit);
	visit(node->key);
	dfsInOrder(node->right, visit);
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

bool isSimilarValuesTree(const BinaryTree<int, IntCmp>& tree) {
	int value = tree.getRootValue();
	bool isSimilar = true;
	
	tree.DfsInOrder([&value, &isSimilar](const int& key) { 
		if (key != value)
			isSimilar = false;
	});

	return isSimilar;
}

int main() {
	IntCmp cmp;
	BinaryTree<int, IntCmp> tree(cmp);

	int elem;
	while (std::cin >> elem) { }

	std::cout << static_cast<int>(isSimilarValuesTree(tree));
	return 0;
}
