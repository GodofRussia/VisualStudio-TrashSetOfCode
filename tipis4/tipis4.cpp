﻿#include <iostream>
#include <fstream>
#include <string>
#include <string.h>
#include <queue>
#include <unordered_map>
#include <map>
#include <array>
#include <vector>
#include <cstdlib>
#include <ctime>

#define MAX_LENGTH 100

using namespace std;

struct Node {
	std::string ch;
	int freq;
	Node* left, * right;
};

Node* getNode(std::string ch, int freq, Node* left, Node* right) {
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
void encode(Node* root, std::string str, std::unordered_map<std::string, std::string>& huffmanCode) {
	if (root == nullptr)
		return;

	// доходим до листа дерева
	if (!root->left && !root->right) {
		huffmanCode[root->ch] = str;
	}

	encode(root->left, str + "0", huffmanCode);
	encode(root->right, str + "1", huffmanCode);
}

void recSymDecode(Node* root, int& index, const std::string binaryCode, std::string& sym) {
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
	while (index < (int)binaryCode.size() - 2) {
		std::string sym;
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

	for (auto& pair : freq) {
		priorityQueue.push(getNode((std::string(1, pair.first)), pair.second, nullptr, nullptr));
	}

	while (priorityQueue.size() != 1) {
		Node* left = priorityQueue.top();
		priorityQueue.pop();
		Node* right = priorityQueue.top();
		priorityQueue.pop();

		int sum = left->freq + right->freq;
		priorityQueue.push(getNode("\0", sum, left, right));
	}
}

// Построение дерева для алгоритма Хаффмана с равновероятной кодировкой
void buildEqHuffmanTree(std::string text, std::priority_queue<Node*, std::vector<Node*>, compare>& priorityQueue) {
	std::unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch];
	}

	for (auto& pair : freq) {
		priorityQueue.push(getNode((std::string(1, pair.first)), pair.second, nullptr, nullptr));
	}

	while (priorityQueue.size() != 1) {
		Node* left = priorityQueue.top();
		priorityQueue.pop();
		Node* right = priorityQueue.top();
		priorityQueue.pop();

		int sum = left->freq + right->freq;
		priorityQueue.push(getNode("\0", sum, left, right));
	}
}

// Построение дерева для алгоритма Хаффмана с блочной кодировкой
void buildblockHuffmanTree(std::string text, std::priority_queue<Node*, std::vector<Node*>, compare>& priorityQueue) {
	std::unordered_map<char, int> freq;
	for (char ch : text) {
		freq[ch]++;
	}

	std::unordered_map<std::string, int> blockFreq;
	for (auto lIter = freq.cbegin(); lIter != freq.cend(); ++lIter) {
		blockFreq[string(1, lIter->first) + string(1, lIter->first)] = lIter->second * lIter->second;
		auto rIter = lIter;
		++rIter;
		for (; rIter != freq.cend(); ++rIter) {
			blockFreq[string(1, lIter->first) + string(1, rIter->first)] = lIter->second * rIter->second;
			blockFreq[string(1, rIter->first) + string(1, lIter->first)] = lIter->second * rIter->second;
		}
	}

	if (text.size() % 2) {
		blockFreq[string(1, text.at(text.size() - 1))];
	}

	for (auto pair : blockFreq) {
		priorityQueue.push(getNode(pair.first, pair.second, nullptr, nullptr));
	}

	while (priorityQueue.size() != 1) {
		Node* left = priorityQueue.top();
		priorityQueue.pop();
		Node* right = priorityQueue.top();
		priorityQueue.pop();

		int sum = left->freq + right->freq;
		priorityQueue.push(getNode("\0", sum, left, right));
	}
}

// Функция инвертирования бит строки с некоторой вероятностью
int makeNoise(std::string& binarySymbols, double chan) {
	size_t i = 0;
	int noiseCount = 0;
	int chance = static_cast<int>(chan * 100);
	srand(time(0));
	while (i < binarySymbols.size()) {
		int bitValue = (rand() % 100) / (100 - chance);
		if (bitValue) {
			binarySymbols[i] = static_cast<char>(~static_cast<int>(binarySymbols[i]));
			++noiseCount;
		}

		++i;
	}

	return noiseCount;
}

