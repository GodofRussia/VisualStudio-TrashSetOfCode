/*
Дано число N и N строк. Каждая строка содержит команду добавления или удаления натуральных чисел, а также запрос на получение k-ой порядковой статистики.
Команда добавления числа A задается положительным числом A, команда удаления числа A задается отрицательным числом “-A”.
Запрос на получение k-ой порядковой статистики задается числом k.
Требования: скорость выполнения запроса - O(log n).
*/

#include <iostream>
#include <utility>

using std::pair;

template <typename T, class Compare>
class AVLTree {
public:
	AVLTree(Compare cmp_);
	~AVLTree();
	void insert(const T& key);
	void remove(const T& key);
	T findKStat(unsigned int stat);
	void DFS();

private:
	struct AVLTreeNode {
		T key;
		unsigned char height;
		unsigned int nodeCount;
		AVLTreeNode* left;
		AVLTreeNode* right;

		AVLTreeNode(T k) : key(k), left(nullptr), right(nullptr), height(1), nodeCount(1) { }
	};

	AVLTreeNode* root;
	Compare cmp;

	unsigned char height(AVLTreeNode* node) { return node ? node->height : 0; }
	unsigned int count(AVLTreeNode* node) { return node ? node->nodeCount : 0; }
	unsigned char balanceFactor(AVLTreeNode* node) { return height(node->right) - height(node->left); }
	void fixHeight(AVLTreeNode* node);
	void fixCount(AVLTreeNode* node);

	AVLTreeNode* rotateRight(AVLTreeNode* node);
	AVLTreeNode* rotateLeft(AVLTreeNode* node);
	AVLTreeNode* balance(AVLTreeNode* node);

	void destroy(AVLTreeNode* node);

	AVLTreeNode* innerInsert(AVLTreeNode* node, const T& key);

	void innerDFS(AVLTreeNode* node);

	pair<AVLTreeNode*, AVLTreeNode*> findAndExtractMin(AVLTreeNode* node);
	AVLTreeNode* innerRemove(AVLTreeNode* node, const T& key);

	AVLTreeNode* findKStat(AVLTreeNode* node, unsigned int stat);
};

template <typename T, class Compare>
void AVLTree<T, Compare>::DFS() {
	innerDFS(root);
}

template <typename T, class Compare>
void AVLTree<T, Compare>::innerDFS(AVLTreeNode* node) {
	if (node == nullptr) {
		return;
	}

	innerDFS(node->left);
	std::cout << node->key << " ";
	innerDFS(node->right);
}

template <typename T, class Compare>
void AVLTree<T, Compare>::fixHeight(AVLTreeNode* node) {
	unsigned char hl = height(node->left);
	unsigned char hr = height(node->right);
	node->height = std::max(hl, hr) + 1;
}

template <typename T, class Compare>
void AVLTree<T, Compare>::fixCount(AVLTreeNode* node) {
	unsigned int cl = count(node->left);
	unsigned int cr = count(node->right);
	node->nodeCount = cl + cr + 1;
}

template <typename T, class Compare>
AVLTree<T, Compare>::AVLTree(Compare cmp_) : cmp(cmp_) {
	root = nullptr;
}

template <typename T, class Compare>
void AVLTree<T, Compare>::destroy(AVLTreeNode* node) {
	if (node == nullptr) {
		return;
	}

	destroy(node->left);
	destroy(node->right);
	delete node;
}

template <typename T, class Compare>
AVLTree<T, Compare>::~AVLTree() {
	destroy(root);
}

template <typename T, class Compare>
typename AVLTree<T, Compare>::AVLTreeNode* AVLTree<T, Compare>::rotateRight(AVLTreeNode* node) {
	AVLTreeNode* leftNode = node->left;
	node->left = leftNode->right;
	leftNode->right = node;

	fixHeight(node);
	fixHeight(leftNode);
	fixCount(node);
	fixCount(leftNode);

	return leftNode;
}


template <typename T, class Compare>
typename AVLTree<T, Compare>::AVLTreeNode* AVLTree<T, Compare>::rotateLeft(AVLTreeNode* node) {
	AVLTreeNode* rightNode = node->right;
	node->right = rightNode->left;
	rightNode->left = node;

	fixHeight(node);
	fixHeight(rightNode);
	fixCount(node);
	fixCount(rightNode);

	return rightNode;
}

