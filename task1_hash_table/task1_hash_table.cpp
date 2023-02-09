/*
Реализуйте структуру данных типа “множество строк” на основе динамической хеш-таблицы с открытой адресацией. Хранимые строки непустые и состоят из строчных латинских букв.
Хеш-функция строки должна быть реализована с помощью вычисления значения многочлена методом Горнера.
Начальный размер таблицы должен быть равным 8-ми. Перехеширование выполняйте при добавлении элементов в случае, когда коэффициент заполнения таблицы достигает 3/4.
Структура данных должна поддерживать операции добавления строки в множество, удаления строки из множества и проверки принадлежности данной строки множеству.

Для разрешения коллизий используйте двойное хеширование.
*/

#include <vector>
#include <iostream>
#include <cassert>
#include <string>
#include <algorithm>

using std::string;
using std::vector;


struct HornerHasher {
	HornerHasher(int a_ = 127) : a(a_) { }

	unsigned int operator()(const string& str) const {
		unsigned int hash = 0;
		for (unsigned int i = 0; i < str.length(); ++i) {
			hash = hash * a + str[i];
		}

		return hash;
	}

private:
	int a;
};

struct SecondHasher {
	SecondHasher(int a_ = 127) : a(a_) { }

	unsigned int operator()(const string& str) const {
		HornerHasher firstHasher(a);

		return 2 * firstHasher(str) + 1;
	}

private:
	int a;
};

// Основная структура хэш-функции, содержащая внутри двойное хэширование
struct Hasher {
	Hasher(int a_ = 127) : a(a_) { }

	unsigned int operator()(const string& str, unsigned int i = 0) const {
		HornerHasher firstHasher(a);
		SecondHasher secondHasher(a);

		return firstHasher(str) + i * secondHasher(str);
	}

private:
	int a;
};

template<class T, class Hash>
class HashTable {
public:
	HashTable(const Hash& _hasher = Hash());
	~HashTable();

	bool Has(const T& key) const;
	bool Add(const T& key);
	bool Delete(const T& key);

private:
	void grow();
	void rehash();

	struct HashTableNode {
		T Data;
		bool _isDeleted;
		bool _isNil;

		bool isNil() const;
		bool isDeleted() const;
		void setDeleted();
		void setNil();
		HashTableNode() : _isNil(true), _isDeleted(false) {}
		HashTableNode(const T& key) : Data(key), _isDeleted(false), _isNil(false) { }
	};

	Hash hasher;
	vector<HashTableNode> table;
	unsigned int keysCount;  // Реальное количество элементов, лежащих в таблице
	unsigned int nonNullptrKeysCount;  // Общее количество занятых элементов с учётом Deleted
	static const int HashTableDefaultSize = 8;
	static const int MaxFill = 3;
};

template<class T, class Hash>
bool HashTable<T, Hash>::HashTableNode::isNil() const {
	return this->_isNil;
}

template<class T, class Hash>
bool HashTable<T, Hash>::HashTableNode::isDeleted() const {
	return this->_isDeleted;
}

template<class T, class Hash>
void HashTable<T, Hash>::HashTableNode::setDeleted() {
	this->_isDeleted = true;
}

template<class T, class Hash>
void HashTable<T, Hash>::HashTableNode::setNil() {
	this->_isNil = true;
}

template<class T, class Hash>
HashTable<T, Hash>::HashTable(const Hash& _hasher) :
	hasher(_hasher),
	table(HashTableDefaultSize, HashTableNode()),
	keysCount(0),
	nonNullptrKeysCount(0) { }

template<class T, class Hash>
HashTable<T, Hash>::~HashTable() {
	for (unsigned int i = 0; i < table.size(); ++i) {
		if (!table[i].isNil()) {
			table[i].setNil();
		}
	}
}

template<class T, class Hash>
bool HashTable<T, Hash>::Has(const T& key) const {
	for (unsigned int i = 0; i < table.size(); ++i) {
		int hash = hasher(key, i) % table.size();
		if (table[hash].isNil()) {
			return false;
		}
		else if (table[hash].isDeleted()) {
			continue;
		}
		
		if (table[hash].Data == key) {
			return true;
		}
	}

	return false;
}

template<class T, class Hash>
bool HashTable<T, Hash>::Add(const T& key) {
	if (4 * (keysCount + 1) > table.size() * MaxFill) {
		grow();
	}

	HashTableNode* firstDeletedNode = nullptr;
	for (unsigned int i = 0; i < table.size(); ++i) {
		int hash = hasher(key, i) % table.size();
		HashTableNode* node = &table[hash];
		if (node->isNil()) {
			if (firstDeletedNode != nullptr) {
				firstDeletedNode->Data = key;
				firstDeletedNode->_isDeleted = false;
				++keysCount;
				return true;
			}

			HashTableNode* newNode = new HashTableNode(key);
			*node = *newNode;
			++keysCount;
			++nonNullptrKeysCount;
			return true;
		}
		else if (node->isDeleted()) {
			if (firstDeletedNode == nullptr) {
				firstDeletedNode = node;
			}
			else {
				firstDeletedNode->Data = key;
				firstDeletedNode->_isDeleted = false;
				++keysCount;
				return true;
			}

			continue;
		}
		else {
			if (node->Data == key) {
				return false;
			}

			continue;
		}
	}

	return false;
}

