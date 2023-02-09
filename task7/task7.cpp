/*
Дан массив неотрицательных целых 64-разрядных чисел.
Количество чисел не больше 1000000. Отсортировать массив методом MSD по битам (бинарный QuickSort).
*/
#include <iostream>
#include <assert.h>

void binaryQuickSort(unsigned long long* array, int n, int bitPos);
int partition(unsigned long long* array, int n, int bitPos);
int findMaxBitCount(unsigned long long* array, int n);

int main() {
    int n;
    std::cin >> n;
    assert(n <= 1000000);
    unsigned long long* array = new unsigned long long[n];
    for (int i = 0; i < n; ++i)
        std::cin >> array[i];

    // 3 параметром количество бит, лежащих левее максимального бита в подборке
    binaryQuickSort(array, n, (64 - findMaxBitCount(array, n)));
    for (int i = 0; i < n; ++i)
        std::cout << array[i] << " ";

    delete[] array;
}

// Функция разделения по 0 и 1 в MSD
int partition(unsigned long long* array, int n, int bitPos) {
    if (n <= 1) {
        return 0;
    }

    int i = 0;
    int j = n - 1;
    // Паттерн, задающий рассматриваемый бит при битовом И на элемент в виде 00...1000...
    unsigned long long bitPattern = ~(~((unsigned long long)0) >> (bitPos + 1)) & (~((unsigned long long)0) >> (bitPos));
    while (i <= j) {
        for (; i <= n - 1 && !(array[i] & bitPattern); ++i) {}
        for (; j >= 0 && (array[j] & bitPattern); --j) {}
        if (i < j) {
            std::swap(array[i++], array[j--]);
        }
    }

    return i;
}

// Функция определения количества бит у наибольшего элемента массива
int findMaxBitCount(unsigned long long* array, int n) {
    unsigned long long max = 0;
    for (int i = 0; i < n; ++i) {
        if (array[i] > max)
            max = array[i];
    }

    int bitCount = 0;
    for (; max > 0; max >>= 1)
        bitCount++;

    return bitCount;
}

// Бинарная сортировка, работающая на поэтапном проходе сравнениями текущих MSD
void binaryQuickSort(unsigned long long* array, int n, int bitPos) {
    if (bitPos >= 64)
        return;

    int part = partition(array, n, bitPos);
    if (part > 0) binaryQuickSort(array, part, bitPos + 1);
    if (part + 1 < n) binaryQuickSort(array + part, n - part, bitPos + 1);
}
