#include <iostream>

int binaryInsertIndexSearch(int* array, int n, int element);

int main()
{
    int n;
    std::cin >> n;
    int* array = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> array[i];
    }
    int k;
    std::cin >> k;

    int position = binaryInsertIndexSearch(array, n, k);
    std::cout << position;

    delete[] array;
    return 0;
}

int binaryInsertIndexSearch(int* array, int n, int element) {
    int first = 0;
    int last = n;
    while (first < last) {
        int mid = (first + last) / 2;
        if (array[mid] < element)
            first = mid + 1;
        else 
            last = mid;
    }
    
    return first;
}