class Source {
private:
	std::string message;
	double invChance_;

public:
	Source(std::string buffer, double inversionChance) : message(buffer), invChance_(inversionChance) { }
	std::string getString() { return message; }
	double getNoiseChance() { return invChance_; }
};

class Coder {
private:
	std::string inputMessage;
	std::string binarySymbols;
	std::priority_queue<Node*, std::vector<Node*>, compare> algQueue;
	std::unordered_map<std::string, std::string> huffmanCode;
	Node* binaryTreeRoot;
	double chance_;

public:
	void setInputData(std::string message, double chance) {
		binarySymbols = "";
		inputMessage = message;
		chance_ = chance;
	}

	void equivalentWork() {
		buildEqHuffmanTree(inputMessage, algQueue);
		binaryTreeRoot = algQueue.top();
		encode(binaryTreeRoot, "", huffmanCode);
		for (auto ch : inputMessage) {
			binarySymbols += huffmanCode[string(1, ch)];
		}
	}

	void blockWork() {
		buildblockHuffmanTree(inputMessage, algQueue);
		binaryTreeRoot = algQueue.top();
		encode(binaryTreeRoot, "", huffmanCode);
		size_t i = 0;
		for (i = 0; i < inputMessage.size() - 1; i += 2) {
			binarySymbols += huffmanCode[inputMessage.substr(i, 2)];
		}

		if (i != inputMessage.size())
			binarySymbols += huffmanCode[string(1, inputMessage.at(inputMessage.size() - 1))];
	}

	void defWork() {
		buildHuffmanTree(inputMessage, algQueue);
		binaryTreeRoot = algQueue.top();
		encode(binaryTreeRoot, "", huffmanCode);
		for (auto ch : inputMessage) {
			binarySymbols += huffmanCode[string(1, ch)];
		}
	}

	std::string getBinaryCode() {
		return binarySymbols;
	}

	Node* getBinaryNode() {
		return binaryTreeRoot;
	}

	double getChance() {
		return chance_;
	}

	std::unordered_map<std::string, std::string> getHuffmanCode() {
		return huffmanCode;
	}

	void countBytes(int& allBytes, int& zeroBytes, int& unitBytes) {
		allBytes = zeroBytes = unitBytes = 0;
		for (int i = 0; i < (int)binarySymbols.size(); ++i) {
			allBytes++;
			(binarySymbols.at(i) == '0') ? zeroBytes++ : unitBytes++;
		}
	}

	int getAverageSymCol() { return binarySymbols.size() / inputMessage.size(); }
};

class NoiseResistanceCoder {
public:
	void work() {
		encodeUsingMatrix();
	}

	void setData(std::string code, double chance, Node* binaryTreeRoot) {
		binarySymbols = code;
		chance_ = chance;
		this->binaryTreeRoot = binaryTreeRoot;
	}

	double getChance() {
		return chance_;
	}

	Node* getBinaryNode() {
		return binaryTreeRoot;
	}

	std::string getBinarySymbols() { return binarySymbols; }
	std::string getMatrixBinarySymbols() { return fromMatrixBinarySymbols; }

private:
	void encodeUsingMatrix();

	std::string binarySymbols;
	Node* binaryTreeRoot;
	double chance_;
	int matrix[4][7] = {
		{1, 0, 0, 0, 1, 1, 1},
		{0, 1, 0, 0, 1, 1, 0},
		{0, 0, 1, 0, 1, 0, 1},
		{0, 0, 0, 1, 0, 1, 1}
	};
	string fromMatrixBinarySymbols;
};

