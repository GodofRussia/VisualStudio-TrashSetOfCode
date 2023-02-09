#include <iostream>
#include <vector>

template<class T>
class BTree {
public:
	BTree(size_t t) : root(nullptr), t(_t) {}
	~BTree() { delete root; }

	void Insert(const T& key);
private:
	struct Node{
		bool isLeaf;
		std::vector<T>* keys;
		std::vector<Node*> children;

		Node(bool isLeaf) : isLeaf(isLeaf) { }
		~Node() { for (Node* child : children) { delete child; } }
	}

	void insertNonFull(Node* node, const T& key);
	size_t _t;
	Node* root
};

template<class T>
void BTree<T>::Insert(const T& key) {
	if (root == nullptr) {
		root = new Node(true);
	}

	if (isNodeFull(root)) {
		Node* newRoot = new Node(false);
		newRoot->children.push_back(root);
		root = newRoot;
		splitChild(root, 0);
	}

	insertNonFull(root, key);
}

template<class T>
void BTree<T>::insertNonFull(Node* node, const T& key) {
	if (node->isLeaf) {
		node->keys.resize(node->keys.size() + 1);
		int pos = node->keys.size() - 1;
		while (pos >= 0)
	}
}