template<class T, class Hash>
bool HashTable<T, Hash>::Delete(const T& key) {
	if (4 * (nonNullptrKeysCount - keysCount + 1) > table.size()) {
		rehash();
	}

	for (unsigned int i = 0; i < table.size(); ++i) {
		int hash = hasher(key, i) % table.size();
		if (table[hash].isNil()) {
			return false;
		}
		else if (table[hash].isDeleted()) {
			continue;
		}
		else {
			if (table[hash].Data == key) {
				table[hash].setDeleted();
				--keysCount;
				return true;
			}

			continue;
		}
	}

	return false;
}

//bool hasHash(vector<int>& visitedHashes, int hash) {
//	sort(visitedHashes.begin(), visitedHashes.end());
//	auto left = visitedHashes.cbegin();
//	auto right = visitedHashes.cend() - 1;
//	while (left <= right) {
//		auto mid = left + (right - left) / 2;
//		if (*mid == hash) {
//			return true;
//		}
//		else if (*mid < hash) {
//			left = mid + 1;
//		}
//		else {
//			right = mid - 1;
//		}
//	}
//
//	return false;
//}

template<class T, class Hash>
void HashTable<T, Hash>::grow() {
	/*
	vector<HashTableNode*> newTable(table.size() * 2, nullptr);
	HashTableNode* node;
	vector<int> visitedHashes;
	for (unsigned int i = 0; i < table.size(); ++i) {
		node = table[i];
		if (isNil(node) || isDeleted(node)) {
			continue;
		}
		if (visitedHashes.size() != 0 && hasHash(visitedHashes, hasher(node->Data, 0) % table.size())) {
			continue;
		}
		
		T currentKey = node->Data;
		visitedHashes.push_back(hasher(node->Data, 0) % table.size());
		HashTableNode* currentNode = node;
		for (unsigned int j = 0; j < table.size(); ++j) {
			int hash = hasher(currentKey, j) % table.size();
			HashTableNode* currentNode = table[hash];

			if (isNil(currentNode)) {
				break;
			}
			else if (isDeleted(currentNode)) {
				continue;
			}
			else {
				int newHash = hasher(currentKey, j) % newTable.size();
				newTable[newHash] = currentNode;
			}
		}
	}

	table = newTable;
	*/
	// Код выше должен был также расширять таблицу, так как мы идём по значениям с разными ключами, вышло трудновато.
	// Долго думал и пробовал, вышло придумать хранить значение хеш-функции при нулевой второй функции хеширования, 
	// потом можно найти код текущего элемента внутри вектора с такими значениями, определить, нужно ли его рассматривать или кластер таких значений по первой функции уже прошёл.
	// Кажется, что оно должно так работать, но новые ключи уже получают другие нулевые значения, и всё ломается, происходит добавление по несколько раз.
	// Вероятно, дело в хэш функциях, вторая всегда выводит нечётное число при четном размере таблицы.
	// Надеюсь, что вы мне проясните по возможности, как это заставить работать, а то интересно :)

	nonNullptrKeysCount = 0;
	keysCount = 0;
	int past_size = table.size();
	vector<HashTableNode> newTable(table.size() * 2, HashTableNode());
	std::swap(table, newTable);
	for (int i = 0; i < past_size; ++i) {
		if (!newTable[i].isNil() && !newTable[i]._isDeleted)
			Add(newTable[i].Data);
	}

	newTable.clear();
}

template<class T, class Hash>
void HashTable<T, Hash>::rehash() {
	nonNullptrKeysCount = 0;
	keysCount = 0;
	int past_size = table.size();
	vector<HashTableNode> newTable(table.size(), HashTableNode());
	std::swap(table, newTable);
	for (int i = 0; i < past_size; ++i) {
		if (!newTable[i].isNil() && !newTable[i]._isDeleted)
			Add(newTable[i].Data);
	}

	newTable.clear();
}

int main() {
	int a = 127;
	Hasher hasher(a);
	HashTable<string, Hasher> table(hasher);
	char operation = 0;
	string word;
	while (std::cin >> operation >> word) {
		if (operation == '+') {
			std::cout << (table.Add(word) ? "OK" : "FAIL") << std::endl;
		}
		if (operation == '-') {
			std::cout << (table.Delete(word) ? "OK" : "FAIL") << std::endl;
		}
		if (operation == '?') {
			std::cout << (table.Has(word) ? "OK" : "FAIL") << std::endl;
		}
	}

	return 0;
}

// Has
// Empty - return false;
// Deleted - идём дальше;
// Key - если искомый - return true, иначе идём дальше

// Add
// Empty - записываем Key ( в первый запомненный Deleted, если есть, либо сюда) и return true;
// Deleted - если первый Deleted - запоминаем, иначе идём дальше;
// Key - если добавляемый - return false, иначе идём дальше.

// Delete
// Empty - return false;
// Deleted - идём дальше;
// Key - если удаляемый - ставим Deleted и return true, иначе идём дальше;

// + два перехеширования
// При большом количестве ключей (3/4) с увеличением размера таблицы
// При большом количестве Deleted (1/4) с сохранением размера таблицы
