/*
Необходимо реализовать и использовать сортировку слиянием в виде шаблонной функции. 
Решение должно поддерживать передачу функции сравнения снаружи.
Общее время работы алгоритма O(n log n).

Группа людей называется современниками если был такой момент, когда они могли собраться вместе.
Для этого в этот момент каждому из них должно было уже исполниться 18 лет, но ещё не исполниться 80 лет.
Дан список Жизни Великих Людей. Необходимо получить максимальное количество современников.
В день 18летия человек уже может принимать участие в собраниях, а в день 80летия и в день смерти уже не может.
Замечание: Человек мог не дожить до 18-летия, либо умереть в день 18-летия. В этих случаях принимать участие в собраниях он не мог.
*/

#include <iostream>

// Структура для содержания точки из промежутка ДМГ
struct Gap {
    int day, month, year;
    int delta = 0;
};

// Класс с функтором для сравнения
class Compare {
public:
    bool operator()(const Gap& l, const Gap& r) const {
        if (l.year < r.year)
            return true;
        else if (l.year == r.year && l.month < r.month)
            return true;
        else if (l.year == r.year && l.month == r.month && l.day < r.day)
            return true;
        else if (l.year == r.year && l.month == r.month && l.day == r.day && l.delta == -1) {
            return true;
        }
        else
            return false;
    }
};

// Слияние массивов по функции сравнения в Compare
template<typename T, class Compare>
void Merge(T* array, int left, int right, int middle, const Compare& cmp) {
    int leftSize = middle - left + 1;
    int rightSize = right - middle;
    T* lArray = new T[leftSize];
    T* rArray = new T[rightSize];

    for (int i = 0; i < leftSize; ++i)
        lArray[i] = array[left + i];
    for (int i = 0; i < rightSize; i++)
        rArray[i] = array[middle + i + 1];

    int mergedInd = left;
    int lArrayInd = 0, rArrayInd = 0;
    while (lArrayInd < leftSize && rArrayInd < rightSize) {
        if (cmp(lArray[lArrayInd], rArray[rArrayInd])) {
            array[mergedInd] = lArray[lArrayInd++];
        }
        else {
            array[mergedInd] = rArray[rArrayInd++];
        }
        ++mergedInd;
    }

    while (lArrayInd < leftSize) {
        array[mergedInd++] = lArray[lArrayInd++];
    }
    while (rArrayInd < rightSize) {
        array[mergedInd++] = rArray[rArrayInd++];
    }

    delete[] lArray;
    delete[] rArray;
}

// Шаблонная сортировка слияниями
template<typename T, class Compare>
void MergeSort(T* array, int left, int right, const Compare& cmp) {
    if (left >= right) {
        return;
    }

    int mid = left + (right - left) / 2;
    MergeSort(array, left, mid, cmp);
    MergeSort(array, mid + 1, right, cmp);
    Merge(array, left, right, mid, cmp);
}

// Функция добавление двух промежутков по ссылкам (начальная дата похождений на встречи и конечная)
bool addGap(Gap& lGap, Gap& rGap, int lday, int lmonth, int lyear, int rday, int rmonth, int ryear) {
    // Проверяем, дожил ли человек до совершеннолетия, в случае неудачи возвращаем флаг ошибки
    if ((lyear + 18 > ryear) || ((lyear + 18 == ryear) && (lmonth > rmonth || (lmonth == rmonth && lday >= rday))))
        return EXIT_FAILURE; 

    lGap.day = lday; lGap.month = lmonth; lGap.year = lyear + 18;
    if (ryear < lyear + 80) {
        rGap.year = ryear; rGap.month = rmonth; rGap.day = rday;
    }
    else if (ryear > lyear + 80) {
        rGap.year = lyear + 80; rGap.month = lmonth; rGap.day = lday;
    }
    else {
        rGap.year = ryear;
        if (lmonth < rmonth) {
            rGap.month = lmonth; rGap.day = lday;
        }
        else {
            rGap.month = rmonth; rGap.day = rday;
        }
    }

    lGap.delta = 1; rGap.delta = -1;  // Обозначение появления или удаления точки с прямой
    return EXIT_SUCCESS;
}

// Функция поиска максимального наложения промежутков разрешенного посещения встреч современников
int contemporCount(Gap* array, int size) {
    Compare cmp;
    MergeSort(array, 0, size - 1, cmp);

    int counter = 0;
    int maxCounter = 0;
    for (int i = 0; i < size; ++i) {
        if (array[i].delta > 0)
            counter++;
        else
            counter--;

        if (counter > maxCounter)
            maxCounter = counter;
    }

    return maxCounter;
}

int main() {
    int n;
    std::cin >> n;
    Gap* array = new Gap[n * 2];
    int suitableGapsNum = 0;
    for (int i = 0; i < n; ++i) {
        int lday, lmonth, lyear, rday, rmonth, ryear;
        std::cin >> lday >> lmonth >> lyear >> rday >> rmonth >> ryear;
        if (!addGap(array[suitableGapsNum], array[suitableGapsNum + 1], lday, lmonth, lyear, rday, rmonth, ryear))  // не добавляем в случае возврата EXIT_FAILURE
            suitableGapsNum += 2;
    }

    int maxCount = contemporCount(array, suitableGapsNum);
    std::cout << maxCount;
    delete[] array;
    return 0;
}
