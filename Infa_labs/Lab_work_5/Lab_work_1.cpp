#include <iostream>

using namespace std;

template<class T>
class List {
    template<class T>
    class Node {
    public:
        Node(T data = T(), int index = -1, Node* pNext = nullptr) {
            this->data = data;
            this->index = index;
            this->pNext = pNext;
        }
        T data;
        int index;
        Node* pNext;
    };
    Node<T> *head;
    int copynumber;
    int Size;
public:
    List() {
        Size = 0;
        copynumber = 0;
        head = nullptr;
    }
    List(const List<T> &ob) 
    {
        this->head = ob.head;
        this->Size = ob.Size;
        copynumber = ob.copynumber + 1;
    }
    ~List()
    {
        if (!copynumber) {
            while(Size > 0) pop_Front();
        }
        else {
            Size = 0;
        }
    }  
    int get_Size() { return Size; }
    void pop_Front();
    int get_Index(Node<T>& node) { return node.index; }
    void push_back(T data, int index);
    T& operator[](const int index);
    List<T> operator+(List<T>& lst);
    //List<T>& operator=(List<T>& lst);
    };

template<class T>
void List<T>::push_back(T data, int index) {
    if (this->head == nullptr) {
        head = new Node<T>(data, index);
        Size++;
        return;
    }
    Node<T> *current = this->head;
    while (current->pNext != nullptr) {
        current = current->pNext;
    }
    current->pNext = new Node<T>(data, index);
    Size++;
}

template <class T>
T& List<T>:: operator[](const int index)
{
    int counter = 0;
    Node<T>* current = this->head;
    while (current != nullptr) {
        if (counter == index) {
            return current->data;
        }
        current = current->pNext;
        counter++;
    }
}

template<class T>
List<T> List<T>::operator+(List<T>& lst)
{
    List<T> new_list;
    if (this->get_Size() > lst.get_Size()) {
        Node<T>* current = lst.head;
        int index = 0;
        while (current != nullptr)
        {
            int ind = 0;
            int cur_ind = current->index;
            Node<T>* other_node = this->head;
            while ((other_node != nullptr) && (ind <= cur_ind))
            {
                if (other_node->index == cur_ind) 
                {
                    new_list.push_back((current->data + other_node->data), cur_ind);
                    break;
                }
                other_node = other_node->pNext;
                ind++;
            }
            current = current->pNext;
            index++;
        }
    }
    else {
        Node<T>* current = this->head;
        int index = 0;
        while (current != nullptr)
        {
            int ind = 0;
            int cur_ind = current->index;
            Node<T>* other_node = lst.head;
            while (other_node != nullptr && ind <= cur_ind)
            {
                if (other_node->index == cur_ind)
                {
                    new_list.push_back(current->data + other_node->data, cur_ind);
                    break;
                }
                other_node = other_node->pNext;
                ind++;
            }
            current = current->pNext;
            index++;
        }
    }
    new_list.copynumber = 1;
    return new_list;
}

template<class T>
void List<T>::pop_Front() {
    Node<T>* current = this->head;
    this->head = current->pNext;
    delete current;
    Size--;
}

template <class T>
void show_list(List<T>& lst);

int main()
{
    setlocale(LC_ALL, "ru");
    List<double> list1;
    List<double> list2;
    int n, m;
    cout << "Введите количество элементов первого массива: ";
    cin >> n;
    double* array1 = new double[n];
    cout << "Введите " << n << " элементов первого массива: " << endl;
    for (int i = 0; i < n; i++)
    {
        cin >> array1[i];
        if (array1[i] != 0) list1.push_back(array1[i], i);
    }
    cout << "Введите количество элементов второго массива: ";
    cin >> m;
    double* array2 = new double[m];
    cout << "Введите " << m << " элементов второго массива: " << endl;
    for (int i = 0; i < m; i++)
    {
        cin >> array2[i];
        if (array2[i] != 0) list2.push_back(array2[i], i);
    }

    List<double> list3;
    list3 = list1 + list2;

    cout << "Первый список: " << endl;
    show_list(list1);
    cout << "Второй список: " << endl;
    show_list(list2);
    cout << "Полученный список: " << endl;
    show_list(list3);

    delete[] array1;
    delete[] array2;
    return 0;
}

template <class T>
void show_list(List<T>& lst) {
    for (int i = 0; i < lst.get_Size(); i++) {
        cout << lst[i] << " ";
    }
    cout << endl;
}
