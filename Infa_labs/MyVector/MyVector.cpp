#include <iostream>

using namespace std;

template <class TypeOfElement>
class MyVector {
public:
	​MyVector() {
		​​length = 0;
		​​capacity = 0;
		​​dataPointer = new TypeOfElement[capacity];
		​
	}
	​MyVector(const MyVector& rhs) {
		​​dataPointer = new TypeOfElement[rhs.Length()];
		​​for(int i = 0; i < rhs.Length(); ++i) {
			​​​dataPointer[i] = rhs.dataPointer[i];
			​​
		}
		​​length = rhs.Length();
		​
	}
	​~MyVector() {
		​​delete[] dataPointer;
		​
	}
	​void PushBack(TypeOfElement newElement);
	​TypeOfElement& operator[] (int index);
	​bool IsEmpty() const;
	​int Length() const;
	​int Capacity() const;
	​void InputMyVector(MyVector& inVector, int N);
	​void OutputMyVector(MyVector& outVector);
	​MyVector <TypeOfElement>& operator= (MyVector<TypeOfElement>& rhs);
	​TypeOfElement& At(int index) {
		​​return dataPointer[index];
		​
	}
	​MyVector <TypeOfElement> operator+(MyVector<TypeOfElement>& rhs);
	​void SetSize(int size);

private:
	​void UpCapacity(int newCapacity);
	​int length;
	​int capacity;
	​TypeOfElement* dataPointer;
};

template <class TypeOfElement>

void MyVector<TypeOfElement>::UpCapacity(int newCapacity) {
	​if(newCapacity >= capacity) {
		​​TypeOfElement* result = new TypeOfElement[capacity + 1];
		​​for(int i = 0; i < capacity; ++i) {
			​​​result[i] = dataPointer[i];
			​​
		}
		​​delete[] dataPointer;
		​​dataPointer = result;
		​
	}
}

template <class TypeOfElement>
void MyVector<TypeOfElement>::PushBack(TypeOfElement newElement) {
	​length++;
	​UpCapacity(length);
	​dataPointer[capacity++] = newElement;
}

template <class TypeOfElement>
bool MyVector<TypeOfElement>::IsEmpty() const {
	​if(length == 0) return true;
	​else return false;
}

template <class TypeOfElement>
TypeOfElement& MyVector<TypeOfElement>::operator[] (int index) {
	​return dataPointer[index];
}

template <class TypeOfElement>
int MyVector<TypeOfElement>::Length() const {
	​return length;
}

template <class TypeOfElement>
int MyVector<TypeOfElement>::Capacity() const {
	​return capacity;
}

template <class TypeOfElement>
void MyVector<TypeOfElement>::InputMyVector(MyVector<TypeOfElement>& inVector, int N) {
	​TypeOfElement el;
	​for(int i = 0; i < N; i++) {
		​​cin » el;
		​​inVector.PushBack(el);
		​
	}
}

template <class TypeOfElement>
void MyVector<TypeOfElement>::OutputMyVector(MyVector<TypeOfElement>& outVector) {
	​for(int i = 0; i < outVector.Length(); ++i) {
		​​cout « outVector[i] « " ";
		​
	}
}

template <class TypeOfElement>
MyVector <TypeOfElement>& MyVector<TypeOfElement>:: operator= (MyVector<TypeOfElement>& rhs) {
	​delete[] dataPointer;
	​dataPointer = new TypeOfElement[rhs.Length()];
	​for(int i = 0; i < rhs.Length(); ++i) {
		​​this->dataPointer[i] = rhs.At(i);
		​
	}
	​length = rhs.length;
	​capacity = rhs.capacity;
	​return* this;
}

template <class TypeOfElement>
void MyVector<TypeOfElement>::SetSize(int size) {
	​if(size >= capacity) {
		​​TypeOfElement* vec = new TypeOfElement[size];
		​​delete[] dataPointer;
		​​dataPointer = vec;
		​​length = size;
		​​capacity = size;
		​
	}
}
template <class TypeOfElement>
MyVector<TypeOfElement> MyVector<TypeOfElement>:: operator+(MyVector<TypeOfElement>& rhs) {
	​int len = this->length + rhs.length;
	​MyVector <TypeOfElement> vec;
	​vec.SetSize(len);

	​int i = 0;
	​for(i; i < this->length; ++i) {
		​​vec.dataPointer[i] = this->dataPointer[i];
		​
	}
	​for(int j = 0; j < rhs.length; ++j) {
		​​vec.dataPointer[i] = rhs.dataPointer[j];
		​​i++;
		​
	}
	​return vec;
}

template <class MyType>
void MySwap(MyType& element1, MyType& element2) {
	​MyType element3 = element1;
	​element1 = element2;
	​element2 = element3;
}

int main()
{
	​setlocale(0, "ru");
	​MyVector<int> vec1;
	​MyVector<int> vec2;
	​int n, m;
	​cout << " Введите количество элементов первого и второго векторов: ";
	​cin >> n >> m;
	​cout << " Введите элементы первого вектора: ";
	​vec1.InputMyVector(vec1, n);
	​cout << " Введите элементы второго вектора: ";
	​vec2.InputMyVector(vec2, m);
	​MyVector<int> vec3 = vec1 + vec2;
	​cout << "Измененный вектор:";
	​vec3.OutputMyVector(vec3);
	​return 0;
}