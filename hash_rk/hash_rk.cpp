#include <iostream>
#include <unordered_set>
#include <string>
#include <functional>

using std::string;

struct Node {
	std::string product_name;
	int color;
	int size;

	size_t hash() const {
		size_t hash = 0;
		for (unsigned int i = 0; i < product_name.length(); ++i) {
			hash = hash * 127 + std::hash<char>()(product_name[i]) + std::hash<int>()(color) + std::hash<int>()(size);
		}

		return hash;
	}

	bool operator==(const Node& rhs) const {
		return (!static_cast<bool>(product_name.compare(rhs.product_name)) && color == rhs.color && size == rhs.size);
	}

	Node(std::string product_name_, int color_, int size_) : product_name(product_name_), color(color_), size(size_) { }
};

bool hasElement(const std::unordered_set<Node, std::function<size_t(const Node& node)>>& table, Node node) {
	return (table.find(node) != table.end());
}

int main() {
	int a = 127; 
	std::unordered_set<Node, std::function<size_t(const Node& node)>> hashTable(100500, 
		[](const Node& node) {
			return node.hash();
	});

	char operation = 0;
	string name;
	int color; int size;
	while (std::cin >> operation >> name >> color >> size) {
		Node node(name, color, size);
		if (operation == '+') {
			if (hashTable.find(node) != hashTable.end()) {
				std::cout << "FAIL" << std::endl;
			}
			else {
				hashTable.insert(node);
				std::cout << "OK" << std::endl;
			}
		}
		if (operation == '-') {
			if (!(hashTable.find(node) != hashTable.end())) {
				std::cout << "FAIL" << std::endl;
			}
			else {
				hashTable.erase(node);
			}
			std::cout << "OK" << std::endl;
		}
		if (operation == '?') {
			std::cout << (hashTable.find(node) != hashTable.end() ? "OK" : "FAIL") << std::endl;
		}
	}

	return 0;
}
