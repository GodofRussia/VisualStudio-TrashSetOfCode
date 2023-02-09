#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>

#define MAX_LENGTH 100

using namespace std;

struct Node {
	char ch;
	int freq;
	Node* left, * right;
};

Node* getNode(char ch, int freq, Node* left, Node* right) {
	Node* node = new Node();

	node->ch = ch;
	node->freq = freq;
	node->left = left;
	node->right = right;

	return node;
}

// Используемый для сравнения и построение очереди класс
struct compare {
	bool operator()(Node* l, Node* r) {
		return (l->freq > r->freq);
	}
};

// заполняет значениями структуру кода Хаффмана с парой символ : строка кодировки
void encode(Node* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
	if (root == nullptr)
		return;

	// доходим до листа дерева
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

void recSymDecode(Node* root, int& index, const std::string binaryCode, char& sym) {
	if (root == nullptr) {
		return;
	}

	if (!root->left && !root->right) {
		sym = root->ch;
		return;
	}

	index++;

	if (binaryCode[index] == '0')
		recSymDecode(root->left, index, binaryCode, sym);
	else
		recSymDecode(root->right, index, binaryCode, sym);
}

void decode(Node* root, const std::string binaryCode, std::string& textMessage) {
	textMessage = "";
	int index = -1;
	int kol = 0;
	while (index < (int) binaryCode.size() - 2) {
		char sym;
		recSymDecode(root, index, binaryCode, sym);
		textMessage += sym;
	}
}

// Построение дерева для алгоритма Хаффмана
void buildHuffmanTree(std::string text, std::priority_queue<Node*, std::vector<Node*>, compare>& priorityQueue) {
	std::unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	for (auto pair : freq) {
		priorityQueue.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (priorityQueue.size() != 1) {
		Node* left = priorityQueue.top();
		priorityQueue.pop();
		Node* right = priorityQueue.top();
		priorityQueue.pop();

		int sum = left->freq + right->freq;
		priorityQueue.push(getNode('\0', sum, left, right));
	}
}

class Source {
private:
	std::string message;

public:
	Source(std::string buffer) : message(buffer) { }
	std::string getString() { return message; }
};

class Coder {
private:
	std::string inputMessage;
	std::string binarySymbols;
	std::priority_queue<Node*, std::vector<Node*>, compare> algQueue;
	std::unordered_map<char, std::string> huffmanCode;
	Node* binaryTreeRoot;

public:
	void setInputData(std::string message) {
		binarySymbols = "";
		inputMessage = message;
	}

	void work() {
		buildHuffmanTree(inputMessage, algQueue);
		binaryTreeRoot = algQueue.top();
		encode(binaryTreeRoot, "", huffmanCode);
		for (char ch : inputMessage) {
			binarySymbols += huffmanCode[ch];
		}
	}

	std::string getBinaryCode() {
		return binarySymbols;
	}

	Node* getBinaryNode() {
		return binaryTreeRoot;
	}

	void countBytes(int& allBytes, int& zeroBytes, int& unitBytes) {
		allBytes = zeroBytes = unitBytes = 0;
		for (int i = 0; i < (int) binarySymbols.size(); ++i) {
			allBytes++;
			(binarySymbols.at(i) == '0') ? zeroBytes++ : unitBytes++;
		}
	}

	int getAverageSymCol() { return binarySymbols.size() / inputMessage.size(); }
};

class SourceCodTransition {
public:
	void send(Source& lhs, Coder& rhs) {
		rhs.setInputData(lhs.getString());
	}
};

class DataTransCanal {
	std::string binarySymbols;
	Node* binaryTreeRoot;

public:
	void setData(std::string binarySymbols, Node* binaryTreeRoot) {
		this->binarySymbols = binarySymbols;
		this->binaryTreeRoot = binaryTreeRoot;
	}

	std::string getBinaryCode() {
		return binarySymbols;
	}

	Node* getBinaryNode() {
		return binaryTreeRoot;
	}
};

class CodCanalTransition {
public:
	void send(Coder& lhs, DataTransCanal& rhs) {
		lhs.work();
		rhs.setData(lhs.getBinaryCode(), lhs.getBinaryNode());
	}
};

class Decoder {
private:
	std::string outputMessage;
	std::string binarySymbols;
	Node* binaryTreeRoot;

public:
	void setData(std::string binarySymbols, Node* binaryTreeRoot) {
		this->binarySymbols = binarySymbols;
		this->binaryTreeRoot = binaryTreeRoot;
	}

	void work() {
		decode(binaryTreeRoot, binarySymbols, outputMessage);
	}

	std::string getOutputStream() { return outputMessage; }
};

class CanalDecoderTransition {
public:
	void send(DataTransCanal& lhs, Decoder& rhs) {
		rhs.setData(lhs.getBinaryCode(), lhs.getBinaryNode());
	}
};

class Acceptor {
	std::string message;

public:
	void setData(std::string messageFrom) {
		message = messageFrom;
	}

	std::string getMessage() { return message; }
};

class DecoderAcceptorTransition {
public:
	void send(Decoder& lhs, Acceptor& rhs) {
		lhs.work();
		rhs.setData(lhs.getOutputStream());
	}
};

int runProg(std::string inputMessage) {
	Source source(inputMessage);
	Coder coder;
	SourceCodTransition trans1;
	trans1.send(source, coder);

	DataTransCanal canal;
	CodCanalTransition trans2;
	trans2.send(coder, canal);

	Decoder decoder;
	CanalDecoderTransition trans3;
	trans3.send(canal, decoder);

	Acceptor acceptor;
	DecoderAcceptorTransition trans4;
	trans4.send(decoder, acceptor);

	int bytesSum, zeroBytes, unitBytes;
	coder.countBytes(bytesSum, zeroBytes, unitBytes);
	std::cout << "Всего двоичных символов передано: " << bytesSum << "\nЕдиничных байт: " << unitBytes << "\nНулевых байт: " << zeroBytes << std::endl;
	std::cout << "Среднее количество двоичных символов для передачи одной буквы: " << coder.getAverageSymCol() << std::endl;

	std::string outputMessage = acceptor.getMessage();
	if (inputMessage == outputMessage)
		return EXIT_SUCCESS;

	return EXIT_FAILURE;
}

std::string readFromFile() {
	ifstream filePtr("input.txt");

	std::string message = "";
	while (!filePtr.eof()) {
		char sym = (char) filePtr.get();
		message += sym;
	}

	return message;
}


int main() {
	setlocale(LC_ALL, "ru");

	if (!runProg("я люблю майнкрафт."))
		std::cout << "Строки совпадают" << std::endl;

	if (!runProg(readFromFile()))
		std::cout << "Строки совпадают" << std::endl;

	return 0;
}
