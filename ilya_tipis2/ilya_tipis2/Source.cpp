
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

int runProg(std::string inputMessage) {
	Source source(inputMessage, noiseChance);
	Coder coder;
	SourceCodTransition trans1;
	trans1.send(source, coder);

	DataTransCanal canal;
	CodCanalTransition trans2;

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

	writeInFile(acceptor.getMessage());
	writeInCodeFile(coder.getBinaryCode());
	writeCoderFile(coder.getHuffmanCode());

	std::string outputMessage = acceptor.getMessage();
	if (inputMessage == outputMessage)
		return EXIT_SUCCESS;

	return EXIT_FAILURE;
}