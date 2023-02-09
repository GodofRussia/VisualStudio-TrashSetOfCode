/*
*  Даны два массива неповторяющихся целых чисел, упорядоченные по возрастанию. A[0..n-1] и B[0..m-1]. n >> m. Найдите их пересечение.
Требования: Время работы: O(m * log k), где k - позиция элемента B[m-1] в массиве A.. В процессе поиска очередного элемента B[i]
в массиве A пользуйтесь результатом поиска элемента B[i-1]. Внимание! В этой задаче для каждого B[i]сначала нужно определить диапазон
для бинарного поиска размером порядка k с помощью экспоненциального поиска, а потом уже в нем делать бинарный поиск.
*/

#include <iostream>
#include <assert.h>
#define ELEM_DONOT_EXIST -1

int find_crossing(int* first_array, int* second_array, int* crossing_array, int n, int m);  // функция поиска пересечения 2 массивов, составляет массив пересечения
int binary_search(int* array, int l, int r, int x);
int exponential_search(int* array, int l, int n, int x);  // левая граница у экспоненциального поиска берется для учёта обхода по элементам A[i] с B[i-1]

int main() {
    int n, m;
    std::cin >> n >> m;
    assert(n <= 10000);
    int* A = new int[n];
    int* B = new int[m];
    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    for (int i = 0; i < m; ++i) {
        std::cin >> B[i];
    }

    int* C = new int[m];  // определение массива пересечения значений
    int k = find_crossing(A, B, C, n, m);  // функция возвращает длину полученного массива пересечения 
    for (int i = 0; i < k; ++i)
        (i != k - 1) ? std::cout << C[i] << " " : std::cout << C[i];

    delete[] A;
    delete[] B;
    delete[] C;
    return 0;
}

int find_crossing(int* a, int* b, int* crossing_array, int n, int m) {
    int last_index_of_B_in_A = 0, k = 0;
    for (int i = 0; i < m; ++i) {
        int index_of_elem_B_in_A = exponential_search(a, last_index_of_B_in_A, n, b[i]);  // индекс левой границы - индекс следующего
        if (index_of_elem_B_in_A == ELEM_DONOT_EXIST)                                    //  после B[i - 1] в массиве A элемента
            continue;

        crossing_array[k] = b[i];
        k++;
        last_index_of_B_in_A = index_of_elem_B_in_A + 1;
    }

    return k;
}

int exponential_search(int* array, int l, int n, int x) {
    if (l == 0) {
        if (array[0] == x)
            return 0;

        l = 1;  // если первые элементы не равны, то начинаем с индекса 1 и далее подбираем диапазон
    }

    int i;
    for (i = (l == 1 || !(l % 2)) ? l : l - 1; i < n && array[i] <= x; i *= 2);  // чтобы найти нужный элемент, нужно в индексах идти
    return binary_search(array, i / 2, std::min(i, n - 1), x);                   // по степеням двойки, однако левым краем может прийти не степень двойки,
}                                                                                // что разрушит алгоритм, поэтому мы декремируем до степени 2

int binary_search(int* array, int l, int r, int x) {
    if (r < l)
        return ELEM_DONOT_EXIST;

    int mid = l + (r - l) / 2;
    if (array[mid] == x)
        return mid;
    if (array[mid] < x)
        return binary_search(array, mid + 1, r, x);

    return binary_search(array, l, mid - 1, x);
}