/*
Дано число N < 10^6 и последовательность целых чисел из [-2^31..2^31] длиной N.
Требуется построить бинарное дерево, заданное наивным порядком вставки.
Т.е., при добавлении очередного числа K в дерево с корнем root, если root→Key ≤ K, то узел K добавляется в правое поддерево root; иначе в левое поддерево root.
Требования:
	Рекурсия запрещена.
	Решение должно поддерживать передачу функции сравнения снаружи.
*/

#include <iostream>
#include <vector>
#include <stack>
#include <cassert>

using std::vector;
using std::stack;

template<class T, class Compare>
class BinaryTree {
public:
	BinaryTree(const Compare& _cmp) : root(nullptr), cmp(_cmp) { }
	~BinaryTree();

	bool Add(const T& key);
	bool Has(const T& key);

	void DfsInOrder(void (*visit)(const T& key));

private:
	struct Node {
		Node* left;
		Node* right;
		T key;
		Node(const T& key) : left(nullptr), right(nullptr), key(key) { }
	};
	Node* root;
	Compare cmp;

	void postOrderDelete();
};

template<class T, class Compare>
BinaryTree<T, Compare>::~BinaryTree() {
	postOrderDelete();
}


// Так как обход удаляет корень последним, удобно использовать его для деструктора
// Реализация обхода с использванием стека для определения порядка удаления без рекурсии
template<class T, class Compare>
void BinaryTree<T, Compare>::postOrderDelete() {
	stack<Node*> stack;
	Node* node = root;

	while (node != nullptr || !stack.empty()) {
		while (node != nullptr) {
			stack.push(node);
			node = node->left;
		}

		node = stack.top()->right;
		delete stack.top();
		stack.pop();
	}
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::Add(const T& key) {
	Node* currentNode = root;
	Node* previousNode = nullptr;
	bool isLeftPos = true;
	while (currentNode != nullptr && currentNode->key != key) {
		if (cmp(key, currentNode->key)) {
			previousNode = currentNode;
			currentNode = currentNode->left;
			isLeftPos = true;
		}
		else {
			previousNode = currentNode;
			currentNode = currentNode->right;
			isLeftPos = false;
		}
	}

	if (currentNode == nullptr && previousNode == nullptr) {
		Node* newNode = new Node(key);
		root = newNode;
		return true;
	}

	if (currentNode != nullptr && currentNode->key == key) {
		return false;
	}

	Node* newNode = new Node(key);
	if (isLeftPos) {
		previousNode->left = newNode;
	}
	else {
		previousNode->right = newNode;
	}

	return true;
}

template<class T, class Compare>
bool BinaryTree<T, Compare>::Has(const T& key) {
	Node* node = root;
	while (node != nullptr && node->key != key) {
		if (cmp(key, node->key)) {
			node = node->left;
		}
		else {
			node = node->right;
		}
	}

	return node != nullptr;
}

// Реализация обхода с использванием стека для определения порядка удаления без рекурсии
template<class T, class Compare>
void BinaryTree<T, Compare>::DfsInOrder(void(*visit)(const T& key)) {
	stack<Node*> stack;
	Node* node = root;

	while (node != nullptr || !stack.empty()) {
		while (node != nullptr) {
			stack.push(node);
			node = node->left;
		}

		visit(stack.top()->key);
		node = stack.top()->right;
		stack.pop();
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
	int n = 0;
	std::cin >> n; // количество значений на вход
	assert(n <= 10000);

	int value = 0;
	for (int i = 0; i < n; ++i) {
		std::cin >> value;
		tree.Add(value);
	}

	tree.DfsInOrder([](const int& key) { std::cout << key << " "; });

	return 0;
}