void NoiseResistanceCoder::encodeUsingMatrix() {
	std::string codeString = "";
	for (size_t i = 4; i <= binarySymbols.size(); i += 4) {
		std::string code7by4 = "";
		for (int k = 0; k < 7; ++k) {
			int sum = 0;

			for (int j = 0; j < 4; ++j) {
				sum += matrix[j][k] * (binarySymbols.at(i - 4 + j) - '0');
			}

			sum = sum % 2;
			code7by4 += to_string(sum);
		}

		codeString += code7by4;
	}

	fromMatrixBinarySymbols = codeString;
}

class SourceCodTransition {
public:
	void send(Source& lhs, Coder& rhs) {
		rhs.setInputData(lhs.getString(), lhs.getNoiseChance());
	}
};

class CodNoiseCodTransition {
public:
	void send1(Coder& lhs, NoiseResistanceCoder& rhs) {
		lhs.defWork();
		rhs.setData(lhs.getBinaryCode(), lhs.getChance(), lhs.getBinaryNode());
	}

	void send2(Coder& lhs, NoiseResistanceCoder& rhs) {
		lhs.blockWork();
		rhs.setData(lhs.getBinaryCode(), lhs.getChance(), lhs.getBinaryNode());
	}

	void send3(Coder& lhs, NoiseResistanceCoder& rhs) {
		lhs.equivalentWork();
		rhs.setData(lhs.getBinaryCode(), lhs.getChance(), lhs.getBinaryNode());
	}
};

class DataTransCanal {
	std::string binarySymbols;
	Node* binaryTreeRoot;
	double _chance;
	string fromMatrixBinarySymbols;
	int errorCount = 0;

public:
	void setData(std::string binarySymbols, Node* binaryTreeRoot, double _chance, string matrix) {
		this->binarySymbols = binarySymbols;
		this->binaryTreeRoot = binaryTreeRoot;
		this->_chance = _chance;
		fromMatrixBinarySymbols = matrix;
	}

	std::string getBinaryCode() {
		makeNoise(binarySymbols, _chance);

		return binarySymbols;
	}

	Node* getBinaryNode() {
		return binaryTreeRoot;
	}

	int getErrorBitCount() {
		return errorCount;
	}

	std::string getMatrix() {
		errorCount = makeNoise(fromMatrixBinarySymbols, _chance);

		return fromMatrixBinarySymbols;
	}
};

