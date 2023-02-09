/*
Постройте B-дерево минимального порядка t и выведите его по слоям.
В качестве ключа используются числа, лежащие в диапазоне 0..232 -1

Требования:
	B-дерево должно быть реализовано в виде шаблонного класса.
	Решение должно поддерживать передачу функции сравнения снаружи.

*/

#include <iostream>
#include <vector>
#include <queue>

using std::queue;
using std::vector;

template<class T, class Compare>
class BTree {
public:
	BTree(size_t t, Compare cmp_) : root(nullptr), _t(t), cmp(cmp_) {}
	~BTree() { delete root; }

	void Insert(const T& key);
	void BfsLevelOrder(void (*visit)(const T& key));
private:
	struct Node {
		bool isLeaf;
		std::vector<T> keys;
		std::vector<Node*> children;

		Node(bool isLeaf) : isLeaf(isLeaf) { }
		~Node() { for (Node* child : children) { delete child; } }
	};

	size_t _t;
	Node* root;
	Compare cmp;

	bool isNodeFull(Node* node) const { return (node->keys.size() == 2 * _t - 1); }
	void insertNonFull(Node* node, const T& key);
	void splitChild(Node* node, int pos);
};

template<class T, class Compare>
void BTree<T, Compare>::Insert(const T& key) {
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

template<class T, class Compare>
void BTree<T, Compare>::insertNonFull(Node* node, const T& key) {
	if (node->isLeaf) {
		node->keys.resize(node->keys.size() + 1);
		int pos = (node->keys.size() < 2)? 0 : node->keys.size() - 2;
		while (pos >= 0 && cmp(key, node->keys[pos])) {
			node->keys[pos + 1] = node->keys[pos];
			--pos;
		}

		if (pos < 0) {
			node->keys[0] = key;
		} else if (pos >= 0 && node->keys.size() != 1 && !cmp(key, node->keys[pos])) {
			node->keys[pos + 1] = key;
		}
		else {
			node->keys[pos] = key;
		}
		
	}
	else {
		int pos = node->keys.size() - 1;
		while (pos >= 0 && cmp(key, node->keys[pos])) {
			--pos;
		}
		if (isNodeFull(node->children[pos + 1])) {
			splitChild(node, pos + 1);
			if (cmp(node->keys[pos + 1], key)) {
				++pos;
			}
		}

		insertNonFull(node->children[pos + 1], key);
	}
}

template<class T, class Compare>
void BTree<T, Compare>::splitChild(Node* node, int pos) {
	Node* splittingNode = node->children[pos];
	Node* newNode = new Node(splittingNode->isLeaf);
	
	newNode->keys.resize(_t - 1);
	for (size_t i = 0; i < _t - 1; ++i) {
		newNode->keys[i] = splittingNode->keys[i + _t];
	}

	T element = splittingNode->keys[_t - 1];
	splittingNode->keys.resize(_t - 1);
	if (!splittingNode->isLeaf) {
		newNode->children.resize(_t);
		for (size_t i = 0; i < _t; ++i) {
			newNode->children[i] = splittingNode->children[i + _t];
		}
		splittingNode->children.resize(_t);
	}

	node->children.resize(node->children.size() + 1);
	for (int i = node->children.size() - 2; i > pos; --i) {
		node->children[i + 1] = node->children[i];
	}
	node->children[pos + 1] = newNode;

	node->keys.resize(node->keys.size() + 1);
	for (int i = node->keys.size() - 1; i > pos; --i) {
		node->keys[i] = node->keys[i - 1];
	}

	node->keys[pos] = element;
}

template<class T, class Compare>
void BTree<T, Compare>::BfsLevelOrder(void (*visit)(const T& key)) {
	queue<Node*> queue;
	queue.push(root);
	queue.push(nullptr);
	int childCount = root->children.size();
	while (!queue.empty()) {
		Node* currentNode = queue.front();
		queue.pop();
		if (currentNode == nullptr) {
			std::cout << std::endl;
			if (!queue.empty()) {
				queue.push(nullptr);
			}

			continue;
		}

		for (size_t i = 0; i < currentNode->keys.size(); ++i) {
			visit(currentNode->keys[i]);
		}

		if (currentNode->isLeaf == false) {
			for (size_t i = 0; i < currentNode->children.size(); ++i) {
				queue.push(currentNode->children[i]);
			}
		}
	}
}

struct CompareInt {
	bool operator()(unsigned int lhs, unsigned int rhs) const {
		return lhs < rhs;
	}
};

int main() {
	int t;
	std::cin >> t;

	CompareInt cmp;
	BTree<unsigned int, CompareInt> tree(t, cmp);
	unsigned int elem;
	while (std::cin >> elem) {
		tree.Insert(elem);
	}

	tree.BfsLevelOrder([](const unsigned int& key) { std::cout << key << " "; });

	return 0;
}
