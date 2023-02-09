#include "Huffman.h"
#include <unordered_map>
#include <queue>
#include <stack>
#include <vector>
#include <string>
#include <bitset>
#include <iostream>

typedef unsigned char byte;
#define interface struct

struct Header {
	unsigned char alphabetSize;
	unsigned char lacuna;
};

struct Node {
	Node* left;
	Node* right;

	unsigned int count;
	unsigned char _byte;

	Node(unsigned char byte_) : _byte(byte_), left(nullptr), right(nullptr), count(0) { }

	Node(Node* left_, Node* right_) : _byte('\0'), left(left_), right(right_), count(0) { }
};

interface IInputStream {
	// Возвращает false, если поток закончился
	virtual bool Read(byte& value) = 0;
};

interface IOutputStream {
	virtual void Write(byte value) = 0;
};

class Writer {
public:
	Writer() : bitsCount(2 * 8), alphabetSize(0) { }
	void Write(unsigned char byte);
	void WriteBit(unsigned char bit);
	void WriteHuffmanTree(Node* node);
	void WriteHeader();
	std::vector<unsigned char> getBuffer() const { return buffer; }

private:
	std::vector<unsigned char> buffer;
	int bitsCount;
	int alphabetSize;
};

void Writer::WriteBit(unsigned char bit) {
	if (bitsCount == 2 * 8) {
		buffer.resize(2);
	}

	if (bitsCount + 1 > buffer.size() * 8) {
		buffer.push_back(0);
	}

	if (bit != 0) {
		int bitPos = bitsCount % 8;
		buffer[buffer.size() - 1] |= 1 << bitPos;
	}

	++bitsCount;
}

void Writer::Write(unsigned char byte) {
	if (bitsCount == 2 * 8) {
		buffer.resize(2);
	}

	if (bitsCount % 8 == 0) {
		buffer.push_back(byte);
	}
	else {
		int buzyBits = bitsCount % 8;
		buffer[buffer.size() - 1] |= byte << buzyBits;
		buffer.push_back(byte >> (8 - buzyBits));
	}

	bitsCount += 8;
}

void Writer::WriteHuffmanTree(Node* node) {
	if (node == nullptr) {
		return;
	}
	else if (node->left == nullptr && node->right == nullptr) {
		WriteBit(1);
		Write(node->_byte);
		alphabetSize += 9;
	}
	else {
		WriteHuffmanTree(node->left);
		WriteHuffmanTree(node->right);
		WriteBit(0);
		++alphabetSize;
	}
}

void Writer::WriteHeader() {
	buffer[0] = alphabetSize;
	buffer[1] = 8 - bitsCount % 8;
}

class Reader {
public:
	Reader(std::vector<unsigned char> buffer_) : _buffer(buffer_), alreadyReadBitCount(0) { 
		Read(_header.alphabetSize);
		Read(_header.lacuna);
	}

	bool Read(unsigned char& byte);
	bool ReadBit(unsigned char& bit);
	void setBuffer(std::vector<unsigned char> buf);
	void ReadHuffmanTree(Node* root);

private:
	Header _header;
	int alreadyReadBitCount = 0;
	std::vector<unsigned char> _buffer;
};

bool Reader::Read(unsigned char& byte) {
	if ((alreadyReadBitCount + 8) / 8 > _buffer.size() - _header.lacuna) {
		return false;
	}

	if (alreadyReadBitCount % 8 == 0) {
		byte = _buffer.at(alreadyReadBitCount / 8);

		alreadyReadBitCount += 8;
	} else {
		byte = 0;
		int bitsCount = 8 - alreadyReadBitCount % 8;
		int remainingBits = alreadyReadBitCount % 8;
		int offset = 0;
		while (bitsCount) {
			unsigned char bit = 0;
			ReadBit(bit);
			byte |= bit << offset++;
			--bitsCount;
		}

		if (alreadyReadBitCount % 8 != 0) {
			return false;
		}

		while (remainingBits) {
			unsigned char bit = 0;
			ReadBit(bit);
			byte |= bit << offset++;
			--remainingBits;
		}
	}

	return true;
}

bool Reader::ReadBit(unsigned char& bit) {
	if ((alreadyReadBitCount + 1) / 8 > _buffer.size() - _header.lacuna) {
		return false;
	}

	int bytePos = alreadyReadBitCount / 8;
	bit = 0;
	bit = _buffer[bytePos] | 1 << (alreadyReadBitCount % 8);

	++alreadyReadBitCount;
	return true;
}

void Reader::ReadHuffmanTree(Node* root) {
	std::stack<Node*> stack;
	unsigned char bit = 0;
	unsigned char counter = 0;
	while (!stack.empty() && counter <= this->_header.alphabetSize && ReadBit(bit)) {
		if (bit == 1) {
			unsigned char byte;
			if (Read(byte)) {
				stack.push(new Node(byte));
				counter += 8;
			} else {
				break;
			}
		} else {
			Node* left = stack.top();
			stack.pop();
			Node* right = stack.top();
			stack.pop();
			root = new Node(left, right);
			stack.push(root);
		}

		++counter;
	}	
}

