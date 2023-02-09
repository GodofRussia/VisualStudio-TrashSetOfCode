#include <iostream>
#include <fstream>

using namespace std;

template <class T>
class List {
    template <class T>
    class Node 
    {
        T data;
        Node *pNext, *pPrev;
    public:
        Node(T data = T(), Node* pNext = nullptr, Node* pPrev = nullptr) {
            this->data = data;
            this->pNext = pNext;
            this->pPrev = pPrev;
        }
    };
    Node<T>* head;
    Node<T>* tail;
    int Size;
private:
    List()
    {
        Size = 0;
        head = nullptr;
        tail = nullptr;
    }
    ~List()
    {
        while (Size) {
            pop_front();
        }
    }
    int GetSize() { return Size; }
    void push_back(T data);
    void push_front(T data);
    void insert(T data);
    void pop_front();
    T& operator[](const int index);

};

template<class T>
void List<T>::push_back(T data)
{
    if (tail == nullptr) {
        head = tail = new Node<T>(data);
    }
    Node<T>* current = new Node<T>(data, ,tail);
    tail->pNext = current;
    tail = current;
}

template<class T>
void List<T>::push_front(T data)
{
    if (head == nullptr) {
        head = tail = new Node<T>(data);
    }
    Node<T>* current = new Node<T>(data, head);
    head->pPrev = current;
    head = current;
}

template<class T>
void List<T>::insert(T data)
{
    if (head == nullptr) {
        head = tail = new Node<T>(data);
    }
    strlen(T)
}

template<class T>
void List<T>::pop_front()
{
    Node<T>* ptr = this->head;
    head = head->pNext;
    head->pPrev = nullptr;
    delete ptr;
}

template<class T>
T& List<T>::operator[](const int index)
{
    
    if (index < (this->GetSize() / 2)) {
        int counter = 0;
        Node<T>* current = this->head;
        while (current != nullptr) 
            {
            if (index == counter) return current->data;
            counter++;
            current = current->pNext;
            }
    }
        
    else {
        int counter = this->GetSize();
        Node<T>* current = this->tail;
        while (current != nullptr)
        {
            if (index == counter) return current->data;
            counter--;
            current = current->pPrev;
        }
    }
}

int main()
{
    setlocale(LC_ALL, "ru");
    ifstream file("input.txt");

    return 0;
}


