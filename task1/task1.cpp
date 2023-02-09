/*
Вернуть значение бита в числе N по его номеру K.
Позиции битов нумеруются с 0.
*/

#include <iostream>
#include <assert.h>
#include <cmath>

// Функция получения бита по позиции
int bitCount(unsigned int number, int bitNum) {
	int bit;
	if (number >= std::pow(2, bitNum)) {
		bit = (number >> bitNum) & 1;
	}
	else
		bit = 0;

	return bit;
}

int main() {
	unsigned int number;
	int bitNum;
	std::cin >> number >> bitNum;
	assert(bitNum >= 0);
	assert(bitNum < 32);

	int bit = bitCount(number, bitNum);

	std::cout << bit << std::endl;
}