class NoiseCodCanalTransition {
public:
	void send(NoiseResistanceCoder& lhs, DataTransCanal& rhs) {
		lhs.work();
		rhs.setData(lhs.getBinarySymbols(), lhs.getBinaryNode(), lhs.getChance(), lhs.getMatrixBinarySymbols());
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

//template<class T, >
//bool operator==(const int* lhs,
//	const int* rhs) {
//	if (lhs.size() != rhs.size())
//		return false;
//
//	for (size_t i = 0; i < lhs.size(); ++i) {
//		if (lhs[i] != rhs[i])
//			return false;
//	}
//
//	return true;
//}

class NoiseResistanceDeCoder {
public:
	NoiseResistanceDeCoder() {
		std::array<int, 3> sindrom0 = { 1, 0, 0 };
		table.emplace(std::make_pair(sindrom0, 0));

		std::array<int, 3> sindrom1 = { 0, 1, 0 };
		table.emplace(std::make_pair(sindrom1, 1));

		std::array<int, 3> sindrom2 = { 0, 0, 1 };
		table.emplace(std::make_pair(sindrom2, 2));

		std::array<int, 3> sindrom3 = { 1, 1, 0 };
		table.emplace(std::make_pair(sindrom3, 3));

		std::array<int, 3> sindrom4 = { 0, 1, 1 };
		table.emplace(std::make_pair(sindrom4, 4));

		std::array<int, 3> sindrom5 = { 1, 1, 1 };
		table.emplace(std::make_pair(sindrom5, 5));

		std::array<int, 3> sindrom6 = { 1, 0, 1 };
		table.emplace(std::make_pair(sindrom6, 6));
	}

	bool work() {
		return decodeUsingMatrix();
	}

	void setData(std::string code, string matrix, Node* root) {
		binarySymbols = code;
		fromMatrixBinarySymbols = matrix;
		binaryTreeRoot = root;
	}

	int getFixedBitCount() {
		return fixedBitCount;
	}

	std::string getBinarySymbols() { return binarySymbols; }
	std::string getMatrix() { return fromMatrixBinarySymbols; }
	Node* getRoot() { return binaryTreeRoot; }

private:
	bool decodeUsingMatrix();
	Node* binaryTreeRoot;
	std::string binarySymbols;
	int matrix[7][3] = {
		{ 1, 1, 1 },
		{ 1, 1, 0 },
		{ 1, 0, 1 },
		{ 0, 1, 1 },
		{ 1, 0, 0 },
		{ 0, 1, 0 },
		{ 0, 0, 1 }
	};

	std::map<std::array<int, 3>, int> table;
	string fromMatrixBinarySymbols;

	int fixedBitCount = 0;
};

bool NoiseResistanceDeCoder::decodeUsingMatrix() {
	for (size_t i = 0; i < fromMatrixBinarySymbols.size(); i += 7) {
		std::array<int, 3> sindrom;
		bool error = false;
		int invalidBit = -1;
		for (int k = 0; k < 3; ++k) {
			int sum = 0;
			for (int j = 0; j < 7; ++j) {
				sum += matrix[j][k] * (fromMatrixBinarySymbols.at(i + j) - '0');
			}

			if (sum % 2) error = true;
			sindrom[k] = abs((sum % 2));
		}

		if (error) {
			invalidBit = table[sindrom];
			if (fromMatrixBinarySymbols[i + invalidBit] == '0') {
				fromMatrixBinarySymbols[i + invalidBit] = '1';
			} else {
				fromMatrixBinarySymbols[i + invalidBit] = '0';
			}
			++fixedBitCount;
		}
	}

	return true;
}

class CanalNoiseDecoderTransition {
public:
	void send(DataTransCanal& lhs, NoiseResistanceDeCoder& rhs) {
		rhs.setData(lhs.getBinaryCode(), lhs.getMatrix(), lhs.getBinaryNode());
	}
};



class NoiseDecoderDecoderTransition {
public:
	bool send(NoiseResistanceDeCoder& lhs, Decoder& rhs) {
		bool status = lhs.work();
		rhs.setData(lhs.getBinarySymbols(), lhs.getRoot());

		return status;
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

std::string readFromFile() {
	ifstream filePtr("input.txt");

	std::string message = "";
	while (!filePtr.eof()) {
		char sym = (char)filePtr.get();
		message += sym;
	}

	return message;
}

void writeInFile(std::string outputText) {
	ofstream filePtr("output.txt");

	filePtr << outputText;
}

void writeInCodeFile(std::string codeText) {
	ofstream filePtr("code.txt");

	filePtr << codeText;
}

void writeCoderFile(std::unordered_map<std::string, std::string> coder) {
	ofstream filePtr("coder.txt");

	for (auto iter = coder.cbegin(); iter != coder.cend(); ++iter) {
		filePtr << iter->first << " - " << iter->second << std::endl;
	}
}

int runProg(std::string inputMessage, double noiseChance, int command) {
	Source source(inputMessage, noiseChance);
	Coder coder;
	SourceCodTransition trans1;
	trans1.send(source, coder);

	NoiseResistanceCoder coder2;
	NoiseResistanceDeCoder decoder2;

	DataTransCanal canal;
	CodNoiseCodTransition trans2;
	switch (command) {
	case 0:
		trans2.send1(coder, coder2);
		break;
	case 1:
		trans2.send2(coder, coder2);
		break;
	case 2:
		trans2.send3(coder, coder2);
		break;
	default:
		std::cout << "Неверная команда" << std::endl;
		return EXIT_FAILURE;
	}

	Decoder decoder;
	NoiseCodCanalTransition trans3;
	trans3.send(coder2, canal);
	CanalNoiseDecoderTransition trans4;
	trans4.send(canal, decoder2);
	NoiseDecoderDecoderTransition trans5;

	Acceptor acceptor;
	DecoderAcceptorTransition trans6;
	bool status = trans5.send(decoder2, decoder);
	trans6.send(decoder, acceptor);

	int bytesSum, zeroBytes, unitBytes;
	coder.countBytes(bytesSum, zeroBytes, unitBytes);
	switch (command) {
	case 0:
		std::cout << "Стандартное посимвольное кодирование" << std::endl;
		break;
	case 1:
		std::cout << "Блочное кодирование" << std::endl;
		break;
	case 2:
		std::cout << "Равновероятное кодирование" << std::endl;
		break;
	}

	std::cout << "Всего двоичных символов передано: " << bytesSum << "\nЕдиничных байт: " << unitBytes << "\nНулевых байт: " << zeroBytes << std::endl;
	std::cout << "Среднее количество двоичных символов для передачи одной буквы: " << coder.getAverageSymCol() << std::endl;
	std::cout << "Количество ошибок возникших в канале: " << canal.getErrorBitCount() << std::endl << "Количество исправленных ошибок: " << decoder2.getFixedBitCount() << std::endl;
	if (canal.getErrorBitCount() < decoder2.getFixedBitCount()) {
		std::cout << "Есть неверно исправленные ошибки" << std::endl;
	}
	else if (canal.getErrorBitCount() > decoder2.getFixedBitCount()) {
		std::cout << "Есть неисправленные ошибки" << std::endl << "Их количество: " << canal.getErrorBitCount() - decoder2.getFixedBitCount() << std::endl;
	}
	else {
		std::cout << "Ошибок либо не возникало, либо они все были успешно убраны" << std::endl;
	}

	writeInFile(acceptor.getMessage());
	writeInCodeFile(coder.getBinaryCode());
	writeCoderFile(coder.getHuffmanCode());

	if (status) {
		std::cout << "Данные не потеряны" << endl;
	}
	else {
		cout << "Данные потеряны" << endl;
	}

	std::string outputMessage = acceptor.getMessage();
	if (inputMessage == outputMessage)
		return EXIT_SUCCESS;

	return EXIT_FAILURE;
}


int main() {
	setlocale(LC_ALL, "ru");

	//if (!runProg("я люблю майнкрафт.", 12.5, 0))
	//	std::cout << "Строки совпадают\n" << std::endl;
	if (!runProg(readFromFile(), 0.2, 0))
		std::cout << "Строки совпадают\n" << std::endl;
	//if (!runProg("я люблю майнкрафт.", 0.6, 0))
	//	std::cout << "Строки совпадают\n" << std::endl;
	//if (!runProg(readFromFile(), 0.1, 0))
	//	std::cout << "Строки совпадают\n" << std::endl;

	//if (!runProg("я люблю майнкрафт.", 0, 1))
	//	std::cout << "Строки совпадают\n" << std::endl;
	//if (!runProg(readFromFile(), 0.05, 1))
	//	std::cout << "Строки совпадают\n" << std::endl;
	//if (!runProg("я люблю майнкрафт.", 0.4, 1))
	//	std::cout << "Строки совпадают\n" << std::endl;
	//if (!runProg(readFromFile(), 0.4, 1))
	//	std::cout << "Строки совпадают\n" << std::endl;

	//if (!runProg("я люблю майнкрафт.", 0.05, 2))
	//	std::cout << "Строки совпадают\n" << std::endl;
	//if (!runProg(readFromFile(), 0.05, 2))
	//	std::cout << "Строки совпадают\n" << std::endl;
	//if (!runProg("я люблю майнкрафт.", 0.4, 2))
	//	std::cout << "Строки совпадают\n" << std::endl;
	//if (!runProg(readFromFile(), 0.4, 2))
	//	std::cout << "Строки совпадают\n" << std::endl;

	return 0;
}