template <typename T, class Compare>
typename AVLTree<T, Compare>::AVLTreeNode* AVLTree<T, Compare>::balance(AVLTreeNode* node) {
	fixHeight(node);
	fixCount(node);
	if (balanceFactor(node) == 2) {
		if (balanceFactor(node->right) < 0) {
			node->right = rotateRight(node->right);
		}
		return rotateLeft(node);
	}
	if (balanceFactor(node) == -2) {
		if (balanceFactor(node->left) > 0) {
			node->left = rotateLeft(node->left);
		}
		return rotateRight(node);
	}

	return node;
}

template <typename T, class Compare>
void AVLTree<T, Compare>::insert(const T& key) {
	root = innerInsert(root, key);
}

template <typename T, class Compare>
typename AVLTree<T, Compare>::AVLTreeNode* AVLTree<T, Compare>::innerInsert(AVLTreeNode* node, const T& key) {
	if (node == nullptr) {
		return new AVLTreeNode(key);
	}

	if (cmp(key, node->key)) {
		node->left = innerInsert(node->left, key);
	} else {
		node->right = innerInsert(node->right, key);
	}

	return balance(node);
}

template <typename T, class Compare>
pair<typename AVLTree<T, Compare>::AVLTreeNode*, typename AVLTree<T, Compare>::AVLTreeNode*>
AVLTree<T, Compare>::findAndExtractMin(AVLTreeNode* node) {
	if (node->left == nullptr) {
		return std::make_pair(node, node->right);
	}

	pair<AVLTreeNode*, AVLTreeNode*> currentMinAndRightOfMinPair = findAndExtractMin(node->left);
	node->left = currentMinAndRightOfMinPair.second;

	return std::make_pair(currentMinAndRightOfMinPair.first, balance(node));
}

template <typename T, class Compare>
void AVLTree<T, Compare>::remove(const T& key) {
	root = innerRemove(root, key);
}

template <typename T, class Compare>
typename AVLTree<T, Compare>::AVLTreeNode* AVLTree<T, Compare>::innerRemove(AVLTreeNode* node, const T& key) {
	if (node == nullptr) {
		return nullptr;
	}

	if (cmp(key, node->key)) {
		node->left = innerRemove(node->left, key);
	} else if (cmp(node->key, key)) {
		node->right = innerRemove(node->right, key);
	} else {
		// С данной логикой мы контролируем и случай с 2 детьми и случаи с 0-1
		AVLTreeNode* leftNode = node->left;
		AVLTreeNode* rightNode = node->right;
		if (node == root) {
			delete node;
			node = nullptr;
			root = nullptr;
		} else {
			delete node;
			node = nullptr;
		}
		
		if (rightNode == nullptr) {
			return leftNode;
		}

		AVLTreeNode* rightOfMin = rightNode->right;
		pair<AVLTreeNode*, AVLTreeNode*> minAndRightOfMinPair = findAndExtractMin(rightNode);
		AVLTreeNode* min = minAndRightOfMinPair.first;
		// Переносим минимальный элемент
		min->right = minAndRightOfMinPair.second;
		min->left = leftNode;

		return balance(min);
	}

	return balance(node);
}

template <typename T, class Compare>
T AVLTree<T, Compare>::findKStat(unsigned int stat) {
	return findKStat(root, stat)->key;
}

template <typename T, class Compare>
typename AVLTree<T, Compare>::AVLTreeNode* AVLTree<T, Compare>::findKStat(AVLTreeNode* node, unsigned int stat) {
	// Тут не проверяем наличие статистики, поскольку это гарантируется, не оговорено, что делать в ином случае 
	unsigned int k = count(node->left);
	if (k == stat) {
		return node;
	} else if (k < stat) {
		return findKStat(node->right, stat - k - 1);
	}

	return findKStat(node->left, stat);
}

struct IntCmp {
	bool operator()(unsigned int lhs, unsigned int rhs) const {
		return lhs < rhs;
	}
};

int main() {
	unsigned int N;
	std::cin >> N;
	IntCmp cmp;
	AVLTree<unsigned int, IntCmp> tree(cmp);

	int element = 0;
	unsigned int statistic = 0;
	for (unsigned int i = 0; i < N; ++i) {
		std::cin >> element >> statistic;
		// Так как отрицательные элементы обозначают удаление, то инвертируем их знак и кастим к беззнаковому (тут расширение гарантируется)
		(element >= 0)? tree.insert(static_cast<unsigned int>(element)) : tree.remove(static_cast<unsigned int>(element * -1));
		std::cout << tree.findKStat(statistic) << std::endl;
	}

	tree.DFS();

	return 0;
}
