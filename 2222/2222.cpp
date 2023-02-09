/*
Реализовать очередь с динамическим зацикленным буфером.
Обрабатывать команды push back и pop front.
*/

#include <iostream>
#include <assert.h>

// Класс очереди с динамическим зацикленным буфером
template<typename T>
class Queue {
	T* buffer;
	size_t bufferSize;

	size_t head;
	size_t tail;

public:
	Queue() = default;
	Queue(size_t bufferSize);
	Queue(const Queue&) = delete;
	Queue(const Queue&&) = delete;
	~Queue();

	void enqueue(const T& data);
	T dequeue();
	bool IsEmpty() const;
	bool isFull() const;
};
 
// Конструктор с задающим максимальное количество элементов параметром (длина массива)
template<typename T>
Queue<T>::Queue(size_t bufferSize) : bufferSize(bufferSize) {
	head = tail = -1;
	buffer = new T[bufferSize];
}

// Деструктор
template<typename T>
Queue<T>::~Queue() {
	delete[] buffer;
}

// Проверка на заполненность очереди
template <typename T>
bool Queue<T>::isFull() const {
	if (head == 0 && tail == bufferSize - 1) {
		return true;
	} else if (head == tail + 1) {
		return true;
	}

	return false;
}

// Добавление элемента в очередь
template<typename T>
void Queue<T>::enqueue(const T& data) {
	assert(!isFull());

	if (head == -1) head = 0;
	tail = (tail + 1) % bufferSize;
	buffer[tail] = data;
}

// Извлечение элемента из очереди
template<typename T>
T Queue<T>::dequeue() {
	assert(!IsEmpty());

	T data = buffer[head];
	if (head == tail) {
		head = tail = -1;
	}
	else {
		head = (head + 1) % bufferSize;
	}

	return data;
}

// Проверка на пустоту очереди
template<typename T>
bool Queue<T>::IsEmpty() const {
	return head == -1 && tail == -1;
}

int main() {
	int n = 0;
	std::cin >> n;
	assert(n <= 1000000);

	bool result = true;
	Queue<int> queue(n);
	for (int i = 0; i < n; ++i) {
		int command = 0;
		int data = 0;
		std::cin >> command >> data;
		switch (command) {
		case 2:
			if (queue.IsEmpty()) {
				result = result && data == -1;
			} else
				result = result && queue.dequeue() == data;
			break;
		case 3:
			if (queue.isFull()) {
				result = result && data == -1;
			} else {
				queue.enqueue(data);
			}
			break;
		default:
			break;
		}
	}

	std::cout << (result ? "YES": "NO");

	return 0;
}
