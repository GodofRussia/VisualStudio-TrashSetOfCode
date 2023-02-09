/*
В операционной системе Technux есть планировщик процессов.
Каждый процесс характеризуется:
    приоритетом P
    временем, которое он уже отработал t
    временем, которое необходимо для завершения работы процесса T
Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и кладет обратно в очередь процессов.
Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
Требуется посчитать кол-во переключений процессора.

Требования:
    В качестве очереди с приоритетом нужно использовать кучу.
    Куча должна быть реализована в виде шаблонного класса.
    Решение должно поддерживать передачу функции сравнения снаружи.
    Куча должна быть динамической.
*/

#include <iostream>
#include <assert.h>

template<typename T>
class IsLessDefault {
public:
    bool operator()(const T& l, const T& r) const { return l < r; }
};

// Класс кучи
template<typename T, class Compare = IsLessDefault<T>>
class Heap {
public:
    explicit Heap(const Compare& cmp);
    Heap(const T* array, int size, const Compare& cmp);

    ~Heap();

    T ExtractMin();
    T PeekMin() const;
    void Add(const T& elem);
    int getSize() const { return size; }

private:
    T* buffer;
    int bufferSize = 0; 
    int size = 0;
    Compare cmp;  // Вводим компаратор отдельным полем кучи

    void siftDown(int);
    void siftUp(int);
    void grow();
    void buildHeap();
};

template<typename T, class Compare>
void Heap<T, Compare>::siftDown(int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;
    if (left < size && cmp(buffer[left], buffer[i]))
        largest = left;
    if (right < size && cmp(buffer[right], buffer[largest]))
        largest = right;
    if (largest != i) {
        std::swap(buffer[i], buffer[largest]);
        siftDown(largest);
    }
}

template<typename T, class Compare>
void Heap<T, Compare>::buildHeap() {
    for (int i = size / 2 - 1; i >= 0; --i) {
        siftDown(i);
    }
}

template<typename T, class Compare>
void Heap<T, Compare>::siftUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (!cmp(buffer[index], buffer[parent]))
            return;

        std::swap(buffer[index], buffer[parent]);
        index = parent;
    }
}

// Так как куча динамическая, используем функцию увеличения размера
template<typename T, class Compare>
void Heap<T, Compare>::grow() {
    if (!bufferSize) {
        T* new_buffer = new T[++bufferSize];
        ++size;
    }

    bufferSize *= 2;
    T* new_buffer = new T[bufferSize];
    for (int i = 0; i < size; ++i)
        new_buffer[i] = buffer[i];

    delete[] buffer;
    buffer = new_buffer;
}

// Передаём компаратор в конструкторе
template<typename T, class Compare>
Heap<T, Compare>::Heap(const Compare& cmp) : cmp(cmp) { }

template<typename T, class Compare>
Heap<T, Compare>::Heap(const T* array, int size, const Compare& cmp) : cmp(cmp), bufferSize(size), size(size) {
    buffer = new T[size];
    for (int i = 0; i < size; ++i) {
        buffer[i] = array[i];
    }

    buildHeap();
}

template<typename T, class Compare>
Heap<T, Compare>::~Heap() {
    delete[] buffer;
}

template<typename T, class Compare>
void Heap<T, Compare>::Add(const T& elem) {
    if (size + 1 > bufferSize)
        grow();

    buffer[size++] = elem;
    siftUp(size - 1);
}

template<typename T, class Compare>
T Heap<T, Compare>::ExtractMin() {
    assert(size);
    T result = buffer[0];
    buffer[0] = buffer[size - 1];
    --size;

    if (!size) {
        siftDown(0);
    }

    return result;
}

template<typename T, class Compare>
T Heap<T, Compare>::PeekMin() const {
    return buffer[0];
}

struct Process {
    int P;
    int t = 0;
    int T;
};

// Класс для передачи функтора сравнения конкретно двух процессов по приоритетам
class PriorityCmp {
public:
    bool operator()(const Process& l, const Process& r) {
        return l.P * (l.t + 1) < r.P * (r.t + 1);
    }
};

// Функция работы планировщика
template<typename T, class Compare>
int runPlanner(Heap<T, Compare>& heap) {
    int switchCounter = 0;
    while (heap.getSize()) {
        Process currentProc = heap.ExtractMin();
        currentProc.t += currentProc.P;
        if (currentProc.t < currentProc.T) {
            heap.Add(currentProc);
        }

        ++switchCounter;
    }

    return switchCounter;
}

int main() {
    int processNum;
    std::cin >> processNum;
    Process* procMassive = new Process[processNum];
    for (int i = 0; i < processNum; ++i) {
        std::cin >> procMassive[i].P >> procMassive[i].T;
    }

    PriorityCmp cmp;
    Heap<Process, PriorityCmp> heap(procMassive, processNum, cmp);

    int switchCounter = runPlanner(heap);
    std::cout << switchCounter;

    delete[] procMassive;
    return 0;
}