// Используемый для сравнения и построение очереди класс
struct Compare {
	bool operator()(Node* lhs, Node* rhs) const {
		return (lhs->count > rhs->count);
	}
};

// Построение ноды
Node* getNode(unsigned char byte_, unsigned char count_, Node* left, Node* right) {
	Node* node = new Node(left, right);

	node->_byte = byte_;
	node->count = count_;
	
	return node;
}

// Построение дерева для алгоритма Хаффмана
void buildHuffmanTree(std::vector<unsigned char> input, std::priority_queue<Node*, std::vector<Node*>, Compare>& priorityQueue) {
	std::unordered_map<unsigned char, int> freq;
	for (auto ch : input) {
		freq[ch]++;
	}

	for (auto& pair : freq) {
		priorityQueue.push(getNode(pair.first, pair.second + '0', nullptr, nullptr));
	}

	while (priorityQueue.size() != 1) {
		Node* left = priorityQueue.top();
		priorityQueue.pop();
		Node* right = priorityQueue.top();
		priorityQueue.pop();

		int sum = left->count + right->count;
		priorityQueue.push(getNode('\0', sum, left, right));
	}
}

// Алгоритм кодирования
void encode(Node* root, std::vector<unsigned char> code, std::unordered_map<unsigned char, std::vector<unsigned char>>& huffmanCode) {
	if (root == nullptr)
		return;

	// доходим до листа дерева
	if (!root->left && !root->right) {
		huffmanCode[root->_byte] = code;
	}

	std::vector<unsigned char> leftCode = code;
	leftCode.push_back(0 + '0');

	std::vector<unsigned char> rightCode = code;
	rightCode.push_back(1 + '0');

	encode(root->left, leftCode, huffmanCode);
	encode(root->right, rightCode, huffmanCode);
}

void recSymDecode(Node* root, int& index, const std::vector<unsigned char> binaryCode, unsigned char& sym) {
	if (root == nullptr) {
		return;
	}

	if (!root->left && !root->right) {
		sym = root->_byte;
		return;
	}

	index++;

	if (binaryCode[index] == '0') {
		recSymDecode(root->left, index, binaryCode, sym);
	} else {
		recSymDecode(root->right, index, binaryCode, sym);
	}
}

void decode(Node* root, const std::vector<unsigned char> binaryCode, std::vector<unsigned char>& output) {
	int index = -1;
	int kol = 0;
	while (index < (int)binaryCode.size() - 2) {
		unsigned char sym;
		recSymDecode(root, index, binaryCode, sym);
		output.push_back(sym);
	}
}

void Encode(IInputStream& original, IOutputStream& compressed) {
	std::priority_queue<Node*, std::vector<Node*>, Compare> algQueue;
	std::unordered_map<unsigned char, std::vector<unsigned char>> huffmanCode;
	unsigned char byte = 0;
	std::vector<unsigned char> input;
	while (original.Read(byte)) {
		input.push_back(byte);
	}

	buildHuffmanTree(input, algQueue);
	Node* binaryTreeRoot = algQueue.top();
	Writer writer;
	writer.WriteHuffmanTree(binaryTreeRoot);
	std::vector<unsigned char> coder;
	encode(binaryTreeRoot, coder, huffmanCode);
	for (auto ch : input) {
		std::vector<unsigned char> code = huffmanCode[ch];
		for (unsigned int strI = 0; strI < code.size(); ++strI) {
			writer.WriteBit(code[strI]);
		}
	}

	writer.WriteHeader();
	std::vector<unsigned char> compressedData;
	compressedData = writer.getBuffer();
	for (unsigned char byte : compressedData) {
		compressed.Write(byte);
	}
}

void Decode(IInputStream& compressed, IOutputStream& original) {
	std::vector<unsigned char> compressedData;
	unsigned char byte = 0;
	while (compressed.Read(byte)) {
		compressedData.push_back(byte);
	}

	Reader reader(compressedData);
	Node* binaryTreeRoot = nullptr;
	reader.ReadHuffmanTree(binaryTreeRoot);
	std::vector<unsigned char> outputMessage;
	std::vector<unsigned char> binarySymbols;
	unsigned char sym = 0;
	while (reader.ReadBit(sym)) {
		binarySymbols.push_back(sym);
	}

	decode(binaryTreeRoot, binarySymbols, outputMessage);
	for (unsigned char byte : outputMessage) {
		original.Write(byte);
	}
}


//int main() {
//	Writer input;
//	input.Write('a');
//	input.Write('b');
//	input.Write('c');
//	input.Write('c');
//	input.Write('d');
//	input.WriteBit(1);
//	input.WriteBit(0);
//	input.WriteBit(0);
//	input.WriteBit(1);
//	for (auto& b : input.getBuffer()) {
//		std::cout << std::bitset<8>(b) << "|";
//	}
//	std::cout << std::endl;
//	input.Write(10);
//	for (auto& b : input.getBuffer()) {
//		std::cout << std::bitset<8>(b) << "|";
//	}
//	std::cout << std::endl;
//
//	Reader reader(input.getBuffer());
//	unsigned char byte = 0;
//	while (reader.Read(byte)) {
//		std::cout << std::bitset<8>(byte) << "|";
//	}
//}
