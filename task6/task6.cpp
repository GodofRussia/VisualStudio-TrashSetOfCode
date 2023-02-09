/*
Дано множество целых чисел из[0..10 ^ 9] размера n.
Используя алгоритм поиска k - ой порядковой статистики, требуется найти следующие параметры множества:
  1) 10 % перцентиль;
  2) медиана;
  3) 90 % перцентиль.

Реализуйте стратегию выбора опорного элемента “случайный элемент”.
Функцию Partition реализуйте методом прохода двумя итераторами от конца массива к началу.
*/

#include <iostream>
#include <cstdlib>

template <class T, class TCompare>
int partition(T* a, int left, int right, const TCompare& getKey);
template <class T, class TCompare>
int findKstat(T* a, int n, int k, const TCompare& getKey);

int main() {
    int n;
    std::cin >> n;
    int* array = new int[n];
    for (int i = 0; i < n; ++i)
        std::cin >> array[i];
    
    std::cout << findKstat(array, n, n * 0.1, [](int lhs, int rhs) { return lhs < rhs; }) << std::endl;  // 10% перцентиль
    std::cout << findKstat(array, n, (n / 2), [](int lhs, int rhs) { return lhs < rhs; }) << std::endl;  // медиана
    std::cout << findKstat(array, n, n * 0.9, [](int lhs, int rhs) { return lhs < rhs; }) << std::endl;  // 90% перцентиль

    delete[] array;
    return 0;
}

// Возвращает индекс опорного элемента
template <class T, class TCompare>
int partition(T* a, int left, int right, const TCompare& compare) {
    if (right - left + 1 <= 1) {
        return right;
    }

    int randomIndex = left + std::rand() % (right - left + 1);
    T pivot = a[randomIndex];
    std::swap(a[randomIndex], a[right]);
    int i = right - 1;
    for (; i >= left && !compare(a[i], pivot); --i) {}
    int j = i - 1;
    while (j >= left) {
        for (; j >= left && compare(a[j], pivot); --j) { }
        if (j >= left && !compare(a[j], pivot)) {
            std::swap(a[i--], a[j--]);
        }
    }

    if (i != right - 1)
        std::swap(a[i+1], a[right]);

    return i + 1;
}

// Поиск k-ой статистики в массиве
template <class T, class TCompare>
int findKstat(T* a, int n, int k, const TCompare& makeComp) {
    int left = 0; int right = n - 1;
    while (true) {
        int pivotPos = partition(a, left, right, makeComp);
        if (pivotPos == k)
            return a[k];

        if (k < pivotPos) {
            right = pivotPos - 1;
        } else {
            left = pivotPos + 1;
        }
    }
}
